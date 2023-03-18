#ifndef MINISTL_ALGO_H
#define MINISTL_ALGO_H
#include "../configurator/memory.hpp"
#include "../utils/util.hpp"
#include "algobase.hpp"
#include "heap_algo.hpp"
#include "set_algo.hpp"

_MINISTL_BEGIN

// find 查找区间内的所有元素，找出第一个匹配等同条件者，返回其迭代器
// 否则返回last
template <class Iterator, class T>
Iterator find(Iterator begin, Iterator end, const T& value) {
  while (begin != end && *begin != value)
    ++begin;
  return begin;
}
// find_if 指定一个仿函数，实施于区间上，并返回第一个使仿函数为true的元素迭代器
template <class Iterator, class Predicate>
Iterator find_if(Iterator begin, Iterator end, Predicate pred) {
  while (begin != end && pred(*begin))
    ++begin;
  return begin;
}
// find_end 在序列1所在的区间中，查找序列二最后一次出现点，
// 若不存在完全匹配序列2的子序列，返回last1迭代器
// 版本1
template <class ForwardIter1, class ForwardIter2>
inline ForwardIter1 find_end(ForwardIter1 first1,
                             ForwardIter1 last1,
                             ForwardIter2 first2,
                             ForwardIter2 last2) {
  typedef typename iterator_traits<ForwardIter1>::iterator_category category1;
  typedef typename iterator_traits<ForwardIter2>::iterator_category category2;

  return _find_end(first1, last1, first2, last2, category1(), category2());
}
// _find_end
// forward iterator版
template <class ForwardIter1, class ForwardIter2>
ForwardIter1 _find_end(ForwardIter1 first1,
                       ForwardIter1 last1,
                       ForwardIter2 first2,
                       ForwardIter2 last2,
                       forward_iterator_tag,
                       forward_iterator_tag) {
  if (first2 == last2)
    return last1;
  else {
    ForwardIter1 result = last1;
    while (1) {
      ForwardIter1 new_result = search(first1, last1, first2, last2);
      if (new_result == last1)
        return result;
      else {
        result = new_result;
        first1 = new_result;
        ++first1;
      }
    }
  }
}
// bidirectional iterator版
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter1 _find_end(BidirectionalIter1 first1,
                             BidirectionalIter1 last1,
                             BidirectionalIter2 first2,
                             BidirectionalIter2 last2,
                             bidirectional_iterator_tag,
                             bidirectional_iterator_tag) {
  typedef reverse_iterator<BidirectionalIter1> reverse1;
  typedef reverse_iterator<BidirectionalIter2> reverse2;

  reverse1 rlast1(first1);
  reverse2 rlast2(first2);
  reverse1 rresult = serach(reverse1(last1), rlast1, reverse2(last2), rlast2);

  if (rresult == rlast1)
    return last1;
  else {
    BidirectionalIter1 result = rresult.base();
    advance(result, -distance(first2, last2));
    return result;
  }
}
// find_first_of
// 以区间内的某些元素为目标，找到他们在区间内的第一次出现的地点
// 版本1
template <class InputIter, class ForwardIter>
InputIter find_first_of(InputIter first1,
                        InputIter last1,
                        ForwardIter first2,
                        ForwardIter last2) {
  for (; first1 != last1; ++first1)
    for (ForwardIter iter = first2; iter != last2; ++iter)
      if (*first1 == *iter)
        return first1;
  return last1;
}
// 版本2
template <class InputIter, class ForwardIter, class BinaryPredicate>
InputIter find_first_of(InputIter first1,
                        InputIter last1,
                        ForwardIter first2,
                        ForwardIter last2,
                        BinaryPredicate comp) {
  for (; first1 != last1; ++first1)
    for (ForwardIter iter = first2; iter != last2; ++iter)
      if (comp(*first1, *iter))
        return first1;
  return last1;
}

// adjacent_find 查找满足条件的相邻元素
// 版本1
template <class ForwardIter>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last) {
  if (first == last)
    return last;
  ForwardIter next = first;
  while (++next != last) {
    if (*first == *next)
      return first;
    first = next;
  }
  return last;
}
// 版本2
template <class ForwardIter, class BinaryPredicate>
ForwardIter adjacent_find(ForwardIter first,
                          ForwardIter last,
                          BinaryPredicate binary_pred) {
  if (first == last)
    return last;
  ForwardIter next = first;
  while (++next != last) {
    if (binary_pred(*first, *next))
      return first;
    first = next;
  }
  return last;
}

// count 将区间内的每一个元素与value比较，返回与value相等的元素个数
template <class InputIter, class T>
typename iterator_traits<InputIter>::difference_type count(InputIter first,
                                                           InputIter last,
                                                           const T& value) {
  typename iterator_traits<InputIter>::difference_type n = 0;
  for (; first != last; ++first)
    if (*first == value)
      ++n;
  return n;
}
// count_if 指定一个仿函数，实施于区间上，并返回使仿函数为true的元素个数
template <class InputIter, class Predicate>
typename iterator_traits<InputIter>::difference_type count_if(InputIter first,
                                                              InputIter last,
                                                              Predicate pred) {
  typename iterator_traits<InputIter>::difference_type n = 0;
  for (; first != last; ++first)
    if (pred(*first))
      ++n;
  return n;
}

// for_each
// 将仿函数func施行于区间的每一个元素上， func不可以改变元素内容
template <class InputIter, class Function>
Function for_each(InputIter first, InputIter last, Function func) {
  for (; first != last; ++first)
    func(*first);
  return func;
}

// generate
// 将仿函数gen的运算结果填入区间内的所有元素
template <class ForwardIter, class Generator>
void generate(ForwardIter first, ForwardIter last, Generator gen) {
  for (; first != last; ++first)
    *first = gen();
}
// generate_n
// 将仿函数gen的运算结果填入从迭代器first开始的n个元素
template <class OutputIter, class Size, class Generator>
OutputIter generate_n(OutputIter first, Size n, Generator gen) {
  for (; n > 0; n--, ++first)
    *first = gen();
  return first;
}

