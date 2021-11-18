#pragma once

#include <fstream>
#include <string>

#include "../Data.h"
#include "../holders/Canton.h"
#include "../../common/SvkStr.h"

using std::ifstream;
using std::istringstream;
using std::getline;

class CsvParser
{

public:
    
    static Canton* readCanton();

private:

    static constexpr char delimeter = ';';

};

inline Canton* CsvParser::readCanton()
{
    // Files have 9 columns together, 2 columns are identical
    constexpr int valueCount = 8;

    ifstream fileObce(data::filePath(data::Source::Obce));
    ifstream fileClen(data::filePath(data::Source::Clenenie));
    string line;
    SvkStr values[valueCount];
    istringstream* stream;

    Canton* stat = new Canton("Slovensko", Canton::Type::Stat);
    Canton* kraj = nullptr;
    Canton* okres = nullptr;
    Canton* obec = nullptr;
    int i = 0;

    if (!fileObce.is_open() || !fileClen.is_open())
    {
        throw std::runtime_error("CsvParser::readCanton() : Files could not be opened.");
    }

    // Pass the header
    getline(fileObce, line);
    getline(fileClen, line);

    while (getline(fileObce, line))
    {
        stream = new istringstream(line);

        for (i = 0; getline(*stream, values[i], delimeter); i++)
        {}

        delete stream;

        // Should always be true
        if (getline(fileClen, line))
        {
            stream = new istringstream(line);
            getline(*stream, values[i], delimeter);

            for (; getline(*stream, values[i], delimeter); i++)
            {}

            delete stream;
        }

        if (kraj)
        {
            kraj = kraj->levelFind(values[7]);
        }
        if (kraj == nullptr)
        {
            kraj = new Canton(values[7], Canton::Type::Kraj, stat);
        }

        if (okres)
        {
            okres = okres->levelFind(values[6]);
        }
        if (okres == nullptr)
        {
            okres = new Canton(values[6], Canton::Type::Okres, kraj);
        }

        obec = Canton::parse(values, Canton::Type::Obec, okres);
        
        // Aggregate values
        *stat += *obec;
        *kraj += *obec;
        *okres += *obec;
    }

    // Release resources
    fileObce.close();
    fileClen.close();
    
    return stat;
}
