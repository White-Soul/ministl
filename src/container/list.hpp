#ifndef MINISTL_LIST_H
#define MINISTL_LIST_H

#include "../configurator/allocator.hpp"
#include "../iterator/iterator.hpp"

_MINISTL_BEGIN

template <class T>
struct _list_node {
  typedef _list_node<T>* void_pointer;
  void_pointer prev;
  void_pointer next;
  T data;
};
// 迭代器
template <class T, class Ref, class Ptr>
struct _list_iterator {
  typedef _list_iterator<T, T&, T*> iterator;
  typedef _list_iterator<T, const T&, const T*> const_iterator;
  typedef _list_iterator<T, Ref, Ptr> self;

  typedef bidirectional_iterator_tag iterator_category;
  typedef T value_type;
  typedef Ptr pointer;
  typedef Ref reference;
  typedef _list_node<T>* link_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  link_type node;  // 迭代器内部当然有普通指针，指向list节点

  // constructor
  _list_iterator(link_type x) : node(x) {}
  _list_iterator() {}
  _list_iterator(const iterator& x) : node(x.node) {}
  _list_iterator(const const_iterator& x) : node(x.node) {}

  bool operator==(const self& x) const { return node == x.node; }
  bool operator!=(const self& x) const { return node != x.node; }

  // 对迭代器取值,节点的数据值
  reference operator*() const { return (*node).data; }
  // 迭代器成员存取运算子的标准做法
  pointer operator->() const { return &(operator*()); }
  // 对迭代器累加1，就是前进一个节点
  self& operator++() {
    node = (link_type)((*node).next);
    return *this;
  }
  self operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
  }
  // 对迭代器递减1，就是后退一个节点
  self& operator--() {
    node = (link_type)((*node).prev);
    return *this;
  }
  self operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
  }
};
// const迭代器
template <class T, class Ref, class Ptr>
struct _list_const_iterator {
  typedef _list_iterator<T, T&, T*> iterator;
  typedef _list_const_iterator<T, T&, T*> const_iterator;
  typedef _list_const_iterator<T, Ref, Ptr> self;

  typedef bidirectional_iterator_tag iterator_category;
  typedef T value_type;
  typedef const Ptr pointer;
  typedef const Ref reference;
  typedef _list_node<T>* link_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  link_type node;  // 迭代器内部当然有普通指针，指向list节点

  // constructor
  _list_const_iterator(link_type x) : node(x) {}
  _list_const_iterator() {}
  _list_const_iterator(const iterator& x) : node(x.node) {}
  _list_const_iterator(const const_iterator& x) : node(x.node) {}

  bool operator==(const self& x) const { return node == x.node; }
  bool operator!=(const self& x) const { return node != x.node; }

  // 对迭代器取值,节点的数据值
  reference operator*() const { return (*node).data; }
  // 迭代器成员存取运算子的标准做法
  pointer operator->() const { return &(operator*()); }
  // 对迭代器累加1，就是前进一个节点
  self& operator++() {
    node = (link_type)((*node).next);
    return *this;
  }
  self operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
  }
  // 对迭代器递减1，就是后退一个节点
  self& operator--() {
    node = (link_type)((*node).prev);
    return *this;
  }
  self operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
  }
};

template <class T, class Alloc = alloc>
class list {
 protected:
  typedef _list_node<T> list_node;
  // 专属空间配置器,每次配置一个节点大小
  typedef allocator<list_node, Alloc> list_node_allocator;

 public:
  typedef
      typename _list_iterator<T, T&, T*>::iterator_category iterator_category;
  typedef _list_iterator<T, T&, T*> iterator;
  typedef _list_const_iterator<T, T&, T*> const_iterator;
  typedef ministl::reverse_iterator<iterator> reverse_iterator;
  typedef ministl::reverse_iterator<const_iterator> const_reverse_iterator;

