#ifndef MINISTL_ALGOBASE_H
#define MINISTL_ALGOBASE_H
#include "../iterator/iterator.hpp"
#include "../utils/util.hpp"

// ========================================== //
//                  基本算法                   //
// ========================================== //

_MINISTL_BEGIN

// equal
// 如果两个序列在[first,last)区间内相等，equal返回true，
// 如果第二序列的元素较多，则不予考虑
// 版本1
template <class InputIter1, class InputIter2>
inline bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2) {
  for (; first1 != last1; ++first1, ++first2) {
    if (*first1 != *first2)
      return false;
  }
  return true;
}
// 版本2
template <class InputIter1, class InputIter2, class BinaryPredicate>
inline bool equal(InputIter1 first1,
                  InputIter1 last1,
                  InputIter2 first2,
                  BinaryPredicate binary_op) {
  for (; first1 != last1; ++first1, ++first2) {
    if (binary_op(*first1, *first2))
      return false;
  }
  return true;
}

// fill
// 将[first, last)内的所有元素值改为新值
template <class ForwardIter, class T>
void fill(ForwardIter first, ForwardIter last, const T& value) {
  for (; first != last; ++first)
    *first = value;
}

// fill_n
// 将[first, last)内的所有元素值改为新值,
// 返回的迭代器指向被填入元素的最后一个元素的下一个位置
template <class OutputIter, class Size, class T>
OutputIter fill_n(OutputIter first, Size n, const T& value) {
  for (; n > 0; --n, ++first)
    *first = value;
  return first;
}

// iter_swap
// 将两个 ForwardIter 所指对象对调
template <class ForwardIter1, class ForwardIter2>
inline void iter_swap(ForwardIter1 a, ForwardIter2 b) {
  typedef typename iterator_traits<ForwardIter1>::value_type value_type;
  value_type tmp = *a;
  *a = *b;
  *b = tmp;
}

// lexicographical_compare
// 以字典序对两个序列进行比较，比较操作针对两序列的对应位置元素
// 版本1
template <class InputIter1, class InputIter2>
bool lexicographical_compare(InputIter1 first1,
                             InputIter1 last1,
                             InputIter2 first2,
                             InputIter2 last2) {
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (*first1 < *first2)
      return true;
    if (*first2 < *first1)
      return false;
  }
  return first1 == last1 && first2 != last2;
}
// 版本2
template <class InputIter1, class InputIter2, class Compare>
bool lexicographical_compare(InputIter1 first1,
                             InputIter1 last1,
                             InputIter2 first2,
                             InputIter2 last2,
                             Compare comp) {
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (comp(*first1, *first2))
      return true;
    if (comp(*first2, *first1))
      return false;
  }
  return first1 == last1 && first2 != last2;
}

// max 取对象中的较大值
// 版本1
template <class T>
inline const T& max(const T& a, const T& b) {
  return a < b ? b : a;
}
// 版本2
template <class T, class Compare>
inline const T& max(const T& a, const T& b, Compare comp) {
  return comp(a, b) ? b : a;
}

// min 取对象中的较小值
// 版本1
template <class T>
inline const T& min(const T& a, const T& b) {
  return b < a ? b : a;
}
// 版本2
template <class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp) {
  return comp(b, a) ? b : a;
}
// 特化版本 const unsigned char*
inline bool lexicographical_compare(const unsigned char* first1,
                                    const unsigned char* last1,
                                    const unsigned char* first2,
                                    const unsigned char* last2) {
  const size_t len1 = last1 - first1;
  const size_t len2 = last2 - first2;
  const int result = memcmp(first1, first2, min(len1, len2));
  return result != 0 ? result < 0 : len1 < len2;
}
// const char*
inline bool lexicographical_compare(const char* first1,
                                    const char* last1,
                                    const char* first2,
                                    const char* last2) {
  const size_t len1 = last1 - first1;
  const size_t len2 = last2 - first2;
  const int result = memcmp(first1, first2, min(len1, len2));
  return result != 0 ? result < 0 : len1 < len2;
}

// mismatch
// 平行比较两个序列，指出两者之间的第一个不匹配点 返回一对迭代器
// 版本1
template <class InputIter1, class InputIter2>
pair<InputIter1, InputIter2> mismatch(InputIter1 first1,
                                      InputIter1 last1,
                                      InputIter2 first2) {
  while (first1 != last1 && *first1 == *first2) {
    ++first1;
    ++first2;
  }
  return pair<InputIter1, InputIter2>(first1, first2);
}
// 版本2
template <class InputIter1, class InputIter2, class BinaryPredicate>
pair<InputIter1, InputIter2> mismatch(InputIter1 first1,
                                      InputIter1 last1,
                                      InputIter2 first2,
                                      BinaryPredicate binary_pred) {
  while (first1 != last1 && binary_pred(*first1, *first2)) {
    ++first1;
    ++first2;
  }
  return pair<InputIter1, InputIter2>(first1, first2);
}

// swap 用来交换两个对象的内容
template <class T>
inline void swap(T& a, T& b) {
  T tmp = a;
  a = b;
  b = tmp;
}