// includes (应用于有序区间)
// 判断序列2是否 涵盖于 序列1
// 版本1 判断区间二的每个元素是否都存在与区间1
template <class InputIter1, class InputIter2>
bool includes(InputIter1 first1,
              InputIter1 last1,
              InputIter2 first2,
              InputIter2 last2) {
  while (first1 != last1 && first2 != last2) {
    if (*first2 < *first1)  // 二元素少于一元素
      return false;
    else if (*first1 < *first2)
      ++first1;
    else
      ++first1, ++first2;
  }
  return first2 == last2;  // 有一个走完了
}
// 版本2
// 判断序列1内是否有子序列，其与序列2对应每个元素满足二元运算
template <class InputIter1, class InputIter2, class Compare>
bool includes(InputIter1 first1,
              InputIter1 last1,
              InputIter2 first2,
              InputIter2 last2,
              Compare comp) {
  while (first1 != last1 && first2 != last2) {
    if (*first2 < *first1)  // 二元素少于一元素
      return false;
    else if (comp(*first1, *first2))
      ++first1;
    else
      ++first1, ++first2;
  }
  return first2 == last2;  // 有一个走完了
}

// max_element 返回一个迭代器，指向序列中数值最大的元素
// 版本1
template <class ForwardIter>
ForwardIter max_element(ForwardIter first, ForwardIter last) {
  if (first == last)
    return first;
  ForwardIter result = first;
  while (++first != last)
    if (*result < *first)
      result = first;
  return result;
}
// 版本2
template <class ForwardIter, class Compare>
ForwardIter max_element(ForwardIter first, ForwardIter last, Compare comp) {
  if (first == last)
    return first;
  ForwardIter result = first;
  while (++first != last)
    if (comp(*result, *first))
      result = first;
  return result;
}

// merge(应用于有序区间)
// 将两个经过排序的集合s1和s2，合并起来置于另一个空间，所得结果也是有序的
// 版本1
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter merge(InputIter1 first1,
                 InputIter1 last1,
                 InputIter2 first2,
                 InputIter2 last2,
                 OutputIter result) {
  while (first1 != last1 && first2 != last2) {
    if (*first2 < *first1) {
      *result = *first2;
      ++first2;
    } else {
      *result = *first1;
      ++first1;
    }
    ++result;
  }
  return copy(first2, last2, copy(first1, last1, result));
}
// 版本2
template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter merge(InputIter1 first1,
                 InputIter1 last1,
                 InputIter2 first2,
                 InputIter2 last2,
                 OutputIter result,
                 Compare comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first2, *first1)) {
      *result = *first2;
      ++first2;
    } else {
      *result = *first1;
      ++first1;
    }
    ++result;
  }
  return copy(first2, last2, copy(first1, last1, result));
}

// min_element
// 返回一个指向序列之中数值最小元素的迭代器
// 版本1
template <class ForwardIter>
ForwardIter min_element(ForwardIter first, ForwardIter last) {
  if (first == last)
    return first;
  ForwardIter result = first;
  while (++first != last)
    if (*first < *result)
      result = first;
  return result;
}
// 版本2
template <class ForwardIter, class Compare>
ForwardIter min_element(ForwardIter first, ForwardIter last, Compare comp) {
  if (first == last)
    return first;
  ForwardIter result = first;
  while (++first != last)
    if (comp(*first, *result))
      result = first;
  return result;
}

// partition
// 将区间中的元素重新排列，所有被一元条件运算pred判定为true的元素都会被放在区间前端
// 这个算法不保证保留元素相对位置
template <class BidirectionalIter, class Predicate>
BidirectionalIter partition(BidirectionalIter first,
                            BidirectionalIter last,
                            Predicate pred) {
  while (true) {
    while (true)
      if (first == last)
        return first;
      else if (pred(*first))
        ++first;
      else
        break;
    --last;
    while (true)
      if (first == last)
        return first;
      else if (!pred(*last))
        --last;
      else
        break;
    iter_swap(first, last);
    ++first;
  }
}

// remove 移除（但不删除）
// 移除区间中所有与value 相等的元素
template <class ForwardIter, class T>
ForwardIter remove(ForwardIter first, ForwardIter last, const T& value) {
  first = find(first, last, value);
  ForwardIter next = first;
  return first == last ? first : remove_copy(++next, last, first, value);
}
// remove_copy
// 移除区间中所有与value
// 相等的元素，它并不删除(原容器并不改变)，而是将结果复制到一个以result标识起始位置的容器上
template <class InputIter, class OutputIter, class T>
OutputIter remove_copy(InputIter first,
                       InputIter last,
                       OutputIter result,
                       const T& value) {
  for (; first != last; ++first)
    if (*first != value) {
      *result = *first;
      ++result;
    }
  return result;
}
// remove_if
// 移除区间中所有被仿函数pred核定为true的元素,他并不删除元素
template <class ForwardIter, class Predicate>
ForwardIter remove_if(ForwardIter first, ForwardIter last, Predicate pred) {
  first = find_if(first, last, pred);
  ForwardIter next = first;
  return first == last ? first : remove_copy_if(++next, last, first, pred);
}
// remove_copy_if
// 移除区间中所有被仿函数pred核定为true的元素,他并不删除元素
template <class InputIterator, class OutputIter, class Predicate>
OutputIter remove_copy_if(InputIterator first,
                          InputIterator last,
                          OutputIter result,
                          Predicate pred) {
  for (; first != last; ++first)
    if (!pred(*first)) {
      *result = *first;
      ++result;
    }
  return result;
}

/**
 * @brief replace 将区间内的所有 old_value 都以 new_value 取代
 * @param[in] first 区间头部
 * @param[in] last 区间尾部
 * @param[in] old_value 旧值
 * @param[in] new_value 新值
 *
 */
template <class ForwardIter, class T>
void replace(ForwardIter first,
             ForwardIter last,
             const T& old_value,
             const T& new_value) {
  for (; first != last; ++first)
    if (*first == old_value)
      *first = new_value;
}

/**
 * @brief replace_copy 将区间内的所有 old_value 都以 new_value
 * 取代。新序列会被复制到result
 * @param[in] first 区间头部
 * @param[in] last 区间尾部
 * @param[out] result 指向被复制的最后一个元素的下个位置
 * @param[in] old_value 旧值
 * @param[in] new_value 新值
 * @return 指向被复制的最后一个元素的下个位置
 */
template <class InputIter, class OutputIter, class T>
OutputIter replace_copy(InputIter first,
                        InputIter last,
                        OutputIter result,
                        const T& old_value,
                        const T& new_value) {
  for (; first != last; ++first)
    *result = *first == old_value ? new_value : *first;
  return result;
}

/**
 * @brief replace_if 将区间内的所有 被 pred 评估为 true 的元素用 new_value 取代.
 * @param[in] first 区间头部
 * @param[in] last 区间尾部
 * @param[in] pred 谓词
 * @param[in] new_value 新值
 */
template <class ForwardIter, class Predicate, class T>
void replace_if(ForwardIter first,
                ForwardIter last,
                Predicate pred,
                const T& new_value) {
  for (; first != last; ++first)
    if (pred(*first))
      *first = new_value;
}

