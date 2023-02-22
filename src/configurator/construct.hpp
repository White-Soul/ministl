
#pragma once

#ifndef MINISTL_CONSTRUCT_H
#define MINISTL_CONSTRUCT_H

#include <new>
#include <type_traits>
#include "../algorithm/stl_algorithm.hpp"
#include "../utils/util.hpp"

_MINISTL_BEGIN

template <class T>
inline void construct(T* ptr) {
  ::new ((void*)ptr) T();
}

template <class T1, class T2>
inline void construct(T1* ptr, const T2& value) {
  ::new ((void*)ptr) T1(value);
}

// destroy
template <class T>
inline void destroy(T* pointer) {
  if (pointer != nullptr)
    pointer->~T();
}

// 如果有trivial destructor
template <class ForwardIter>
inline void _destry_aux(ForwardIter first, ForwardIter last, _false_type) {
  for (; first < last; ++first) {
    destroy(&*first);
  }
}

// 如果没有trivial destructor
template <class ForwardIter>
inline void _destry_aux(ForwardIter first, ForwardIter last, _true_type) {}

// 判断元素类别
template <class ForwardIter, class T>
inline void _destroy(ForwardIter first, ForwardIter last, T*) {
  typedef typename _type_traits<T>::has_trivial_destructor trivial_destructor;
  _destry_aux(first, last, trivial_destructor());
}
// 此函数设法找出元素类别，利用type_traits<>求取最适当措施
template <class ForwardIter>
inline void destroy(ForwardIter first, ForwardIter last) {
  _destroy(first, last, value_type(first));
}

_MINISTL_END

#endif
