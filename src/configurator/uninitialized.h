#ifndef MINISTL_UNINITIALIZED_H
#define MINISTL_UNINITIALIZED_H
#include "../iterator/iterator.hpp"
#include "../utils/util.hpp"

_MINISTL_BEGIN

// uninitialized_fill_n

template <class ForwardIter, class Size, class T>
inline ForwardIter _uninitialized_fill_n_aux(ForwardIter first,
                                             Size n,
                                             const T& x,
                                             _true_type) {
  return fill_n(first, n, x);
}

template <class ForwardIter, class Size, class T, class T1>
inline ForwardIter _uninitialized_fill_n(ForwardIter first,
                                         Size n,
                                         const T& x,
                                         T1*) {
  typedef typename _type_traits<T1>::is_POD_type is_POD;
  return _uninitialized_fill_n_aux(first, n, x, is_POD());
}

template <class ForwardIter, class Size, class T, class T1>
inline ForwardIter _uninitialized_fill_n(ForwardIter first,
                                         Size n,
                                         const T& x,
                                         _false_type) {
  ForwardIter cur = first;
  for (; n > 0; --n, ++cur)
    construct(&*cur, x);
  return cur;
}
template <class ForwardIter, class Size, class T>
inline ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& x) {
  return _uninitialized_fill_n(first, n, x, value_type(first));
}
// uninitialized_copy
template <class InputIter, class ForwardIter>
inline ForwardIter _uninitialized_copy_aux(InputIter first,
                                           InputIter last,
                                           ForwardIter result,
                                           _true_type) {
  return std::copy(first, last, result);
}
template <class InputIter, class ForwardIter>
inline ForwardIter _uninitialized_copy_aux(InputIter first,
                                           InputIter last,
                                           ForwardIter result,
                                           _false_type) {
  ForwardIter cur = result;
  for (; first != last; ++first, ++cur) {
    construct(&*cur, *first);
  }
  return cur;
}

template <class InputIter, class ForwardIter, class T>
inline ForwardIter _uninitialized_copy(InputIter first,
                                       InputIter last,
                                       ForwardIter result,
                                       T*) {
  typedef typename _type_traits<T>::is_POD_type is_POD;
  return _uninitialized_copy_aux(first, last, result, is_POD());
}
template <class InputIter, class ForwardIter>
inline ForwardIter uninitialized_copy(InputIter first,
                                      InputIter last,
                                      ForwardIter result) {
  return _uninitialized_copy(first, last, result, value_type(result));
}
// uninitialized_fill
template <class ForwardIter, class T>
inline void _uninitialized_fill_aux(ForwardIter first,
                                    ForwardIter last,
                                    const T& x,
                                    _true_type) {
  std::fill(first, last, x);
}

template <class ForwardIter, class T>
void _uninitialized_fill_aux(ForwardIter first,
                             ForwardIter last,
                             const T& x,
                             _false_type) {
  ForwardIter cur = first;
  for (; cur != last; cur++) {
    construct(&*cur, x);
  }
}

template <class ForwardIter, class T, class T1>
inline void _uninitialized_fill(ForwardIter first,
                                ForwardIter last,
                                const T& x,
                                T1*) {
  typedef typename _type_traits<T1>::is_POD_type is_POD;
  _uninitialized_fill_aux(first, last, x, is_POD());
}

template <class ForwardIter, class T>
inline void uninitialized_fill(ForwardIter first,
                               ForwardIter last,
                               const T& x) {
  _uninitialized_fill(first, last, x, value_type(first));
}

/*****************************************************************************************/
// uninitialized_move
// 把[first, last)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
/*****************************************************************************************/
template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_move(InputIter first,
                                  InputIter last,
                                  ForwardIter result,
                                  _true_type) {
  return std::move(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_move(InputIter first,
                                  InputIter last,
                                  ForwardIter result,
                                  _false_type) {
  ForwardIter cur = result;
  try {
    for (; first != last; ++first, ++cur) {
      construct(&*cur, std::move(*first));
    }
  } catch (...) {
    destroy(result, cur);
  }
  return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_move(InputIter first,
                               InputIter last,
                               ForwardIter result) {
  typedef typename _type_traits<InputIter>::is_POD_type is_POD;
  return unchecked_uninit_move(first, last, result, is_POD());
}

/*****************************************************************************************/
// uninitialized_move_n
// 把[first, first + n)上的内容移动到以 result
// 为起始处的空间，返回移动结束的位置
/*****************************************************************************************/
template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_move_n(InputIter first,
                                    Size n,
                                    ForwardIter result,
                                    _true_type) {
  return std::move(first, first + n, result);
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_move_n(InputIter first,
                                    Size n,
                                    ForwardIter result,
                                    _false_type) {
  auto cur = result;
  try {
    for (; n > 0; --n, ++first, ++cur) {
      construct(&*cur, std::move(*first));
    }
  } catch (...) {
    for (; result != cur; ++result)
      destroy(&*result);
    throw;
  }
  return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter uninitialized_move_n(InputIter first, Size n, ForwardIter result) {
  typedef typename _type_traits<InputIter>::is_POD_type is_POD;
  return unchecked_uninit_move_n(first, n, result, is_POD());
}

_MINISTL_END

#endif