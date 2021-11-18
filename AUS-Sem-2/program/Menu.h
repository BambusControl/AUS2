#pragma once
#include <future>
#include <iostream>

#include "Filter.h"
#include "data/handlers/Sequencer.h"
#include "common/ConsoleIO.h"
#include "data/handlers/formatters/DataFormatter.h"
#include "data/parsers/CsvParser.h"
#include "structures/table/unsorted_sequence_table.h"

using io = ConsoleIO;
using std::cout;
using std::endl;

class Menu
{

public:

    Menu();
    ~Menu();

    void run();

private:

    std::future<Canton*> dataTask;
    Canton* data;

    void askAndRun();
    void runCustom();
    void run03();
    void run04();

};

inline Menu::Menu() :
    dataTask(std::async(std::launch::async, CsvParser::readCanton)),
    data(nullptr)
{}

inline Menu::~Menu()
{
    delete data;
    data = nullptr;
}

inline void Menu::run()
{
    do
    {
        askAndRun();
        cout << endl;
    } while (io::decide("éel·te si opakovaù?"));
}

inline void Menu::askAndRun()
{
    auto input = io::chooseFrom(
        {
            "VypÌsanie inform·ciÌ o ˙zemn˝ch jednotk·ch",
            "Zoradenie ˙zemn˝ch jednotiek spÂÚaj˙cich dan˝ filter",
            "Vlastn˝ v˝ber filtrov"
        },
        "Vyber si funkcionalitu:"
    );

    switch (input)
    {
    case 0:
        run03();
        break;

    case 1:
        run04();
        break;

    case 2:
        runCustom();
        break;

    default:
        throw std::logic_error("Menu::selectLevel : invalid level.");
    }
}

inline void Menu::runCustom()
{
    using UnsortedTable = structures::UnsortedSequenceTable<SvkStr, Canton*>;
    using CT = CriterionType;
    using FT = FilterType;
    using DF = DataFormatter;

    // Await loaded data
    if (data == nullptr)
    {
        data = dataTask.get();
    }

    // Filter the data
    auto table = 
        DF::filterCanton(*data, {
            FT::UJNazov,
            FT::UJPocetObyvatelov,
            FT::UJZastavanost,
            FT::UJTyp,
            FT::UJPrislusnost
        });

    // Write table data
    DF::writeTable(*table, {
        CT::UJ_Nazov,
        CT::UJ_Typ,
        CT::UJ_PocetMladych,
        CT::UJ_PocetDospelych,
        CT::UJ_PocetDochodcov,
        CT::UJ_PocetObyvatelov,
        CT::UJ_CelkovaVymera,
        CT::UJ_ZastavanaPlocha,
        CT::UJ_Zastavanost
    });

    // Release resources
    delete table;
}

inline void Menu::run03()
{
    using CT = CriterionType;
    using FT = FilterType;
    using DF = DataFormatter;

    // Ask user for filter
    auto input = io::chooseFrom(
        { "N·zov", "PoËet obyvateæov", "Zastavanosù" },
        "Vyber si dodatoËn˝ filter:"
    );

    // Await loaded data
    if (data == nullptr)
    {
        data = dataTask.get();
    }

    // Filter the data
    auto table = 
        DF::filterCanton(*data, {
            FT::UJTyp,
            FT::UJPrislusnost,
            static_cast<FT>(input)
        });

    // Write table data
    DF::writeTable(*table, {
        CT::UJ_Nazov,
        CT::UJ_PocetObyvatelov,
        CT::UJ_PocetMladych,
        CT::UJ_PocetDospelych,
        CT::UJ_PocetDochodcov,
        CT::UJ_CelkovaVymera,
        CT::UJ_ZastavanaPlocha,
        CT::UJ_Zastavanost
    });

    // Release resources
    delete table;
}

inline void Menu::run04()
{
    using UnsortedTable = structures::UnsortedSequenceTable<SvkStr, Canton*>;
    using CT = CriterionType;
    using FT = FilterType;
    using DF = DataFormatter;

    // Ask for sorting criterion 
    auto input = io::chooseFrom(
        { "N·zov", "PoËet obyvateæov", "Zastavanosù" },
        "Vyber si zoraÔovacie kritÈrium:"
    );

    CT type;
    switch (input)
    {
    case 0:
        type = CT::UJ_Nazov;
        break;

    case 1:
        type = CT::UJ_PocetObyvatelov;
        break;

    case 2:
        type = CT::UJ_Zastavanost;
        break;

    default:
        throw std::logic_error("Menu::run04() : unexpected input value!");
    }

    // Await loaded data
    if (data == nullptr)
    {
        data = dataTask.get();
    }

    // Filter the data table
    auto table = 
        DF::filterCanton(*data, {
            FT::UJTyp,
            FT::UJPrislusnost
        });

    // Sort the table
    DF::sort(
        *reinterpret_cast<UnsortedTable*>(table), 
        type
    );

    if (type == CT::UJ_Nazov)
    {
        DF::writeTable(*table, {
            CT::UJ_Nazov
        });
    }
    else
    {
        DF::writeTable(*table, {
            CT::UJ_Nazov,
            type
        });
    }


    // Release resources
    delete table;
}