/**
 * @brief replace_copy_if 将区间内的所有 被 pred 评估为 true 的元素用 new_value
 * 取代。新序列会被复制到result
 * @param[in] first 区间头部
 * @param[in] last 区间尾部
 * @param[out] result 指向被复制的最后一个元素的下个位置
 * @param[in] pred 谓词
 * @param[in] new_value 新值
 * @return 指向被复制的最后一个元素的下个位置
 */
template <class InputIter, class OutputIter, class Predicate, class T>
OutputIter replace_copy_if(InputIter first,
                           InputIter last,
                           OutputIter result,
                           Predicate pred,
                           const T& new_value) {
  for (; first != last; ++first)
    *result = pred(*first) ? new_value : *first;
  return result;
}

/**
 * @brief reverse 将区间的元素在原容器中颠倒重排
 * @param[in] first 区间头部
 * @param[in] last 区间尾部
 */
template <class BidirectionalIter>
inline void reverse(BidirectionalIter first, BidirectionalIter last) {
  _reverse(first, last, iterator_category(first));
}
template <class BidirectionalIter>
void _reverse(BidirectionalIter first,
              BidirectionalIter last,
              bidirectional_iterator_tag) {
  while (true)
    if (first == last || first == --last)
      return;
    else
      iter_swap(first++, last);
}
template <class RandomAccessIter>
void _reverse(RandomAccessIter first,
              RandomAccessIter last,
              random_access_iterator_tag) {
  while (first < last)
    iter_swap(first++, --last);
}

/**
 * @brief reverse_copy
 * 将区间的元素在颠倒重排，新序列会被放在以result指出的容器中。原序列没有改变
 * @param[in] first 区间头部
 * @param[in] last 区间尾部
 * @param[out] result 新产生的最后元素的下一位置
 * @return 新产生的最后元素的下一位置
 */
template <class BidirectionalIter, class OutputIter>
OutputIter reverse_copy(BidirectionalIter first,
                        BidirectionalIter last,
                        OutputIter result) {
  while (first != last) {
    --last;
    *result = *last;
    ++result;
  }
  return result;
}

/**
 * @brief rotate 将 [first, middle) 内的元素和 [middle, last) 内的互换
 * @param[in] first 区间头部
 * @param[in] middle 区间内任意位置
 * @param[in] last 区间尾部
 */
template <class ForwardIter>
inline void rotate(ForwardIter first, ForwardIter middle, ForwardIter last) {
  if (first == middle || middle == last)
    return;
  _rotate(first, middle, last, distance_type(first), iterator_category(first));
}
template <class ForwardIter, class Distance>
void _rotate(ForwardIter first,
             ForwardIter middle,
             ForwardIter last,
             Distance*,
             forward_iterator_tag) {
  for (ForwardIter i = middle;;) {
    iter_swap(first, i);
    ++first;
    ++i;
    if (first == middle) {
      if (i == last)
        return;
      middle = i;
    } else if (i == last)
      i = middle;
  }
}
template <class RandomAccessIter, class Distance>
void _rotate(RandomAccessIter first,
             RandomAccessIter middle,
             RandomAccessIter last,
             Distance*,
             random_access_iterator_tag) {
  Distance n = _gcd(last - first, middle - first);
  while (n--)
    _rotate_cycle(first, last, first + n, middle - first, value_type);
}
// 最大公因子
template <class EuclideanRingElement>
EuclideanRingElement _gcd(EuclideanRingElement m, EuclideanRingElement n) {
  while (n != 0) {
    EuclideanRingElement t = m % n;
    m = n;
    n = t;
  }
  return m;
}
template <class RandomAccessIter, class Distance, class T>
void _rotate_cycle(RandomAccessIter first,
                   RandomAccessIter last,
                   RandomAccessIter initial,
                   Distance shift,
                   T*) {
  T value = *initial;
  RandomAccessIter ptr1 = initial;
  RandomAccessIter ptr2 = ptr1 + shift;
  while (ptr2 != initial) {
    *ptr1 = *ptr2;
    ptr1 = ptr2;
    if (last - ptr2 > shift)
      ptr2 += shift;
    else
      ptr2 = first + (shift - (last - ptr2));
  }
  *ptr1 = value;
}
/**
 * @brief rotate_copy 将 [first, middle) 内的元素和 [middle, last)
 * 内的互换,但新序列会被置于 result 所指出的容器中。原序列没有改变
 * @param[in] first 区间头部
 * @param[in] middle 区间内任意位置
 * @param[in] last 区间尾部
 * @return 新产生的最后元素的下一位置
 */
template <class ForwardIter, class OutputIter>
OutputIter rotate_copy(ForwardIter first,
                       ForwardIter middle,
                       ForwardIter last,
                       OutputIter result) {
  return copy(first, middle, copy(middle, last, result));
}
/**
 * @brief search 在序列1 [first1, last1) 内查找 序列2[first2,
 * last2)的首次出现点，若不存在，则返回序列1的尾迭代器
 * @param[in] first1 序列1头部
 * @param[in] last1 序列1尾部
 * @param[in] first2 序列2头部
 * @param[in] last2 序列2尾部
 * @return 首次出现点对应的迭代器，或者序列1的尾迭代器
 */
template <class ForwardIter1, class ForwardIter2>
inline ForwardIter1 search(ForwardIter1 first1,
                           ForwardIter1 last1,
                           ForwardIter2 first2,
                           ForwardIter2 last2) {
  return _search(first1, last1, first2, last2, distance_type(first1),
                 distance_type(first2));
}
template <class ForwardIter1,
          class ForwardIter2,
          class Distance1,
          class Distance2>
ForwardIter1 _search(ForwardIter1 first1,
                     ForwardIter1 last1,
                     ForwardIter2 first2,
                     ForwardIter2 last2,
                     Distance1*,
                     Distance2*) {
  Distance1 d1 = 0;
  distance(first1, last1, d1);
  Distance2 d2 = 0;
  distance(first2, last2, d2);
  if (d1 < d2)
    return last1;
  ForwardIter1 cur1 = first1;
  ForwardIter1 cur2 = first2;
  while (cur2 != last2)
    if (*cur1 == *cur2) {
      ++cur1;
      ++cur2;
    } else {
      if (d1 == d2)
        return last1;
      else {
        cur1 = ++first1;
        cur2 = first2;
        --d1;
      }
    }
  return first1;
}
/**
 * @brief search_n 在序列 [first, last) 内查找
 * 连续count个符合条件的元素形成的子序列，返回一个指向该子序列起始处的迭代器，
 * 如果找不到则返回 序列 尾迭代器
 * @param[in] first 序列头部迭代器
 * @param[in] last 序列尾部迭代器
 * @param[in] count 整数
 * @param[in] value 元素
 * @return 指向该子序列起始处的迭代器，或者序列1的尾迭代器
 */
