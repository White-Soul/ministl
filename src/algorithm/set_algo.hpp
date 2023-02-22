#ifndef MINISTL_SET_ALGO_H
#define MINISTL_SET_ALGO_H

#include "../utils/util.hpp"

_MINISTL_BEGIN

// set_union 构造并集
// 版本一
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_union(InputIter1 first1,
                     InputIter1 last1,
                     InputIter2 first2,
                     InputIter2 last2,
                     OutputIter result) {
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      *result = *first1;
      ++first1;
    } else if (*first1 > *first2) {
      *result = *first2;
      ++first2;
    } else {
      *result = *first1;
      ++first1;
      ++first2;
    }
    ++result;
  }
  return copy(first2, last2, copy(first1, last1, result));
}
// set_intersection 构造s1，s2的交集
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_intersection(InputIter1 first1,
                            InputIter1 last1,
                            InputIter2 first2,
                            InputIter2 last2,
                            OutputIter result) {
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      ++first1;
    } else if (*first1 > *first2) {
      ++first2;
    } else {
      *result = *first1;
      ++first1;
      ++first2;
      ++result;
    }
  }
  return result;
}

// set_difference 构造s1，s2差集
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_difference(InputIter1 first1,
                          InputIter1 last1,
                          InputIter2 first2,
                          InputIter2 last2,
                          OutputIter result) {
  while (first1 != last1 && first2 != last2)
    if (*first1 < *first2) {
      *result = *first1;
      ++first1;
      ++result;
    } else if (*first1 > *first2) {
      ++first2;
    } else {
      ++first1;
      ++first2;
    }
  return copy(first1, last1, result);
}

// set_symmetric_difference 构造 s1，s2之对称差集
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_symmetric_difference(InputIter1 first1,
                                    InputIter1 last1,
                                    InputIter2 first2,
                                    InputIter2 last2,
                                    OutputIter result) {
  while (first1 != last1 && first2 != last2)
    if (*first1 < *first2) {
      *result = *first1;
      ++first1;
      ++result;
    } else if (*first1 > *first2) {
      *result = *first2;
      ++first2;
    } else {
      ++first1;
      ++first2;
    }
  return copy(first2, last2, copy(first1, last1, result));
}

_MINISTL_END

#endif