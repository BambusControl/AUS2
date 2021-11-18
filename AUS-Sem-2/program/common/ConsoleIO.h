#pragma once
#include <iostream>
#include <string>
#include <initializer_list>

#include "SvkStr.h"

using std::cout;
using std::cin;
using std::endl;
using std::getline;
using std::string;
using std::initializer_list;

class ConsoleIO 
{

public:

    template <typename Type>
    static Type numberBetween(const Type max, const Type min = 1, const Type offset = 0);
        
    template <typename Type>
    static Type numberAtLeast(const Type maxValue, const Type min = 0);

    static SvkStr textLine();

    static inline void writeValues(const initializer_list<string>& values, const int offset = 1);

    static int chooseFrom(const initializer_list<string> values, const char* prompt = nullptr, const int offset = -1);
   
    static bool decide(const char* yesValue, const char* noValue, const char* prompt = nullptr);
   
    static inline bool decide(const char* prompt = nullptr);

};

template <typename Type>
inline Type ConsoleIO::numberBetween(const Type max, const Type min, const Type offset)
{
    Type input;
    bool repeat = true;

    while (repeat)
    {
        cout << "Napíšte èíslo od " << min << " po " << max << " : ";
        cin >> input;

        if (input < min || input > max || cin.fail())
        {
            cout << "Napíšte èíslo od " << min << " po " << max << '!' << endl;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(256, '\n');
            }
        }
        else
        {
            repeat = false;
        }
    };

    cout << endl;
    return input + offset;
}

template<typename Type>
inline Type ConsoleIO::numberAtLeast(const Type maxValue, const Type min)
{
    Type input;
    bool repeat = true;

    while (repeat)
    {
        cout << "Napíšte èíslo (>=" << min << ") : ";
        cin >> input;

        if (input < min || cin.fail())
        {
            cout << "Napíšte èíslo od " << min << " po " << maxValue << '!' << endl;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(256, '\n');
            }
        }
        else
        {
            repeat = false;
        }
    };

    cout << endl;
    return input;
}

inline SvkStr ConsoleIO::textLine()
{
    SvkStr input;
    bool repeat = true;

    cin.ignore(LLONG_MAX, '\n');

    do
    {
        cout << "Napíšte reazec znakov:\n";
        getline(cin, input);

        if (cin.fail())
        {
            cout << "Napíšte reazec znakov!\n";

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(256, '\n');
            }
        }
        else
        {
            repeat = false;
        }
    } while (repeat);

    cout << endl;
    return input;
}

inline void ConsoleIO::writeValues(const initializer_list<string>& values, const int offset)
{
    const string* iter = values.begin();

    for (int i = offset; iter < values.end(); i++)
    {
        cout << '\t' << i << ". " << *iter++ << endl;
    }
}

inline int ConsoleIO::chooseFrom(const initializer_list<string> values, const char* prompt, const int offset)
{
    if (prompt)
    {
        cout << prompt << endl;
    }

    writeValues(values, 1);
    return numberBetween<int>(values.size(), 1, offset);
}

inline bool ConsoleIO::decide(const char* yesValue, const char* noValue, const char* prompt)
{
    if (prompt)
    {
        cout << prompt << endl;
    }

    cout << '\t' << "1. " << yesValue << endl;
    cout << '\t' << "2. " << noValue << endl;

    return numberBetween<int>(2, 1) == 1;
}

inline bool ConsoleIO::decide(const char* prompt)
{
    return decide("Áno", "Nie", prompt);
}

