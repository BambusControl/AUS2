#include <fstream>
#include <iostream>
#include <iostream>
#include <string>

#include <windows.h>
#include <wincon.h>

#include "common/heap_monitor.h"

#include "Menu.h"
#include "common/SvkStr.h"
#include "structures/table/sorted_sequence_table.h"

#include "data/parsers/CsvParser.h"

using namespace std;

void test();

int main()
{
    // TODO

    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);
    setlocale(LC_ALL, "slovak");

    initHeapMonitor();
    /*
    auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    for (size_t i = 1; i < 16; i++)
    {
        SetConsoleTextAttribute(hConsole, i);
        cout << i << ' ';

        if (i % 50 == 0)
        {
            cout << endl;
        }
    }

    for (size_t i = 0; i < 100; i++)
    {
        SetConsoleTextAttribute(hConsole, 4 + (6 * (i % 2)));
        cout << "Sample text" << endl;
    }*/

    Menu m;
    
    m.run();
    return 0;

    /*try
    {
        m.run();
    }
    catch (std::exception e)
    {
        cerr << "\n ERROR:\n" << e.what() << endl;
    }

    return 0;
    */
}

void test()
{
    using namespace std;

    auto c = CsvParser::readCanton();
    delete c;


}