template <class ForwardIter, class Integer = size_t, class T>
ForwardIter search_n(ForwardIter first,
                     ForwardIter last,
                     Integer count,
                     const T& value) {
  if (count <= 0)
    return first;
  else {
    first = find(first, last, value);
    while (first != last) {
      Integer n = count - 1;
      ForwardIter i = first;
      ++i;
      while (i != last && n != 0 && *i == value) {
        ++i;
        --n;
      }
      if (n == 0)
        return first;
      else
        first = find(i, last, value);
    }
    return last;
  }
}
/**
 * @brief search_n 在序列 [first, last) 内查找
 * "连续count个元素皆满足指定条件"形成的子序列，返回一个指向该子序列起始处的迭代器，
 * 如果找不到则返回 序列 尾迭代器
 * @param[in] first 序列头部迭代器
 * @param[in] last 序列尾部迭代器
 * @param[in] count 整数
 * @param[in] value 元素
 * @param[in] binary_pred 指定条件
 * @return 指向该子序列起始处的迭代器，或者序列1的尾迭代器
 */
template <class ForwardIter,
          class Integer = size_t,
          class T,
          class BinaryPredicate>
ForwardIter search_n(ForwardIter first,
                     ForwardIter last,
                     Integer count,
                     const T& value,
                     BinaryPredicate binary_pred) {
  if (count <= 0)
    return first;
  else {
    while (first != last) {
      if (binary_pred(*first, value))
        break;
      ++first;
    }
    while (first != last) {
      Integer n = count - 1;
      ForwardIter i = first;
      ++i;
      while (i != last && n != 0 && binary_pred(*i, value)) {
        ++i;
        --n;
      }
      if (n == 0)
        return first;
      else {
        while (i != last) {
          if (binary_pred(*i, value))
            break;
          ++i;
        }
        first = i;
      }
    }
    return last;
  }
}

/**
 * @brief swap_ranges 将区间 [first1, last1) 内的元素和 "从 first2
 * 开始、个数相同"
 * 的元素互相交换，两个序列可以位于同一容器中（不能彼此重叠），也可不同,
 * @param[in] first1 区间1头部迭代器
 * @param[in] last1 区间1尾部迭代器
 * @param[in] first2 区间2头部迭代器
 * @return 第二序列中的最后一个被交换元素的下一位置
 */
template <class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_ranges(ForwardIter1 first1,
                         ForwardIter1 last1,
                         ForwardIter2 first2) {
  for (; first1 != last1; ++first1, ++first2)
    iter_swap(first1, first2);
  return first2;
}

/**
 * @brief transform 以仿函数 op 作用于 [first, last)
 * 中的每一个元素，并以其结果产生一个新序列
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[out] result 新区间头迭代器
 * @return 指向结果区间中的最后元素的下一位置
 */
template <class InputIter, class OutputIter, class UnaryOperation>
OutputIter transform(InputIter first,
                     InputIter last,
                     OutputIter result,
                     UnaryOperation op) {
  for (; first != last; ++first, ++result)
    *result = op(*first);
  return result;
}
/**
 * @brief transform 以仿函数 binary_op 作用于一双元素，其中一个元素来自 [first1,
 * last1)，另一个元素来自 “从 first2 开始的序列” 并以其结果产生一个新序列
 * @param[in] first1 区间1头部迭代器
 * @param[in] last1 区间1尾部迭代器
 * @param[in] first2 区间2头部迭代器
 * @param[out] result 新区间头迭代器
 * @return 指向结果区间中的最后元素的下一位置
 */
template <class InputIter1,
          class InputIter2,
          class OutputIter,
          class BinaryOperation>
OutputIter transform(InputIter1 first1,
                     InputIter1 last1,
                     InputIter2 first2,
                     OutputIter result,
                     BinaryOperation binary_op) {
  for (; first1 != last1; ++first1, ++first2, ++result)
    *result = binary_op(*first1, *first2);
  return result;
}
/**
 * @brief unique 移除相邻重复元素（只保留一个）。
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @return 指向新区间尾部迭代器
 */
template <class ForwardIter>
ForwardIter unique(ForwardIter first, ForwardIter last) {
  first = adjacent_find(first, last);
  return unique_copy(first, last, first);
}
/**
 * @brief unique_copy 移除相邻重复元素（只保留一个）,
 * 将结果复制到以 result 开头的区间上。
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[out] result 新区间头部迭代器
 * @return 指向新区间尾部迭代器
 */
template <class InputIter, class OutputIter>
inline OutputIter unique_copy(InputIter first,
                              InputIter last,
                              OutputIter result) {
  if (first == last)
    return result;
  return _unquie_copy(first, last, result, iterator_category(result));
}
template <class InputIter, class ForwardIter>
ForwardIter _unique_copy(InputIter first,
                         InputIter last,
                         ForwardIter result,
                         forward_iterator_tag) {
  *result = *first;
  while (++first != last)
    if (*result != *first)
      *++result = *first;
  return ++result;
}
template <class InputIter, class OutputIter>
OutputIter _unique_copy(InputIter first,
                        InputIter last,
                        OutputIter result,
                        output_iterator_tag) {
  return _unique_copy(first, last, result, value_type(first));
}
template <class InputIter, class OutputIter, class T>
OutputIter _unique_copy(InputIter first,
                        InputIter last,
                        OutputIter result,
                        T*) {
  T value = *first;
  *result = value;
  while (++first != last) {
    if (value != *first) {
      value = *first;
      *++result = value;
    }
  }
  return ++result;
}
/**
 * @brief lower_bound(应用于有序区间 升序)
 * 在已经排序的[first,last)区间上寻找元素 value
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[in] value 元素
 * @return 指向对应元素的迭代器，若没有则返回第一个“不小于value”的元素
 */