// _copy_dispatch 3个版本
// 完全泛化版本
template <class InputIter, class OutputIter>
struct _copy_dispatch {
  OutputIter operator()(InputIter first, InputIter last, OutputIter result) {
    return _copy(first, last, result, iterator_category(first));
  }
};
// 偏特化版本1
template <class T>
struct _copy_dispatch<T*, T*> {
  T* operator()(T* first, T* last, T* result) {
    typedef typename _type_traits<T>::has_trivial_assignment_operator t;
    return _copy_t(first, last, result, t());
  }
};
// 偏特化版本2
template <class T>
struct _copy_dispatch<const T*, T*> {
  T* operator()(const T* first, const T* last, T* result) {
    typedef typename _type_traits<T>::has_trivial_assignment_operator t;
    return _copy_t(first, last, result, t());
  }
};
// copy
// 将输入区间内的元素复制到输出区间
// 完全泛化版本
template <class InputIter, class OutputIter>
inline OutputIter copy(InputIter first, InputIter last, OutputIter result) {
  return _copy_dispatch<InputIter, OutputIter>()(first, last, result);
}
// 特殊版本1
inline char* copy(const char* first, const char* last, char* result) {
  memmove(result, first, last - first);
  return result + (last - first);
}
// 特殊版本2
inline wchar_t* copy(const wchar_t* first,
                     const wchar_t* last,
                     wchar_t* result) {
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return result + (last - first);
}
//_copy
// InputIter版
template <class InputIter, class OutputIter>
inline OutputIter _copy(InputIter first,
                        InputIter last,
                        OutputIter result,
                        input_iterator_tag) {
  for (; first != last; ++result, ++first)
    *result = *first;
  return result;
}
// RandomAccessIter 版
template <class RandomAccessIter, class OutputIter>
inline OutputIter _copy(RandomAccessIter first,
                        RandomAccessIter last,
                        OutputIter result,
                        random_access_iterator_tag) {
  return _copy_d(first, last, result, distance_type(first));
}
template <class RandomAccessIter, class OutputIter, class Distance>
inline OutputIter _copy_d(RandomAccessIter first,
                          RandomAccessIter last,
                          OutputIter result,
                          Distance*) {
  for (Distance n = last - first; n > 0; --n, ++result, ++first)
    *result = *first;
  return result;
}
// _copy_t
// 适用于平凡的赋值运算符
template <class T>
inline T* _copy_t(const T* first, const T* last, T* result, _true_type) {
  memmove(result, first, sizeof(T) * (last - first));
  return result + (last - first);
}
// 适用于不平凡的赋值运算符
template <class T>
inline T* _copy_t(const T* first, const T* last, T* result, _false_type) {
  return _copy_d(first, last, result, (ptrdiff_t*)0);
}

// _copy_backward_dispatch
template <class BidirectionalIter1, class BidirectionalIter2>
struct _copy_backward_dispatch {
  BidirectionalIter2 operator()(BidirectionalIter1 first,
                                BidirectionalIter1 last,
                                BidirectionalIter2 result) {
    return _copy_backward(first, last, result, iterator_category(first));
  }
};
// 偏特化版本1
template <class T>
struct _copy_backward_dispatch<T*, T*> {
  T* operator()(T* first, T* last, T* result) {
    typedef typename _type_traits<T>::has_trivial_assignment_operator t;
    return _copy_backward_t(first, last, result, t());
  }
};
// 偏特化版本2
template <class T>
struct _copy_backward_dispatch<const T*, T*> {
  T* operator()(const T* first, const T* last, T* result) {
    typedef typename _type_traits<T>::has_trivial_assignment_operator t;
    return _copy_backward_t(first, last, result, t());
  }
};
// copy_backward
template <class BidirectionalIter1, class BidirectionalIter2>
inline BidirectionalIter2 copy_backward(BidirectionalIter1 first,
                                        BidirectionalIter1 last,
                                        BidirectionalIter2 result) {
  return _copy_backward_dispatch<BidirectionalIter1, BidirectionalIter2>()(
      first, last, result);
}
// bidirectional_iterator_tag
template <class BidirectionalIter1, class BidirectionalIter2>
inline BidirectionalIter2 _copy_backward(BidirectionalIter1 first,
                                         BidirectionalIter1 last,
                                         BidirectionalIter2 result,
                                         bidirectional_iterator_tag) {
  for (; first != last;)
    *--result = *--last;
  return result;
}
// random_access_iterator_tag
template <class BidirectionalIter1, class BidirectionalIter2>
inline BidirectionalIter2 _copy_backward(BidirectionalIter1 first,
                                         BidirectionalIter1 last,
                                         BidirectionalIter2 result,
                                         random_access_iterator_tag) {
  return _copy_backward_d(first, last, result, distance_type(first));
}
template <class RandomAccessIter, class OutputIter, class Distance>
inline OutputIter _copy_backward_d(RandomAccessIter first,
                                   RandomAccessIter last,
                                   OutputIter result,
                                   Distance*) {
  for (Distance n = last - first; n > 0; --n)
    *--result = *--last;
  return result;
}
//_copy_backward_t
// 适用于平凡的赋值运算符
template <class T>
inline T* _copy_backward_t(const T* first,
                           const T* last,
                           T* result,
                           _true_type) {
  memmove(result - (last - first), first, sizeof(T) * (last - first));
  return result - (last - first);
}
// 适用于不平凡的赋值运算符
template <class T>
inline T* _copy_backward_t(const T* first,
                           const T* last,
                           T* result,
                           _false_type) {
  return _copy_backward_d(first, last, result, (ptrdiff_t*)0);
}

_MINISTL_END

#endif