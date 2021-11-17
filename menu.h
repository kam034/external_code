#ifndef EXTERNAL_CODE_MENU_H
#define EXTERNAL_CODE_MENU_H

#include "library.h"
using namespace Laba3;


template<class T>
void getNum(T &num);

inline char* conj(char* c, const char* s1, const char* s2, int time);
inline char* disj(char* c, const char* s1, const char* s2, int time);

void conjunction(Binary_signal& a);
void disjunction(Binary_signal& a);
void negation(Binary_signal& a);
void print_menu();
void print_table(Binary_signal& a, const char* s1, const char* s2, int time);
void menu();


#endif //EXTERNAL_CODE_MENU_H