  typedef typename iterator::reference reference;
  typedef typename const_iterator::reference const_reference;
  typedef typename iterator::pointer pointer;
  typedef typename const_iterator::pointer const_pointer;
  typedef typename iterator::difference_type difference_type;
  typedef typename iterator::value_type value_type;
  typedef typename iterator::size_type size_type;
  typedef list_node* link_type;

 protected:
  link_type node;
  // 配置一个节点并传回
  link_type get_node() { return list_node_allocator::allocate(); }
  // 释放一个节点
  void put_node(link_type p) { list_node_allocator::deallocate(p); }
  // 产生一个节点，带有元素值
  link_type create_node(const T& x) {
    link_type p = get_node();
    construct(&p->data, x);
    return p;
  }
  // 销毁一个节点
  void destroy_node(link_type p) {
    destroy(&p->data);
    put_node(p);
  }
  // 产生一个空链表
  void empty_initialize() {
    node = get_node();  // 配置一个节点，令node指向它
    node->next = node;  // 令node头尾都指向自己
    node->prev = node;
  }
  // 将[first, last)内的所有元素移动到position之前
  void transfer(iterator position, iterator first, iterator last);

 public:
  // 构造函数
  list() { empty_initialize(); }
  ~list() {
    if (node) {
      clear();
    }
  }
  // 操作
  iterator begin() { return node->next; }
  const_iterator begin() const { return node->next; }
  iterator end() { return node; }
  const_iterator end() const { return node; }
  // reverse
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }
  // const
  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }
  const_reverse_iterator crbegin() const { return rbegin(); }
  const_reverse_iterator crend() const { return rend(); }

  bool empty() const { return node->next == node; }
  size_type size() const {
    size_type result = 0;
    result = distance(begin(), end());
    return result;
  }
  size_type max_size() const { return size_type(-1); }

  reference front() { return *begin(); }
  const_reference front() const { return *begin(); }
  reference back() { return *(--end()); }
  const_reference back() const { return *(--end()); }
  // 在迭代器position处插入一个节点内容为x
  iterator insert(iterator position, const T& x);
  // 插入一个节点为尾节点
  void push_back(const T& x) { insert(end(), x); }
  // 插入一个节点为头节点
  void push_front(const T& x) { insert(begin(), x); }

  // 移除迭代器 position所指节点
  iterator erase(iterator position);
  // 移除头节点
  void pop_front() { erase(begin()); }
  // 移除尾节点
  void pop_back() {
    iterator tmp = end();
    erase(--tmp);
  }
  // 清除所有节点
  void clear();
  // 将数值为value 的所有元素删除
  void remove(const T& value);
  // 移除数值相同的连续元素，只有连续且相同的元素会被删为一个
  void unique();
  // 将x接合于position所指位置之前，x必须不同与*this
  void splice(iterator postiton, list& x) {
    if (!x.empty())
      transfer(postiton, x.begin(), x.end());
  }
  // 将i接合于position所指位置之前，position和i可指向同一个list
  void splice(iterator postiton, list&, iterator i) {
    iterator j = i;
    ++j;
    if (postiton == i || postiton == j)
      return;
    transfer(postiton, i, j);
  }
  // 将[first,last)内的所有元素接合与position之前
  // position和[first,last)可指向同一个list；
  // 但position不能位于[first,last)之内
  void splice(iterator postiton, list&, iterator first, iterator last) {
    if (first != last)
      transfer(postiton, first, last);
  }
  // merge()将x合并到*this身上，两个list的内容必须经过sort递增排序
  void merge(list& x);
  // reverse()将*this的内容逆向重置
  void reverse();
  // sort() 排序
  void sort();
  // 交换两个list
  void swap(list& x) {
    link_type tmp = x.node;
    x.node = (this->node);
    (this->node) = tmp;
  }
};
// 在迭代器position处插入一个节点内容为x
template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator position,
                                                         const T& x) {
  link_type tmp = create_node(x);  // 产生一个节点，设为x
  // 调整指针，插入tmp
  tmp->next = position.node;
  tmp->prev = position.node->prev;
  (link_type(position.node->prev))->next = tmp;
  position.node->prev = tmp;
  return tmp;
}
// 移除迭代器 position所指节点
template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator position) {
  link_type next_node = link_type(position.node->next);
  link_type prev_node = link_type(position.node->prev);
  prev_node->next = next_node;
  next_node->prev = prev_node;
  destroy_node(position.node);
  return iterator(next_node);
}
// 清除所有节点
template <class T, class Alloc>
void list<T, Alloc>::clear() {
  link_type cur = (link_type)node->next;
  while (cur != node) {  // 遍历每一个节点
    link_type tmp = cur;
    cur = (link_type)cur->next;
    destroy_node(tmp);  // 销毁
  }
  // 恢复原始
  node->next = node;
  node->prev = node;
}
// 将数值为value 的所有元素删除
template <class T, class Alloc>
void list<T, Alloc>::remove(const T& value) {
  iterator first = begin();
  iterator last = end();
  while (first != last) {  // 遍历
    iterator next = first;
    ++next;
    if (*first == value)  // 找到就移除
      erase(first);
    first = next;
  }
}
// 移除数值相同的连续元素，只有连续且相同的元素会被删为一个
template <class T, class Alloc>
void list<T, Alloc>::unique() {
  iterator first = begin();
  iterator last = end();
  if (first == last)  // 空链表，什么都不做
    return;
  iterator next = first;
  while (++next != last) {  // 遍历
    if (*first == *next)    // 如果相同，移除
      erase(next);
    else
      first = next;  // 调整指针
    next = first;    // 修正区段范围
  }
}
// 将[first, last)内的所有元素移动到position之前
template <class T, class Alloc>
void list<T, Alloc>::transfer(iterator position,
                              iterator first,
                              iterator last) {
  if (position != last) {
    (*(link_type((*last.node).prev))).next = position.node;
    (*(link_type((*first.node).prev))).next = last.node;
    (*(link_type((*position.node).prev))).next = first.node;
    link_type tmp = link_type((*position.node).prev);
    (*position.node).prev = (*last.node).prev;
    (*last.node).prev = (*first.node).prev;
    (*first.node).prev = tmp;
  }
}
// merge()将x合并到*this身上，两个list的内容必须经过sort递增排序
template <class T, class Alloc>
void list<T, Alloc>::merge(list<T, Alloc>& x) {
  iterator first1 = begin();
  iterator last1 = end();
  iterator first2 = x.begin();
  iterator last2 = x.end();
  while (first1 != last1 && first2 != last2) {
    if (*first2 < *first1) {
      iterator next = first2;
      transfer(first1, first2, ++next);
      first2 = next;
    } else
      ++first1;
  }
  if (first2 != last2)
    transfer(last1, first2, last2);
}
// reverse()将*this的内容逆向重置
template <class T, class Alloc>
void list<T, Alloc>::reverse() {
  // 如果链表为空，或只有一个元素，不进行
  if (node->next == node || link_type(node->next)->next == node)
    return;
  iterator first = begin();
  ++first;
  while (first != end()) {
    iterator old = first;
    ++first;
    transfer(begin(), old, first);
  }
}
// sort() 排序
template <class T, class Alloc>
void list<T, Alloc>::sort() {
  // 如果链表为空，或只有一个元素，不进行
  if (node->next == node || link_type(node->next)->next == node)
    return;
  // 一些新的lists，作为中介数据
  list<T, Alloc> carry;
  list<T, Alloc> counter[64];
  int fill = 0;
  while (!empty()) {
    carry.splice(carry.begin(), *this, begin());
    int i = 0;
    while (i < fill && !counter[i].empty()) {
      counter[i].merge(carry);
      carry.swap(counter[i++]);
    }
    carry.swap(counter[i]);
    if (i == fill)
      ++fill;
  }
  for (int i = 1; i < fill; ++i)
    counter[i].merge(counter[i - 1]);
  swap(counter[fill - 1]);
}

_MINISTL_END

#endif