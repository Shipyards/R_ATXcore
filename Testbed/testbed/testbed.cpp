// testbed.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "R_JATXcore.hpp"
#include "sayhi.hpp"
#include "jarray.hpp"

int main()
{
    JATX::core_init(4);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    JATX::core_addtask(new sayhi());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    JATX::core_addtask(new sayhi());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    JATX::core_deinit();

    JATX::R_jarray<int>* orangesize = new JATX::R_jarray<int>(5);

    orangesize->operator[](3) = 5;
    orangesize->operator[](4) = 7;

    std::cout << orangesize->operator[](4) << std::endl;

    delete orangesize;
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
