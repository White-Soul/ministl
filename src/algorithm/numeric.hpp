#ifndef MINISTL_NUMERIC_H
#define MINISTL_NUMERIC_H
#include "../functor/functor.hpp"
#include "../iterator/iterator.hpp"
#include "../utils/util.hpp"

// 数值算法
_MINISTL_BEGIN
// accumulate 计算init和[first,last)内的元素总和
// 版本1
template <class InputIter, class T>
T accumulate(InputIter first, InputIter last, T init) {
  for (; first != last; ++first)
    init = init + *first;
  return init;
}
// 版本2
template <class InputIter, class T, class BinaryOperation>
T accumulate(InputIter first,
             InputIter last,
             T init,
             BinaryOperation binary_op) {
  for (; first != last; ++first)
    init = binary_op(init, *first);  // 执行二元操作
  return init;
}

// adjacent_difference 用来计算[first,last)中相邻的差
// 版本1
template <class InputIter, class OutputIter>
OutputIter adjacent_difference(InputIter first,
                               InputIter last,
                               OutputIter result) {
  typedef typename iterator_traits<InputIter>::value_type value_type;
  if (first == last)
    return result;
  *result = *first;
  value_type value = *first;
  while (++first != last) {
    value_type tmp = *first;
    *++result = tmp - value;
    value = tmp;
  }
  return ++result;
}
// 版本2
template <class InputIter, class OutputIter, class BinaryOperation>
OutputIter adjacent_difference(InputIter first,
                               InputIter last,
                               OutputIter result,
                               BinaryOperation binary_op) {
  typedef typename iterator_traits<InputIter>::value_type value_type;
  if (first == last)
    return result;
  *result = *first;
  value_type value = *first;
  while (++first != last) {
    value_type tmp = *first;
    *++result = binary_op(tmp, value);
    value = tmp;
  }
  return ++result;
}

// inner_product
// 计算[first1,last1) 和 [first2, first2 + (lsat1-first1)]的一般内积
// 版本1
template <class InputIter1, class InputIter2, class T>
T inner_product(InputIter1 first1,
                InputIter1 last1,
                InputIter2 first2,
                T init) {
  for (; first1 != last1; ++first1, ++first2)
    init = init + (*first1 * *first2);
  return init;
}
// 版本2
template <class InputIter1,
          class InputIter2,
          class T,
          class BinaryOperation1,
          class BinaryOperation2>
T inner_product(InputIter1 first1,
                InputIter1 last1,
                InputIter2 first2,
                T init,
                BinaryOperation1 binary_op1,
                BinaryOperation2 binary_op2) {
  for (; first1 != last1; ++first1, ++first2)
    init = binary_op1(init, binary_op2(*first1, *first2));
  return init;
}

// partial_sum 计算局部和，
// 版本1
template <class InputIter, class OutputIter>
OutputIter partial_sum(InputIter first, InputIter last, OutputIter result) {
  typedef typename iterator_traits<InputIter>::value_type value_type;
  if (first == last)
    return result;
  *result = *first;
  value_type value = *first;
  while (++first != last) {
    value = value + *first;
    *++result = value;
  }
  return ++result;
}
// 版本2
template <class InputIter, class OutputIter, class BinaryOperation>
OutputIter partial_sum(InputIter first,
                       InputIter last,
                       OutputIter result,
                       BinaryOperation binary_op) {
  typedef typename iterator_traits<InputIter>::value_type value_type;
  if (first == last)
    return result;
  *result = *first;
  value_type value = *first;
  while (++first != last) {
    value = binary_op(value, *first);
    *++result = value;
  }
  return ++result;
}

// power
// 版本1 乘幂
template <class T, class Integer>
inline T power(T x, Integer n) {
  return power(x, n, multiplies<T>());
}
// 版本2 幂次方
// MonoidOperation 必须满足结合律
template <class T, class Integer, class MonoidOperation>
T power(T x, Integer n, MonoidOperation op) {
  if (n == 0)
    return identity_element(op);
  else {
    while ((n & 1) == 0) {
      n >>= 1;
      x = op(x, x);
    }
    T result = x;
    n >>= 1;
    while (n != 0) {
      x = op(x, x);
      if ((n & 1) != 0)
        result = op(result, x);
      n >>= 1;
    }
    return result;
  }
}

// itoa 设定某区间的内容，使其内的每一个元素从value开始呈现递增状态
template <class ForwardIter, class T>
void iota(ForwardIter first, ForwardIter last, T value) {
  while (first != last)
    *first++ = value++;
}

_MINISTL_END

#endif