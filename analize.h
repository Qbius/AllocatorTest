#ifndef __ANALIZE_H__
#define __ANALIZE_H__

#include <iostream>
#include <string>
#include <initializer_list>

#include <vector>
#include <list>
#include <deque>

#include "allol.h"

template<template <typename, typename> class TContainer, typename TValueType>
struct full_stats
{
    std::string container_name;
    std::string value_name;
    typename allol<TValueType>::call_stats call_stats;

public:

    full_stats(const std::string& con_name, const std::string& val_name, const typename allol<TValueType>::call_stats& s) : container_name(con_name), value_name(val_name), call_stats(s)
    {
    }
};

template<template <typename, typename> class TContainer, typename TValueType>
void analize(const typename full_stats<TContainer, TValueType>& stats)
{
    std::cout << stats.container_name << " " << stats.value_name << " stats" << std::endl;
    std::cout << stats.call_stats.alloc_count << " times allocated" << std::endl;
    std::cout << stats.call_stats.dealc_count << " times deallocated" << std::endl;
    std::cout << stats.call_stats.cnstr_count << " constructors called" << std::endl;
    std::cout << stats.call_stats.destr_count << " destructors called" << std::endl;
}

template<template <typename, typename> class TContainer, typename TValueType>
typename full_stats<TContainer, TValueType> populate(const std::initializer_list<TValueType>& i_list)
{
    allol<TValueType>::reset_stats();
    TContainer<TValueType, allol<TValueType>> c{ i_list };

    TValueType temp;
    return { typeid(&c).name(), typeid(&temp).name(), allol<TValueType>::stats };
}

template<template <typename, typename> class TContainer, typename TValueType>
void test(const std::initializer_list<TValueType>& i_list)
{
    analize<TContainer, TValueType>(populate<TContainer>(i_list));
}

template<typename TValueType>
void test_sequences(const std::initializer_list<TValueType>& i_list)
{
    test<std::vector>(i_list);
    test<std::list>(i_list);
    test<std::deque>(i_list);
}

#endif
