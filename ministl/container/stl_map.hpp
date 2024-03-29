#ifndef MINISTL_MAP_H
#define MINISTL_MAP_H
#include "../configurator/allocator.hpp"
#include "../functor/functor.hpp"
#include "../utils/util.hpp"
#include "rb_tree.hpp"

_MINISTL_BEGIN

template <class Key,
          class T,
          class Compare = std::less<Key>,
          class Alloc = alloc>
class map
{
public:
  typedef Key key_type;
  typedef T data_type;
  typedef T mapped_type;
  typedef pair<Key, T> value_type;
  typedef Compare key_compare;
  // 定义一个仿函数 //调用元素比较函数
  class value_compare : public binary_function<value_type, value_type, bool>
  {
    friend class map<Key, T, Compare, Alloc>;

  protected:
    Compare comp;
    value_compare(Compare c) : comp(c) {}

  public:
    bool operator()(const value_type &x, const value_type &y) const
    {
      return comp(x.first, y.first);
    }
  };

private:
  typedef rb_tree<key_type,
                  value_type,
                  select1st<value_type>,
                  key_compare,
                  Alloc>
      rep_type;
  rep_type t;

public:
  typedef typename rep_type::pointer pointer;
  typedef typename rep_type::const_pointer poconst_pointerinter;
  typedef typename rep_type::reference reference;
  typedef typename rep_type::const_reference const_reference;
  typedef typename rep_type::iterator iterator;
  typedef typename rep_type::const_iterator const_iterator;
  typedef typename rep_type::reverse_iterator reverse_iterator;
  typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename rep_type::size_type size_type;
  typedef typename rep_type::difference_type difference_type;

  map() : t(Compare()) {}
  explicit map(const Compare &comp) : t(comp) {}

  template <class InputIter>
  map(InputIter first, InputIter last) : t(Compare())
  {
    t.insert_unique(first, last);
  }

  template <class InputIter>
  map(InputIter first, InputIter last, const Compare &comp) : t(comp)
  {
    t.insert_unique(first, last);
  }

  map(const map &x) : t(x.t) {}

  map &operator=(const map &x)
  {
    t = x.t;
    return *this;
  }

  key_compare key_comp() const { return t.key_comp(); }
  value_compare value_comp() const { return value_compare(t.key_comp()); }
  // 迭代器r
  iterator begin() { return t.begin(); }
  const_iterator begin() const { return t.begin(); }
  iterator end() { return t.end(); }
  const_iterator end() const { return t.end(); }
  // r迭代器
  reverse_iterator rbegin() { return t.rbegin(); }
  reverse_iterator rend() { return t.rend(); }
  const_reverse_iterator rbegin() const { return t.rbegin(); }
  const_reverse_iterator rend() const { return t.rend(); }
  // c迭代器
  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }
  const_reverse_iterator crbegin() const { return rbegin(); }
  const_reverse_iterator crend() const { return rend(); }

  bool empty() const { return t.empty(); }
  size_type size() const { return t.size(); }
  size_type max_size() const { return t.max_size(); }

  T &operator[](const key_type &k)
  {
    return (*(insert(value_type(k, T()))).first).second;
  }
  void swap(map &x) { t.swap(x.t); }
  // 插入
  pair<iterator, bool> insert(const value_type &x)
  {
    return t.insert_unique(x);
  }
  iterator insert(iterator pos, const value_type &x)
  {
    return t.insert_unique(pos, x);
  }
  template <class InputIter>
  void insert(InputIter first, InputIter last)
  {
    t.insert_unique(first,last);
  }
  // 删除
  void erase(iterator pos) { t.erase(pos); }
  size_type erase(const key_type &x) { return t.erase(x); }
  void erase(iterator first, iterator last) { t.erase(first, last); }
  void clear() { t.clear(); }
  // 操作
  iterator find(const key_type &x) { return t.find(x); }
  const_iterator find(const key_type &x) const { return t.find(x); }
  size_type count(const key_type &x) const { return t.count(x); }
  iterator lower_bound(const key_type &x) { return t.lower_bound(x); }
  const_iterator lower_bound(const key_type &x) const
  {
    return t.lower_bound(x);
  }
  iterator upper_bound(const key_type &x) { return t.upper_bound(x); }
  const_iterator upper_bound(const key_type &x) const
  {
    return t.upper_bound(x);
  }
  pair<iterator, iterator> equal_range(const key_type &x)
  {
    return t.equal_range(x);
  }
  pair<const_iterator, const_iterator> equal_range(const key_type &x) const
  {
    return t.equal_range(x);
  }
  friend bool operator==(const map &x, const map &y) { return x.t == y.t; }
  friend bool operator<(const map &x, const map &y) { return x.t < y.t; }
};

_MINISTL_END

#endif