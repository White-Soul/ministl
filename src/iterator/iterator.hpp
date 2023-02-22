#ifndef MINI_ITERATOR_H
#define MINI_ITERATOR_H
#include "../utils/util.hpp"

_MINISTL_BEGIN

// 5种迭代器类型
// 只读
struct input_iterator_tag {};
// 只写
struct output_iterator_tag {};
// 可读写
struct forward_iterator_tag : public output_iterator_tag,
                              public input_iterator_tag {};
// 可双向移动
struct bidirectional_iterator_tag : public forward_iterator_tag {};
// 涵盖所有指针算术
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// iterator
template <class Category,
          class T,
          class Distance = ptrdiff_t,
          class Pointer = T*,
          class Reference = T&>
struct iterator {
  typedef Category iterator_category;
  typedef T value_type;
  typedef Distance difference_type;
  typedef Pointer pointer;
  typedef Reference reference;
};
// 萃取迭代器的特性 traits
template <class Iterator>
struct iterator_traits {
  typedef typename Iterator::iterator_category iterator_category;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::difference_type difference_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference reference;
};
// 原生指针特化版
template <class T>
struct iterator_traits<T*> {
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef ptrdiff_t difference_type;
  typedef T* pointer;
  typedef T& reference;
};
// const 原生指针特化版
template <class T>
struct iterator_traits<const T*> {
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef ptrdiff_t difference_type;
  typedef const T* pointer;
  typedef const T& reference;
};
// 这个函数可以很方便决定某个迭代器类型
template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category iterator_category(
    const Iterator&) {
  typedef typename iterator_traits<Iterator>::iterator_category category;
  return category();
}
// 这个函数可以很方便决定迭代器的distance type
template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type* distance_type(
    const Iterator&) {
  return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}
// 这个函数可以很方便决定迭代器的value type
template <class Iterator>
inline typename iterator_traits<Iterator>::value_type* value_type(
    const Iterator&) {
  return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}
// 下面是整组distance函数
template <class InputIter>
inline typename iterator_traits<InputIter>::difference_type
_distance(InputIter first, InputIter last, input_iterator_tag) {
  typename iterator_traits<InputIter>::difference_type n = 0;
  while (first != last) {
    ++first;
    ++n;
  }
  return n;
}
template <class RandomAccessIter>
inline typename iterator_traits<RandomAccessIter>::difference_type _distance(
    RandomAccessIter first,
    RandomAccessIter last,
    random_access_iterator_tag) {
  return last - first;
}
template <class InputIter>
inline typename iterator_traits<InputIter>::difference_type distance(
    InputIter first,
    InputIter last) {
  typedef typename iterator_traits<InputIter>::iterator_category category;
  return _distance(first, last, category());
}
// 下面是整组advance函数
template <class InputIter, class Distance>
inline void _advance(InputIter& i, Distance n, input_iterator_tag) {
  while (n--)
    i++;
}
template <class BidirectionalIter, class Distance>
inline void _advance(BidirectionalIter& i,
                     Distance n,
                     bidirectional_iterator_tag) {
  if (n >= 0)
    while (n--)
      i++;
  else
    while (n++)
      i--;
}
template <class RandomAccessIter, class Distance>
inline void _advance(RandomAccessIter& i,
                     Distance n,
                     random_access_iterator_tag) {
  i += n;
}
template <class InputIter, class Distance>
inline void advance(InputIter& i, Distance n) {
  _advance(i, n, iterator_category(i));
}
// 定义truetype和falsetype
struct _true_type {};
struct _false_type {};
// type_traits 萃取类型的特性
template <class type>
struct _type_traits {
  typedef _true_type this_dummy_member_must_be_first;

