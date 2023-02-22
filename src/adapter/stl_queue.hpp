#ifndef MINISTL_QUEUE_H
#define MINISTL_QUEUE_H

#include "../algorithm/stl_algorithm.hpp"
#include "../container/stl_deque.hpp"
#include "../container/stl_vector.hpp"
#include "../utils/util.hpp"

_MINISTL_BEGIN
// queue
template <class T, class Sequence = deque<T>>
class queue {
  friend bool operator==(const queue& x, const queue& y) { return x.c == y.c; }
  friend bool operator<(const queue& x, const queue& y) { return x.c < y.c; }

 public:
  typedef typename Sequence::value_type value_type;
  typedef typename Sequence::size_type size_type;
  typedef typename Sequence::reference reference;
  typedef typename Sequence::const_reference const_reference;

 protected:
  Sequence c;

 public:
  bool empty() const { return c.empty(); }
  size_type size() const { return c.size(); }
  reference front() { return c.front(); }
  const_reference front() const { return c.front(); }
  reference back() { return c.back(); }
  const_reference back() const { return c.back(); }
  void push(const value_type& x) { c.push_back(x); }
  void pop() { c.pop_back(); }
};
// priority_queue
template <class T,
          class Sequence = vector<T>,
          class Compare = std::less<typename Sequence::value_type>>
class priority_queue {
 public:
  typedef typename Sequence::value_type value_type;
  typedef typename Sequence::size_type size_type;
  typedef typename Sequence::reference reference;
  typedef typename Sequence::const_reference const_reference;

 protected:
  Sequence c;    // 底层容器
  Compare comp;  // 元素大小比较标准
 public:
  priority_queue() : c() {}
  explicit priority_queue(const Compare& x) : c(), comp(x) {}

  template <class InputIter>
  priority_queue(InputIter first, InputIter last, const Compare& x)
      : c(first, last), comp(x) {
    make_heap(c.begin(), c.end(), comp);
  }
  template <class InputIter>
  priority_queue(InputIter first, InputIter last) : c(first, last) {
    make_heap(c.begin(), c.end(), comp);
  }
  bool empty() const { return c.empty(); }
  size_type size() const { return c.size(); }
  reference top() { return c.front(); }
  const_reference top() const { return c.front(); }
  void push(const value_type& x) {
    try {
      c.push_back(x);
      push_heap(c.begin(), c.end(), comp);
    } catch (...) {
      c.clear();
    }
  }
  void pop() {
    try {
      pop_heap(c.begin(), c.end(), comp);
      c.pop_back();
    } catch (...) {
      c.clear();
    }
  }
};

_MINISTL_END

#endif