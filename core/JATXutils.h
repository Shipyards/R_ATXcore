/*
   Copyright 2024 Jacob T. Ray

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once

inline void JATXsleep(int);
inline void clearScreen();
#ifdef __linux__ //for linux
#include <unistd.h>
inline void JATXsleep(int sleepamt) { usleep(sleepamt); }
inline void clearScreen() { system("clear"); }
#elif __APPLE__ //for apple
#include <unistd.h>
inline void JATXsleep(int sleepamt) { usleep(sleepamt); }
inline void clearScreen() { system("clear"); }
#elif _WIN32 //for windows
#include <windows.h>
inline void JATXsleep(int sleepamt) { Sleep(sleepamt); }
inline void clearScreen() { system("cls"); }
#else
#endif

#include <iostream>
#include <random>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <mutex>


namespace JATX
{
    class ID10T_ERR {}; // ID10T error

    // wrapper to protect any variabe in multithreading
    template<class type>
    class threadsafe
    {
    private:
        type internalvar;
        std::mutex protec;
    public:
        void _lock() { this->protec.lock(); }
        void _unlock() { this->protec.unlock(); }
        type& _get()
        {
            this->_lock();
            return this->interalvar;
        }
    };

    /* highly inefficient random from range number generator */
    static int randomFromRange(const int range_from, const int range_to)
    {
        using namespace std;
        random_device rand_dev;
        mt19937 generator(rand_dev());
        uniform_int_distribution<int>  distr(range_from, range_to);
        return distr(generator);
    }
    /* fuction to print to screen with a delay inbetween each character, delay is in milliseconds and is sightly randomized */
    static void timeDelayPrint(std::string printThis, int delay)
    {
        using namespace std;
        //cout << endl;
        for (int b = 0; b < printThis.length(); b++)
        {
            cout << printThis[b] << flush;
            JATXsleep(delay + randomFromRange(0,20) * (delay / 2));
        };
    };
    /* theoretically clears terminal */
    static void clrscreen()
    {
        clearScreen();
    }
    /* simple if string is integer */
    /*static bool strIsInt(std::string in)
    {
        if (in.empty() || (isdigit(in[0]) && (!(in[0] != '-') && !isdigit(in[1])) && (!(in[0] != '+') && !isdigit(in[1])))) return false;
        else if (isdigit(in[0]) || ((in[0] != '-') && isdigit(in[1])) || ((in[0] != '+') && isdigit(in[1]))) return true;
    };*/
    /* converts a character (0,1,2,3,4,5,6,7,8,9) into its actual numerical value, rather than its ascii value, if it is not valid for coversion, function returns 10 */
    static unsigned short int chartoint(char charin)
    {
        switch(charin)
        {
        case 48:
            return 0;
            break;
        case 49:
            return 1;
            break;
        case 50:
            return 2;
            break;
        case 51:
            return 3;
            break;
        case 52:
            return 4;
            break;
        case 53:
            return 5;
            break;
        case 54:
            return 6;
            break;
        case 55:
            return 7;
            break;
        case 56:
            return 8;
            break;
        case 57:
            return 9;
            break;
        default:
            return 10;
            break;
        }
    }
    static void swap_space_for_(std::string &strin)
    {
        using namespace std;
        string::iterator it1;
        for (it1 = strin.begin(); it1 != strin.end(); it1++)
        {
            if (*it1 == ' ')
            {
                *it1 = '_';
            }
        }
    }
    /*static std::vector<int> extractintfromstr(std::string strin)
    {
        using namespace std;
        vector<int> ints;
        string::iterator strit;
        for (strit = strin.begin(); strit != strin.end(); strit++)
        {
            if (isdigit(*strit))
            {
                ints.push_back(chartoint(*strit));
            }
        }
        return ints;
    }*/
    //string to an unsigned integer
    static unsigned int str_to_unsint(std::string strin)
    {
        using namespace std;
        unsigned int returnint = 0;
        unsigned int place = 1;
        string::reverse_iterator strit;
        for (strit = strin.rbegin(); strit != strin.rend(); strit++)
        {
            if (isdigit(*strit))
            {
                returnint += chartoint(*strit) * place;
                place = place * 10;
            }
        }
        return returnint;
    }
    static int str_to_int(std::string strin) //only picks up sign if it is first (no spaces first)
    {
        using namespace std;
        unsigned int returnint = 0;
        unsigned int place = 1;
        bool negative = false;
        string::reverse_iterator strit;
        if (strin[0] == '-')
        {
            negative = true;
        }
        for (strit = strin.rbegin(); strit != strin.rend(); strit++)
        {
            if (isdigit(*strit))
            {
                returnint += chartoint(*strit) * place;
                place = place * 10;
            }
        }
        if (negative)
        {
            returnint * -1;
        }
        return returnint;
    }
    // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    static std::string currentDateTime() {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        localtime_s(&tstruct,&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        return buf;
    }
};