  typedef _false_type has_trivial_default_constructor;
  typedef _false_type has_trivial_copy_constructor;
  typedef _false_type has_trivial_assignment_operator;
  typedef _false_type has_trivial_destructor;
  typedef _false_type is_POD_type;
};
// 对每一种标量定义特化版本
template <>
struct _type_traits<char> {
  typedef _true_type has_trivial_default_constructor;
  typedef _true_type has_trivial_copy_constructor;
  typedef _true_type has_trivial_assignment_operator;
  typedef _true_type has_trivial_destructor;
  typedef _true_type is_POD_type;
};
template <>
struct _type_traits<signed char> {
  typedef _true_type has_trivial_default_constructor;
  typedef _true_type has_trivial_copy_constructor;
  typedef _true_type has_trivial_assignment_operator;
  typedef _true_type has_trivial_destructor;
  typedef _true_type is_POD_type;
};
template <>
struct _type_traits<unsigned char> {
  typedef _true_type has_trivial_default_constructor;
  typedef _true_type has_trivial_copy_constructor;
  typedef _true_type has_trivial_assignment_operator;
  typedef _true_type has_trivial_destructor;
  typedef _true_type is_POD_type;
};
template <>
struct _type_traits<short> {
  typedef _true_type has_trivial_default_constructor;
  typedef _true_type has_trivial_copy_constructor;
  typedef _true_type has_trivial_assignment_operator;
  typedef _true_type has_trivial_destructor;
  typedef _true_type is_POD_type;
};
template <>
struct _type_traits<unsigned short> {
  typedef _true_type has_trivial_default_constructor;
  typedef _true_type has_trivial_copy_constructor;
  typedef _true_type has_trivial_assignment_operator;
  typedef _true_type has_trivial_destructor;
  typedef _true_type is_POD_type;
};
template <>
struct _type_traits<int> {
  typedef _true_type has_trivial_default_constructor;
  typedef _true_type has_trivial_copy_constructor;
  typedef _true_type has_trivial_assignment_operator;
  typedef _true_type has_trivial_destructor;
  typedef _true_type is_POD_type;
};
template <>
struct _type_traits<unsigned int> {
  typedef _true_type has_trivial_default_constructor;
  typedef _true_type has_trivial_copy_constructor;
  typedef _true_type has_trivial_assignment_operator;
  typedef _true_type has_trivial_destructor;
  typedef _true_type is_POD_type;
};
template <>
struct _type_traits<long> {
  typedef _true_type has_trivial_default_constructor;
  typedef _true_type has_trivial_copy_constructor;
  typedef _true_type has_trivial_assignment_operator;
  typedef _true_type has_trivial_destructor;
  typedef _true_type is_POD_type;
};
template <>
struct _type_traits<unsigned long> {
  typedef _true_type has_trivial_default_constructor;
  typedef _true_type has_trivial_copy_constructor;
  typedef _true_type has_trivial_assignment_operator;
  typedef _true_type has_trivial_destructor;
  typedef _true_type is_POD_type;
};
template <>
struct _type_traits<float> {
  typedef _true_type has_trivial_default_constructor;
  typedef _true_type has_trivial_copy_constructor;
  typedef _true_type has_trivial_assignment_operator;
  typedef _true_type has_trivial_destructor;
  typedef _true_type is_POD_type;
};
template <>
struct _type_traits<double> {
  typedef _true_type has_trivial_default_constructor;
  typedef _true_type has_trivial_copy_constructor;
  typedef _true_type has_trivial_assignment_operator;
  typedef _true_type has_trivial_destructor;
  typedef _true_type is_POD_type;
};
template <>
struct _type_traits<long double> {
  typedef _true_type has_trivial_default_constructor;
  typedef _true_type has_trivial_copy_constructor;
  typedef _true_type has_trivial_assignment_operator;
  typedef _true_type has_trivial_destructor;
  typedef _true_type is_POD_type;
};
// 偏特化版
template <class T>
struct _type_traits<T*> {
  typedef _true_type has_trivial_default_constructor;
  typedef _true_type has_trivial_copy_constructor;
  typedef _true_type has_trivial_assignment_operator;
  typedef _true_type has_trivial_destructor;
  typedef _true_type is_POD_type;
};

/*================================================*/
// 反向迭代器
template <class Iterator>
class reverse_iterator {
 private:
  Iterator current;  // 对应的正向迭代器；
 public:
  // 反向迭代器的五种相应型别
  typedef
      typename iterator_traits<Iterator>::iterator_category iterator_category;
  typedef typename iterator_traits<Iterator>::value_type value_type;
  typedef typename iterator_traits<Iterator>::difference_type difference_type;
  typedef typename iterator_traits<Iterator>::pointer pointer;
  typedef typename iterator_traits<Iterator>::reference reference;

  typedef Iterator iterator_type;
  typedef reverse_iterator<Iterator> self;

 public:
  // 构造函数
  reverse_iterator() {}
  explicit reverse_iterator(iterator_type i) : current(i) {}
  reverse_iterator(const self& rhs) : current(rhs.current) {}

 public:
  // 取出对应的正向迭代器
  iterator_type base() const { return current; }
  // 重载操作符
  reference operator*() const {  // 实际对应正向迭代器的前一个位置
    auto tmp = current;
    return *--tmp;
  }
  pointer operator->() const { return &(operator*()); }

  // 前进(++)变为后退(--)
  self& operator++() {
    --current;
    return *this;
  }
  self operator++(int) {
    self tmp = *this;
    --current;
    return tmp;
  }
  // 后退(--)变为前进(++)
  self& operator--() {
    ++current;
    return *this;
  }
  self operator--(int) {
    self tmp = *this;
    ++current;
    return tmp;
  }

  self& operator+=(difference_type n) {
    current -= n;
    return *this;
  }
  self operator+(difference_type n) const { return self(current - n); }
  self& operator-=(difference_type n) {
    current += n;
    return *this;
  }
  self operator-(difference_type n) const { return self(current + n); }

  reference operator[](difference_type n) const { return *(*this + n); }
};
// 重载 operator-
template <class Iterator>
typename reverse_iterator<Iterator>::difference_type operator-(
    const reverse_iterator<Iterator>& lhs,
    const reverse_iterator<Iterator>& rhs) {
  return rhs.base() - lhs.base();
}

// 重载比较操作符
template <class Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return lhs.base() == rhs.base();
}

template <class Iterator>
bool operator<(const reverse_iterator<Iterator>& lhs,
               const reverse_iterator<Iterator>& rhs) {
  return rhs.base() < lhs.base();
}

template <class Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return !(lhs == rhs);
}

template <class Iterator>
bool operator>(const reverse_iterator<Iterator>& lhs,
               const reverse_iterator<Iterator>& rhs) {
  return rhs < lhs;
}

template <class Iterator>
bool operator<=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return !(rhs < lhs);
}

template <class Iterator>
bool operator>=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs) {
  return !(lhs < rhs);
}
_MINISTL_END

#endif