template <class ForwardIter, class T>
inline ForwardIter lower_bound(ForwardIter first,
                               ForwardIter last,
                               const T& value) {
  return _lower_bound(first, last, value, distance_type(first),
                      iterator_category(first));
}
template <class ForwardIter, class T, class Distance>
ForwardIter _lower_bound(ForwardIter first,
                         ForwardIter last,
                         const T& value,
                         Distance,
                         forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);
  Distance half;
  ForwardIter middle;
  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);
    if (*middle < value) {
      first = middle;
      ++first;
      len = len - half - 1;
    } else
      len = half;
  }
  return first;
}
template <class RandomAccessIter, class T, class Distance>
RandomAccessIter _lower_bound(RandomAccessIter first,
                              RandomAccessIter last,
                              const T& value,
                              Distance,
                              random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIter middle;
  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (*middle < value) {
      first = middle + 1;
      len = len - half - 1;
    } else
      len = half;
  }
  return first;
}
/**
 * @brief lower_bound(应用于有序区间 升序)
 * 在已经排序的[first,last)区间上寻找最远的迭代器 i，使得[first,i)中的每个迭代器
 * j 都满足 comp(*j, value) 为真。
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[in] value 元素
 * @param[in] comp 条件
 * @return 最远的迭代器 i
 */
template <class ForwardIter, class T, class Compare>
inline ForwardIter lower_bound(ForwardIter first,
                               ForwardIter last,
                               const T& value,
                               Compare comp) {
  return _lower_bound(first, last, value, comp, distance_type(first),
                      iterator_category(first));
}
template <class ForwardIter, class T, class Distance, class Compare>
ForwardIter _lower_bound(ForwardIter first,
                         ForwardIter last,
                         const T& value,
                         Compare comp,
                         Distance,
                         forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);
  Distance half;
  ForwardIter middle;
  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);
    if (comp(*middle, value)) {
      first = middle;
      ++first;
      len = len - half - 1;
    } else
      len = half;
  }
  return first;
}
template <class RandomAccessIter, class T, class Distance, class Compare>
RandomAccessIter _lower_bound(RandomAccessIter first,
                              RandomAccessIter last,
                              const T& value,
                              Compare comp,
                              Distance,
                              random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIter middle;
  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (comp(*middle, value)) {
      first = middle + 1;
      len = len - half - 1;
    } else
      len = half;
  }
  return first;
}
/**
 * @brief upper_bound (应用于有序区间 升序)
 * 在已经排序的[first,last)区间上寻找value,
 * 返回不破坏顺序的情况下，可插入value的最后一个合适位置
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[in] value 元素
 * @return 可插入value的最后一个合适位置
 */
template <class ForwardIter, class T>
inline ForwardIter upper_bound(ForwardIter first,
                               ForwardIter last,
                               const T& value) {
  return _upper_bound(first, last, value, distance_type(first),
                      iterator_category(first));
}
template <class ForwardIter, class T, class Distance>
ForwardIter _upper_bound(ForwardIter first,
                         ForwardIter last,
                         const T& value,
                         Distance*,
                         forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);
  Distance half;
  ForwardIter middle;
  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);
    if (value < *middle) {
      len = half;
    } else {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}
template <class RandomAccessIter, class T, class Distance>
RandomAccessIter _upper_bound(RandomAccessIter first,
                              RandomAccessIter last,
                              const T& value,
                              Distance,
                              random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIter middle;
  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (value < *middle) {
      len = half;
    } else {
      first = middle + 1;
      len = len - half - 1;
    }
  }
  return first;
}
/**
 * @brief upper_bound (应用于有序区间 升序)
 * 在已经排序的[first,last)区间上寻找最远的迭代器
 * i，使得[first,i)中的每个迭代器 j 都满足 comp(value, *j) 不为真。
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[in] value 元素
 * @param[in] comp 条件
 * @return 最远的迭代器 i
 */
template <class ForwardIter, class T, class Compare>
inline ForwardIter upper_bound(ForwardIter first,
                               ForwardIter last,
                               const T& value,
                               Compare comp) {
  return _upper_bound(first, last, value, comp, distance_type(first),
                      iterator_category(first));
}
template <class ForwardIter, class T, class Distance, class Compare>
ForwardIter _upper_bound(ForwardIter first,
                         ForwardIter last,
                         const T& value,
                         Compare comp,
                         Distance*,
                         forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);
  Distance half;
  ForwardIter middle;
  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);
    if (comp(value, *middle)) {
      len = half;
    } else {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}
template <class RandomAccessIter, class T, class Distance, class Compare>
RandomAccessIter _upper_bound(RandomAccessIter first,
                              RandomAccessIter last,
                              const T& value,
                              Compare comp,
                              Distance,
                              random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIter middle;
  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (comp(value, *middle)) {
      len = half;
    } else {
      first = middle + 1;
      len = len - half - 1;
    }
  }
  return first;
}
/**
 * @brief binary_search (应用于有序区间 升序)
 * 在已经排序的[first,last)区间上寻找元素value
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[in] value 元素
 * @return 存在返回 true，否则返回 false
 */
template <class ForwardIter, class T>
bool binary_search(ForwardIter first, ForwardIter last, const T& value) {
  ForwardIter i = lower_bound(first, last, value);
  return i != last && !(value < *i);
}

/**
 * @brief upper_bound (应用于有序区间 升序)
 * 在已经排序的[first,last)区间上寻找一个迭代器
 * 当且仅当 i 使得 comp(*i, value) 和 comp(value, *i) 都不为真
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[in] value 元素
 * @param[in] comp 条件
 * @return 存在则返回 true 否则返回 false
 */
template <class ForwardIter, class T, class Compare>
bool binary_search(ForwardIter first,
                   ForwardIter last,
                   const T& value,
                   Compare comp) {
  ForwardIter i = lower_bound(first, last, value, comp);
  return i != last && !comp(value, *i);
}

/**
 * @brief next_permutation 会取得 [first,last) 区间的下一个排列组合
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[in] value 元素
 * @return 存在返回 true，否则返回 false
 */
template <class BidirectionalIter>
bool next_permutation(BidirectionalIter first, BidirectionalIter last) {
  if (first == last)
    return false;
  BidirectionalIter i = first;
  ++i;
  if (i == last)
    return false;
  i = last;
  --i;
  for (;;) {
    BidirectionalIter ii = i;
    --i;
    if (*i < *ii) {
      BidirectionalIter j = last;
      while (!(*i < *--j))
        ;
      iter_swap(i, j);
      reverse(ii, last);
      return true;
    }
    if (i == first) {
      reverse(first, last);
      return false;
    }
  }
}
/**
 * @brief next_permutation 会取得 [first,last) 区间的下一个排列组合
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[in] value 元素
 * @param[in] comp 条件
 * @return 存在返回 true，否则返回 false
 */
