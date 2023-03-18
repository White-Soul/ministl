#ifndef MINISTL_UTIL_H
#define MINISTL_UTIL_H

#include <climits>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <new>
#include <type_traits>

#define _MINISTL_BEGIN namespace ministl {
#define _MINISTL_END }

_MINISTL_BEGIN

template <class T1, class T2>
struct pair {
  typedef T1 first_type;
  typedef T2 second_type;

  T1 first;
  T2 second;

  pair() : first(T1()), second(T2()) {}
  pair(const T1& a, const T2& b) : first(a), second(b) {}
  pair(const pair<T1,T2> & p) : first(p.first), second(p.second){}
};

_MINISTL_END

#endif