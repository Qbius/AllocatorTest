#include "analize.h"
#include <vector>
#include <deque>

template<typename T>
void temp(const T&)
{
}

template<template<typename, typename> typename T>
void temptemp(const T<int, std::allocator<int>>&)
{
}

int main()
{
    data_dump dump;

    dump
    >> analize<std::vector>{1, 2, 3}
    >> analize<std::deque>{1, 2, 3}
    ;
}