template <class BidirectionalIter, class Compare>
bool next_permutation(BidirectionalIter first,
                      BidirectionalIter last,
                      Compare comp) {
  if (first == last)
    return false;
  BidirectionalIter i = first;
  ++i;
  if (i == last)
    return false;
  i = last;
  --i;
  for (;;) {
    BidirectionalIter ii = i;
    --i;
    if (comp(*i, *ii)) {
      BidirectionalIter j = last;
      while (!comp(*i, *--j))
        ;
      iter_swap(i, j);
      reverse(ii, last);
      return true;
    }
    if (i == first) {
      reverse(first, last);
      return false;
    }
  }
}
/**
 * @brief prev_permutation 会取得 [first,last) 区间的前一个排列组合
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[in] value 元素
 * @return 存在返回 true，否则返回 false
 */
template <class BidirectionalIter>
bool prev_permutation(BidirectionalIter first, BidirectionalIter last) {
  if (first == last)
    return false;
  BidirectionalIter i = first;
  ++i;
  if (i == last)
    return false;
  i = last;
  --i;
  for (;;) {
    BidirectionalIter ii = i;
    --i;
    if (*i > *ii) {
      BidirectionalIter j = last;
      while (!(*i > *--j))
        ;
      iter_swap(i, j);
      reverse(ii, last);
      return true;
    }
    if (i == first) {
      reverse(first, last);
      return false;
    }
  }
}
/**
 * @brief prev_permutation 会取得 [first,last) 区间的下一个排列组合
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[in] value 元素
 * @param[in] comp 条件
 * @return 存在返回 true，否则返回 false
 */
template <class BidirectionalIter, class Compare>
bool prev_permutation(BidirectionalIter first,
                      BidirectionalIter last,
                      Compare comp) {
  if (first == last)
    return false;
  BidirectionalIter i = first;
  ++i;
  if (i == last)
    return false;
  i = last;
  --i;
  for (;;) {
    BidirectionalIter ii = i;
    --i;
    if (comp(*ii, *i)) {
      BidirectionalIter j = last;
      while (!comp(*--j, *i))
        ;
      iter_swap(i, j);
      reverse(ii, last);
      return true;
    }
    if (i == first) {
      reverse(first, last);
      return false;
    }
  }
}
/**
 * @brief random_shuffle 会将 [first,last) 区间的元素次序随机重排
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 */
template <class RandomAccessIter>
inline void random_shuffle(RandomAccessIter first, RandomAccessIter last) {
  _random_shuffle(first, last, distance_type(first));
}
template <class RandomAccessIter, class Distance>
void _random_shuffle(RandomAccessIter first, RandomAccessIter last, Distance*) {
  if (first == last)
    return;
  for (RandomAccessIter i = first + 1; i != last; ++i)
#ifndef __STL_NO_DRAND48
    iter_swap(i, first + Distance(rand() % ((i - first) + 1)));
#else
    iter_swap(i, first + Distance(lrand48() % ((i - first) + 1)));
#endif
}
/**
 * @brief random_shuffle 会将 [first,last) 区间的元素次序随机重排
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[in] rand 产生随机数的的仿函数
 */
template <class RandomAccessIter, class RandomNumberGenerator>
void random_shuffle(RandomAccessIter first,
                    RandomAccessIter last,
                    RandomNumberGenerator& rand) {
  if (first == last)
    return;
  for (RandomAccessIter i = first + 1; i != last; ++i)
    iter_swap(i, first + rand((i - first) + 1));
}
/**
 * @brief partial_sort 接受一个位于 [first,last) 区间的迭代器 middle
 * 重新安排区间 使得middle-first个最小元素以递增顺序排序，放在[first, middle)内
 * 其余last-middle元素放在[middle,last)中，不保证有任何特定顺序
 * @param[in] first 区间头部迭代器
 * @param[in] middle 区间中的任意迭代器
 * @param[in] last 区间尾部迭代器
 */
template <class RandomAccessIter>
inline void partial_sort(RandomAccessIter first,
                         RandomAccessIter middle,
                         RandomAccessIter last) {
  make_heap(first, middle);
  for (RandomAccessIter i = middle; i < last; ++i)
    if (*i < *first)
      _pop_heap(first, middle, i, T(*i), distance_type(first));
  sort_heap(first, middle);
}
// 重载版本 用 comp 代替比较
template <class RandomAccessIter, class Compare>
inline void partial_sort(RandomAccessIter first,
                         RandomAccessIter middle,
                         RandomAccessIter last,
                         Compare comp) {
  make_heap(first, middle);
  for (RandomAccessIter i = middle; i < last; ++i)
    if (comp(*i, *first))
      _pop_heap(first, middle, i, T(*i), distance_type(first));
  sort_heap(first, middle);
}

/**
 * @brief partial_sort_copy 接受一个位于 [first,last) 区间
 * 重新安排区间
 * 使得(last-first)个最小元素排序后的结果，放在[result_first,result_last)内
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[out] result_first 区间中的任意迭代器
 * @param[out] result_last 区间尾部迭代器
 */
template <class InputIter, class RandomAccessIter>
inline RandomAccessIter partial_sort_copy(InputIter first,
                                          InputIter last,
                                          RandomAccessIter result_first,
                                          RandomAccessIter result_last) {
  _partial_sort_copy(first, last, result_first, result_last,
                     distance_type(result_first));
}
template <class InputIter, class RandomAccessIter, class Distance>
RandomAccessIter psort_copy_aux(InputIter first,
                                InputIter last,
                                RandomAccessIter result_first,
                                RandomAccessIter result_last,
                                Distance*) {
  if (result_first == result_last)
    return result_last;
  auto result_iter = result_first;
  while (first != last && result_iter != result_last) {
    *result_iter = *first;
    ++result_iter;
    ++first;
  }
  make_heap(result_first, result_iter);
  while (first != last) {
    if (*first < *result_first) {
      _adjust_heap(result_first, (Distance*)(0), result_iter - result_first,
                   *first);
    }
    ++first;
  }
  sort_heap(result_first, result_iter);
  return result_iter;
}
// 重载版本 用 comp 代替比较
template <class InputIter, class RandomAccessIter, class Compare>
inline RandomAccessIter partial_sort_copy(InputIter first,
                                          InputIter last,
                                          RandomAccessIter result_first,
                                          RandomAccessIter result_last,
                                          Compare comp) {
  return _partial_sort_copy(first, last, result_first, result_last,
                            distance_type(result_first), comp);
}
template <class InputIter, class RandomIter, class Distance, class Compared>
RandomIter psort_copy_aux(InputIter first,
                          InputIter last,
                          RandomIter result_first,
                          RandomIter result_last,
                          Distance*,
                          Compared comp) {
  if (result_first == result_last)
    return result_last;
  auto result_iter = result_first;
  while (first != last && result_iter != result_last) {
    *result_iter = *first;
    ++result_iter;
    ++first;
  }
  make_heap(result_first, result_iter, comp);
  while (first != last) {
    if (comp(*first, *result_first)) {
      _adjust_heap(result_first, (Distance*)(0), result_iter - result_first,
                   *first, comp);
    }
    ++first;
  }
  sort_heap(result_first, result_iter, comp);
  return result_iter;
}

