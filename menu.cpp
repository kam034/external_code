#include "menu.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <climits>

template<class T>
void getNum(T &num) {
    while (true) {
        std::cin >> num;
        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
    }
}

inline char* conj(char* c, const char* s1, const char* s2, int time) {
    for (int i = 0; i < time; i++)
        c[i] = (s1[i] == '1' || s2[i] == '1') ? '1' : '0';
    return c;
}


inline char* disj(char* c, const char* s1, const char* s2, int time) {
    for (int i = 0; i < sizeof(s1) - 1; i++)
        c[i] = (s1[i] == '1' && s2[i] == '1') ? '1' : '0';
    return c;
}


void conjunction(Binary_signal& a) {
    std::cout << "Enter the size of both signals -> " << std::endl;
    int time;
    std::cin >> time;

    std::cout << "The first signal ->" << std::endl;
    std::string string1;
    std::cin >> std::setw(time) >> string1;

    std::cout << "The second signal ->" << std::endl;
    std::string string2;
    std::cin >> std::setw(time) >> string2;

    char* c;
    try {
        c = new char [time+1];
    }
    catch (std::bad_alloc& e)
    {
        std::cout << "Error: memory allocation! " << e.what() << '\n';
    }
    const char* str1 = string1.c_str();
    const char* str2 = string2.c_str();

    c = conj(c, str1, str2, time);

    std::string string(c);

    try {
        a = Binary_signal(c, time, 255);
    }
    catch (std::invalid_argument &ia) {
        std::cerr<< ia.what() << std::endl;
        std::cout << "The signal was not processed!";
    }

    print_table(a, str1, str2, time);

    std::cout << "Output signal -> ";
    std::cout << a;

    delete [] c;
}


void disjunction(Binary_signal& a) {
    std::cout << "Enter the size of both signals -> " << std::endl;
    int time;
    std::cin >> time;

    std::cout << "The first signal ->" << std::endl;
    std::string string1;
    std::cin >> std::setw(time) >> string1;

    std::cout << "The second signal ->" << std::endl;
    std::string string2;
    std::cin >> std::setw(time) >> string2;

    char* c;
    try {
        c = new char [time+1];
    }
    catch (std::bad_alloc& ba)
    {
        std::cout << "Error: memory allocation! " << ba.what() << '\n';
    }
    const char* str1 = string1.c_str();
    const char* str2 = string2.c_str();

    c = disj(c, str1, str2, time);

    std::string string(c);
    try {
        a = Binary_signal(c, time, 255);
    }
    catch (std::invalid_argument &ia) {
        std::cerr<< ia.what() << std::endl;
        std::cout << "The signal was not processed!";
    }
    std::cout << "Output signal -> ";
    std::cout << a;

    delete [] c;
}


void negation(Binary_signal& a) {
    a = ~ a;
    std::cout << "Output signal -> ";
    std::cout << a;
}

void print_menu() {
    const char* MSGS[] = {"\n0.Exit",
                          "1. Conjunction or logical addition - &&",
                          "2. Disjunction or logical multiplication - ||",
                          "3. Inversion or logical negation ",
                          "Please, make your choice ->"};

    for (auto  i : MSGS) {
        std::cout << i << std::endl;
    }
}

void print_table(Binary_signal& a, const char* s1, const char* s2, int time) {
    std::cout << "===The truth table==" << std::endl;
    for (int i = 0; i < time; i++) {
        std::cout << "│" << s1[i] << "│" << s2[i] << "│" << a.getValue(i) << "│" << std::endl;
    }
}


void menu() {
    Binary_signal bs;
    std::cout << "This is a program for working with binary signals." << std::endl;
    std::cout << "Here are the actions you can do: " << std::endl;
    int choice;

    do {
        print_menu();
        getNum(choice);
        switch (choice) {
            case 0:
                std::cout << "EXIT!" << std::endl;
                break;
            case 1:
                conjunction(bs);
                break;
            case 2:
                disjunction(bs);
                break;
            case 3:
                negation(bs);
                break;
            default:
                std::cout << "ERROR! There is no such command." << std::endl;
        }
    } while (choice != 0);
}

