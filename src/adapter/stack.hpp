#ifndef MINISTL_STACK_H
#define MINISTL_STACK_H

#include "../container/deque.hpp"
#include "../utils/util.hpp"

_MINISTL_BEGIN

template <class T, class Sequence = deque<T>>
class stack {
  friend bool operator==(const stack& x, const stack& y) { return x.c == y.c; }
  friend bool operator<(const stack& x, const stack& y) { return x.c < y.c; }

 public:
  typedef typename Sequence::value_type value_type;
  typedef typename Sequence::size_type size_type;
  typedef typename Sequence::reference reference;
  typedef typename Sequence::const_reference const_reference;

 protected:
  Sequence c;

 public:
  stack() : c(){};
  bool empty() const { return c.empty(); }
  size_type size() const { return c.size(); }
  reference top() { return c.back(); }
  const_reference top() const { return c.back(); }
  void push(const value_type& x) { c.push_back(x); }
  void pop() { c.pop_back(); }
};

_MINISTL_END

#endif