#ifndef MINISTL_FUNCTOR_H
#define MINISTL_FUNCTOR_H

#include "../utils/util.hpp"

_MINISTL_BEGIN
// unary_function用来呈现一元函数的参数型别和返回型别
template <class Arg, class Result>
struct unary_function {
  typedef Arg argument_type;
  typedef Result result_type;
};
// binary_function 用来呈现二元函数的第一，第二参数型别和返回类型
template <class Arg1, class Arg2, class Result>
struct binary_function {
  typedef Arg1 first_argument_type;
  typedef Arg2 second_argument_type;
  typedef Result result_type;
};
// 算术类仿函数
template <class T>
struct plus : public binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const { return x + y; }
};
template <class T>
struct minus : public binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const { return x - y; }
};
template <class T>
struct multiplies : public binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const { return x * y; }
};
template <class T>
struct divides : public binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const { return x / y; }
};
template <class T>
struct modulus : public binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const { return x % y; }
};
template <class T>
struct negate : public unary_function<T, T> {
  T operator()(const T& x) const { return -x; }
};
// 证同元素
template <class T>
inline T identity_element(plus<T>) {
  return T(0);
}
template <class T>
inline T identity_element(multiplies<T>) {
  return T(1);
}
// 关系运算仿函数
template <class T>
struct equal_to : public binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x == y; }
};
template <class T>
struct not_equal_to : public binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x != y; }
};
template <class T>
struct greater : public binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x > y; }
};
template <class T>
struct greater_equal : public binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x >= y; }
};
template <class T>
struct less : public binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T>
struct less_equal : public binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x <= y; }
};
// 逻辑运算类
template <class T>
struct logical_and : public binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x && y; }
};
template <class T>
struct logical_or : public binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const { return x || y; }
};
template <class T>
struct logical_not : public unary_function<T, bool> {
  bool operator()(const T& x) const { return !x; }
};
// 证同，
template <class T>
struct identity : public unary_function<T, T> {
  const T& operator()(const T& x) const { return x; }
};
// 选择，
template <class Pair>
struct select1st : public unary_function<Pair, typename Pair::first_type> {
  const typename Pair::first_type& operator()(const Pair& x) const {
    return x.first;
  }
};
template <class Pair>
struct select2nd : public unary_function<Pair, typename Pair::second_type> {
  const typename Pair::second_type& operator()(const Pair& x) const {
    return x.second;
  }
};
// 投影
template <class Arg1, class Arg2>
struct project1st : public binary_function<Arg1, Arg2, Arg1> {
  Arg1 operator()(const Arg1& x, const Arg2& y) const { return x; }
};
template <class Arg1, class Arg2>
struct project2nd : public binary_function<Arg1, Arg2, Arg2> {
  Arg2 operator()(const Arg1& x, const Arg2& y) const { return y; }
};

_MINISTL_END

#endif