// insertion sort
// 版本1
template <class RandomAccessIter>
void _insertion_sort(RandomAccessIter first, RandomAccessIter last) {
  if (first == last)
    return;
  for (RandomAccessIter i = first + 1; i != last; ++i)
    _linear_insert(first, i, value_type(first));
}
template <class RandomAccessIter, class T>
inline void _linear_insert(RandomAccessIter first, RandomAccessIter last, T*) {
  T value = *last;
  if (value < *first) {
    copy_backward(first, last, last + 1);
    *first = value;
  } else
    _unguarded_linear_insert(last, value);
}
template <class RandomAccessIter, class T>
void _unguarded_linear_insert(RandomAccessIter last, T value) {
  RandomAccessIter next = last;
  --next;
  // 一旦不在出现逆转对，循环结束
  while (value < *next) {
    *last = *next;
    last = next;
    --next;
  }
  *last = value;
}
// 返回 a b c 之中居中者
template <class T>
inline const T& _median(const T& a, const T& b, const T& c) {
  if (a < b)
    if (b < c)
      return b;
    else if (a < c)
      return c;
    else
      return a;
  else if (a < c)
    return a;
  else if (b < c)
    return c;
  else
    return b;
}
// 分割函数
template <class RandomAccessIter, class T>
RandomAccessIter _unguarded_partition(RandomAccessIter first,
                                      RandomAccessIter last,
                                      T pivot) {
  while (true) {
    while (*first < pivot)
      ++first;
    --last;
    while (pivot < *last)
      --last;
    // 只适用于random iterator
    if (!(first < last))
      return first;
    iter_swap(first, last);
    ++first;
  }
}
/**
 * @brief sort 接受一个位于 [first,last) 区间
 * 重新安排区间使得元素从小到大排列
 * 只适用于 RandomAccessIterator
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 */
template <class RandomAccessIter>
inline void sort(RandomAccessIter first, RandomAccessIter last) {
  if (first != last) {
    _introsort_loop(first, last, value_type(first), _lg(last - first) * 2);
    _final_insertion_sort(first, last);
  }
}
// _lg 用来控制分割恶化情况
template <class Size>
inline Size _lg(Size n) {
  Size k;
  for (k = 0; n > 1; n >>= 1)
    ++k;
  return k;
}
// introsort算法
// 版本一
const int __ministl_threshold = 16;
template <class RandomAccessIter, class T, class Size>
void _introsort_loop(RandomAccessIter first,
                     RandomAccessIter last,
                     T*,
                     Size depth_limit) {
  while (last - first > __ministl_threshold) {
    if (depth_limit == 0) {
      partial_sort(first, last, last);
      return;
    }
    --depth_limit;
    // 分割点落在cut身上
    RandomAccessIter cut = _unguarded_partition(
        first, last,
        T(_median(*first, *(first + (last - first) / 2), *(last - 1))));

    // 对右半递归进行sort
    _introsort_loop(cut, last, value_type(first), depth_limit);
    last = cut;
  }
}
// 版本1
template <class RandomAccessIter>
void _final_insertion_sort(RandomAccessIter first, RandomAccessIter last) {
  if (last - first > __ministl_threshold) {
    _insertion_sort(first, first + __ministl_threshold);
    _unguarded_insertion_sort(first + __ministl_threshold, last);
  } else
    _insertion_sort(first, last);
}
// 版本1
template <class RandomAccessIter>
inline void _unguarded_insertion_sort(RandomAccessIter first,
                                      RandomAccessIter last) {
  _unguarded_insertion_sort_aux(first, last, value_type(first));
}
template <class RandomAccessIter, class T>
void _unguarded_insertion_sort_aux(RandomAccessIter first,
                                   RandomAccessIter last,
                                   T*) {
  for (RandomAccessIter i = first; i != last; ++i)
    _unguarded_linear_insert(i, T(*i));
}
/**
 * @brief equal_range (应用于有序区间 升序) 接受一个位于 [first,last) 区间
 * 在区间中寻找value，返回一对迭代器
 * @param[in] first 区间头部迭代器
 * @param[in] last 区间尾部迭代器
 * @param[in] value 数据
 * @return
 * 返回一个pair，其中first是在不破坏次序的前提下，value可插入的第一个位置
 * second是在不破坏次序的前提下，value可插入的最后一个位置
 */
template <class ForwardIter, class T>
inline pair<ForwardIter, ForwardIter> equal_range(ForwardIter first,
                                                  ForwardIter last,
                                                  const T& value) {
  return _equal_range(first, last, value, distance_type(first),
                      iterator_category(first));
}
template <class RandomAccessIter, class T, class Distance>
pair<RandomAccessIter, RandomAccessIter> _equal_range(
    RandomAccessIter first,
    RandomAccessIter last,
    const T& value,
    Distance*,
    random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIter middle, left, right;
  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (*middle < value) {
      first = middle + 1;
      len = len - half - 1;
    } else if (value < *middle)
      len = half;
    else {
      left = lower_bound(first, middle, value);
      right = upper_bound(++middle, first + len, value);
      return pair<RandomAccessIter, RandomAccessIter>(left, right);
    }
  }
  return pair<RandomAccessIter, RandomAccessIter>(first, first);
}
template <class ForwardIter, class T, class Distance>
pair<ForwardIter, ForwardIter> _equal_range(ForwardIter first,
                                            ForwardIter last,
                                            const T& value,
                                            Distance*,
                                            forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);
  Distance half;
  ForwardIter middle, left, right;
  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);
    if (*middle < value) {
      first = middle;
      ++first;
      len = len - half - 1;
    } else if (value < *middle)
      len = half;
    else {
      left = lower_bound(first, middle, value);
      advance(first, len);
      right = upper_bound(++middle, first, value);
      return pair<ForwardIter, ForwardIter>(left, right);
    }
  }
  return pair<ForwardIter, ForwardIter>(first, first);
}
/**
 * @brief inplace_merge (应用于有序区间 升序)
 * 如果两个连在一起的序列，[first, middle),[middle,
 * last)都已是排好序的，那么这个算法可以将它们结合成单一序列，仍然有序
 * @param[in] first 区间头部迭代器
 * @param[in] middle 区间内迭代器
 * @param[in] last 区间尾部迭代器
 */
template <class BidirectionalIter>
inline void inplace_merge(BidirectionalIter first,
                          BidirectionalIter middle,
                          BidirectionalIter last) {
  if (first == middle || middle == last)
    return;
  _inplace_merge_aux(first, middle, last, value_type(first),
                     distance_type(first));
}
template <class BidirectionalIter, class T, class Distance>
inline void _inplace_merge_aux(BidirectionalIter first,
                               BidirectionalIter middle,
                               BidirectionalIter last,
                               T*,
                               Distance*) {
  Distance len1 = 0;
  distance(first, middle, len1);
  Distance len2 = 0;
  distance(middle, last, len2);

  temporary_buffer<BidirectionalIter, T> buf(first, last);
  if (buf.begin() == 0)
    _merge_without_buffer(first, middle, last, len1, len2);
  else
    _merge_adaptive(first, middle, last, len1, len2, buf.begin(),
                    Distance(buf.size()));
}
// 没有缓冲区的情况下合并
template <class BidirectionalIter, class Distance>
void _merge_without_buffer(BidirectionalIter first,
                           BidirectionalIter middle,
                           BidirectionalIter last,
                           Distance len1,
                           Distance len2) {
  if (len1 == 0 || len2 == 0)
    return;
  if (len1 + len2 == 2) {
    if (*middle < *first)
      iter_swap(first, middle);
    return;
  }
  auto first_cut = first;
  auto second_cut = middle;
  Distance len11 = 0;
  Distance len22 = 0;
  if (len1 > len2) {  // 序列一较长，找到序列一的中点
    len11 = len1 >> 1;
    advance(first_cut, len11);
    second_cut = lower_bound(middle, last, *first_cut);
    len22 = distance(middle, second_cut);
  } else {  // 序列二较长，找到序列二的中点
    len22 = len2 >> 1;
    advance(second_cut, len22);
    first_cut = upper_bound(first, middle, *second_cut);
    len11 = distance(first, first_cut);
  }
  auto new_middle = rotate(first_cut, middle, second_cut);
  _merge_without_buffer(first, first_cut, new_middle, len11, len22);
  _merge_without_buffer(new_middle, second_cut, last, len1 - len11,
                        len2 - len22);
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter1 merge_backward(BidirectionalIter1 first1,
                                  BidirectionalIter1 last1,
                                  BidirectionalIter2 first2,
                                  BidirectionalIter2 last2,
                                  BidirectionalIter1 result) {
  if (first1 == last1)
    return copy_backward(first2, last2, result);
  if (first2 == last2)
    return copy_backward(first1, last1, result);
  --last1;
  --last2;
  while (true) {
    if (*last2 < *last1) {
      *--result = *last1;
      if (first1 == last1)
        return copy_backward(first2, ++last2, result);
      --last1;
    } else {
      *--result = *last2;
      if (first2 == last2)
        return copy_backward(first1, ++last1, result);
      --last2;
    }
  }
}

template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
BidirectionalIter1 rotate_adaptive(BidirectionalIter1 first,
                                   BidirectionalIter1 middle,
                                   BidirectionalIter1 last,
                                   Distance len1,
                                   Distance len2,
                                   BidirectionalIter2 buffer,
                                   Distance buffer_size) {
  BidirectionalIter2 buffer_end;
  if (len1 > len2 && len2 <= buffer_size) {
    buffer_end = copy(middle, last, buffer);
    copy_backward(first, middle, last);
    return copy(buffer, buffer_end, first);
  } else if (len1 <= buffer_size) {
    buffer_end = copy(first, middle, buffer);
    copy(middle, last, first);
    return copy_backward(buffer, buffer_end, last);
  } else {
    return rotate(first, middle, last);
  }
}

// 有缓冲区的情况下合并
template <class BidirectionalIter, class Distance, class Pointer>
void _merge_adaptive(BidirectionalIter first,
                     BidirectionalIter middle,
                     BidirectionalIter last,
                     Distance len1,
                     Distance len2,
                     Pointer buffer,
                     Distance buffer_size) {
  // 区间长度足够放进缓冲区
  if (len1 <= len2 && len1 <= buffer_size) {
    Pointer buffer_end = copy(first, middle, buffer);
    merge(buffer, buffer_end, middle, last, first);
  } else if (len2 <= buffer_size) {
    Pointer buffer_end = copy(middle, last, buffer);
    merge_backward(first, middle, buffer, buffer_end, last);
  } else {  // 区间长度太长，分割递归处理
    auto first_cut = first;
    auto second_cut = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if (len1 > len2) {
      len11 = len1 >> 1;
      advance(first_cut, len11);
      second_cut = lower_bound(middle, last, *first_cut);
      len22 = distance(middle, second_cut);
    } else {
      len22 = len2 >> 1;
      advance(second_cut, len22);
      first_cut = upper_bound(first, middle, *second_cut);
      len11 = distance(first, first_cut);
    }
    auto new_middle = rotate_adaptive(first_cut, middle, second_cut,
                                      len1 - len11, len22, buffer, buffer_size);
    _merge_adaptive(first, first_cut, new_middle, len11, len22, buffer,
                    buffer_size);
    _merge_adaptive(new_middle, second_cut, last, len1 - len11, len2 - len22,
                    buffer, buffer_size);
  }
}
/**
 * @brief nth_element
 * 重新排列[first,last)，使迭代器nth所指的元素，与整个区间完整排序后
 * 同一位置的元素同值，并保证[nth,last)内没有一个元素小于[first,nth)内的元素
 * 子区间[nth,last)和[first,nth)内的元素次序无保正
 * @param[in] first 区间头部迭代器
 * @param[in] nth 区间内迭代器
 * @param[in] last 区间尾部迭代器
 */
template <class RandomAccessIter>
inline void nth_element(RandomAccessIter first,
                        RandomAccessIter nth,
                        RandomAccessIter last) {
  _nth_element(first, nth, last, value_type(first));
}
template <class RandomAccessIter, class T>
void _nth_element(RandomAccessIter first,
                  RandomAccessIter nth,
                  RandomAccessIter last,
                  T*) {
  while (last - first > 3) {
    RandomAccessIter cut = _unguarded_partition(
        first, last,
        T(_median(*first, *(first + (last - first) / 2), *(last - 1))));
    if (cut < nth)
      first = cut;
    else
      last = cut;
  }
  _insertion_sort(first, last);
}

_MINISTL_END

#endif