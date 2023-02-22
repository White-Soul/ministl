#ifndef MINISTL_DEQUE_H
#define MINISTL_DEQUE_H

#include <iostream>

#include "../configurator/allocator.hpp"
#include "../iterator/iterator.hpp"

_MINISTL_BEGIN
template <class T, class Ref, class Ptr, size_t BufSiz>
struct _deque_const_iterator;

template <class T, class Ref, class Ptr, size_t BufSiz>
struct _deque_iterator {  // 未继承iterator
  typedef _deque_iterator<T, T&, T*, BufSiz> iterator;
  typedef _deque_const_iterator<T, T&, T*, BufSiz> const_iterator;
  static size_t buffer_size() { return _deque_buf_size(BufSiz, sizeof(T)); }
  static inline size_t _deque_buf_size(size_t n, size_t sz) {
    return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
  }
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef T* value_pointer;
  typedef Ptr pointer;
  typedef Ref reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T** map_pointer;

  typedef _deque_iterator self;

  // 保持与容器的联结
  value_pointer cur;    // 此迭代器所指缓冲区的现行元素
  value_pointer first;  // 此迭代器所指缓冲区的头
  value_pointer last;  // 此迭代器所指缓冲区的尾部（含备用空间）
  map_pointer node;  // 指向管控中心
  // 跳一个缓冲区
  void set_node(map_pointer new_node) {
    node = new_node;
    first = *new_node;
    last = first + difference_type(buffer_size());
  }
  // 重载运算
  reference operator*() const { return *cur; }
  pointer operator->() const { return &(operator*()); }
  difference_type operator-(const self& x) const {
    return difference_type(buffer_size()) * (node - x.node - 1) +
           (cur - first) + (x.last - x.cur);
  }

  self& operator++() {
    ++cur;
    if (cur == last) {
      set_node(node + 1);
      cur = first;
    }
    return *this;
  }
  self operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
  }
  self& operator--() {
    if (cur == last) {
      set_node(node - 1);
      cur = first;
    }
    --cur;
    return *this;
  }
  self operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
  }
  // 实现随机存取
  self& operator+=(difference_type n) {
    difference_type offset = n + (cur - first);
    if (offset >= 0 && offset < difference_type(buffer_size()))
      // 目标位置在同一缓冲区内
      cur += n;
    else {
      // 目标位置不在同一缓冲区内
      difference_type node_offset =
          offset > 0 ? offset / difference_type(buffer_size())
                     : -difference_type((-offset - 1) / buffer_size()) - 1;
      // 切换至正确的缓冲区
      set_node(node + node_offset);
      // 切换回正确的元素
      cur = first + (offset - node_offset * difference_type(buffer_size()));
    }
    return *this;
  }
  self operator+(difference_type n) const {
    self tmp = *this;
    return tmp += n;  // 调用+=
  }
  // 利用+= 完成-=
  self& operator-=(difference_type n) { return *this += -n; }

  self operator-(difference_type n) const {
    self tmp = *this;
    return tmp -= n;  // 调用-=
  }

  // 随机存取
  reference operator[](difference_type n) const { return *(*this + n); }

  bool operator==(const self& x) const { return cur == x.cur; }
  bool operator!=(const self& x) const { return cur != x.cur; }
  bool operator<(const self& x) const {
    return (node = x.node) ? (cur < x.cur) : (node < x.node);
  }
};
// const 迭代器
template <class T, class Ref, class Ptr, size_t BufSiz>
struct _deque_const_iterator {  // 未继承iterator
  typedef _deque_iterator<T, T&, T*, BufSiz> iterator;
  typedef _deque_const_iterator<T, T&, T*, BufSiz> const_iterator;
  static size_t buffer_size() { return _deque_buf_size(BufSiz, sizeof(T)); }
  static inline size_t _deque_buf_size(size_t n, size_t sz) {
    return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
  }
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef const T* value_pointer;
  typedef const Ptr pointer;
  typedef const Ref reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef const T** map_pointer;

  typedef _deque_const_iterator<T, Ref, Ptr, BufSiz> self;

  // 保持与容器的联结
  value_pointer cur;    // 此迭代器所指缓冲区的现行元素
  value_pointer first;  // 此迭代器所指缓冲区的头
  value_pointer last;  // 此迭代器所指缓冲区的尾部（含备用空间）
  map_pointer node;  // 指向管控中心
  // 跳一个缓冲区
  void set_node(map_pointer new_node) {
    node = new_node;
    first = *new_node;
    last = first + difference_type(buffer_size());
  }
  // 重载运算
  reference operator*() const { return *cur; }
  pointer operator->() const { return &(operator*()); }
  difference_type operator-(const self& x) const {
    return difference_type(buffer_size()) * (node - x.node - 1) +
           (cur - first) + (x.last - x.cur);
  }

  self& operator++() {
    ++cur;
    if (cur == last) {
      set_node(node + 1);
      cur = first;
    }
    return *this;
  }
  self operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
  }
  self& operator--() {
    if (cur == last) {
      set_node(node - 1);
      cur = first;
    }
    --cur;
    return *this;
  }
  self operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
  }
  // 实现随机存取
  self& operator+=(difference_type n) {
    difference_type offset = n + (cur - first);
    if (offset >= 0 && offset < difference_type(buffer_size()))
      // 目标位置在同一缓冲区内
      cur += n;
    else {
      // 目标位置不在同一缓冲区内
      difference_type node_offset =
          offset > 0 ? offset / difference_type(buffer_size())
                     : -difference_type((-offset - 1) / buffer_size()) - 1;
      // 切换至正确的缓冲区
      set_node(node + node_offset);
      // 切换回正确的元素
      cur = first + (offset - node_offset * difference_type(buffer_size()));
    }
    return *this;
  }
  self operator+(difference_type n) const {
    self tmp = *this;
    return tmp += n;  // 调用+=
  }
  // 利用+= 完成-=
  self& operator-=(difference_type n) { return *this += -n; }

  self operator-(difference_type n) const {
    self tmp = *this;
    return tmp -= n;  // 调用-=
  }

  // 随机存取
  reference operator[](difference_type n) const { return *(*this + n); }

  bool operator==(const self& x) const { return cur == x.cur; }
  bool operator!=(const self& x) const { return cur != x.cur; }
  bool operator<(const self& x) const {
    return (node = x.node) ? (cur < x.cur) : (node < x.node);
  }
};

// deque 定义
template <class T, class Alloc = alloc, size_t BufSiz = 0>
class deque {
 public:
  typedef _deque_iterator<T, T&, T*, BufSiz> iterator;
  typedef _deque_const_iterator<T, T&, T*, BufSiz> const_iterator;
  typedef typename iterator::reference reference;
  typedef typename const_iterator::reference const_reference;
  typedef typename iterator::pointer pointer;
  typedef typename const_iterator::pointer const_pointer;
  typedef typename iterator::value_type value_type;
  typedef typename iterator::size_type size_type;
  typedef typename iterator::difference_type difference_type;
  typedef typename iterator::iterator_category iterator_category;

  typedef ministl::reverse_iterator<iterator> reverse_iterator;
  typedef ministl::reverse_iterator<const_iterator> const_reverse_iterator;

 protected:
  // 元素指针的指针
  typedef typename iterator::map_pointer map_pointer;
  // 专属空间配置器，每次配置一个元素大小
  typedef allocator<value_type, Alloc> data_allocator;
  // 专属空间配置器，每次配置一个指针大小
  typedef allocator<pointer, Alloc> map_allocator;

 protected:
  iterator start;   // 第一个节点
  iterator finish;  // 最后一个节点
  map_pointer
      map;  // 指向map，map是块连续空间，每块都是一个指针，指向一块缓冲区
  size_type map_size;  // map内能容纳多少指针

 public:
  bool operator==(const deque& x) const { return map == x.map; }
  // 迭代器操作
  iterator begin() const { return start; }
  const_iterator begin() { return start; }
  iterator end() { return finish; }
  const_iterator end() const { return finish; }
  // r迭代器操作
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }
  // c迭代器操作
  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }
  const_reverse_iterator crbegin() const { return rbegin(); }
  const_reverse_iterator crend() const { return rend(); }

  reference operator[](size_type n) { return start[difference_type(n)]; }
  const_reference operator[](size_type n) const {
    return start[difference_type(n)];
  }
  reference front() { return *start; }
  const_reference front() const { return *start; }
  reference back() {
    iterator tmp = finish;
    --tmp;  // operator--
    return *tmp;
  }
  const_reference back() const {
    iterator tmp = finish;
    --tmp;  // operator--
    return *tmp;
  }

  size_type size() const { return finish - start; }
  size_type max_size() const { return size_type(-1); }
  bool empty() const { return finish == start; }

 protected:
  // 一个map需要管理最少几个节点
  size_type initial_map_size() { return 8; }
  // 负责产生并安排好deque结构,并设置元素初值
  void fill_initialize(size_type n, const value_type& value);
  // 负责安排deque的结构
  void create_map_and_nodes(size_type num_elements);
  // 只有当最后缓冲区已无（或还有一个）元素备用空间时才会调用
  void push_back_aux(const value_type& t);
  // 只有当第一缓冲区已无元素备用空间时才会调用
  void push_front_aux(const value_type& t);
  // 判断map什么时候需要整治
  void reserve_map_at_back(size_type nodes_to_add = 1);
  void reserve_map_at_front(size_type nodes_to_add = 1);
  // 实际进行整治
  void reallocate_map(size_type nodes_to_add, bool add_at_front);
  // 最后缓冲区没有元素
  void pop_back_aux();
  // 第一缓冲区仅有一个元素
  void pop_front_aux();
  // 插入元素
  iterator insert_aux(iterator pos, const value_type& x);

 public:
  // 构造器
  deque() : deque(0, 0) {}
  deque(int n, const value_type& value)
      : start(), finish(), map(0), map_size(0) {
    fill_initialize(n, value);
  }
  // push_back
  void push_back(const value_type& t) {
    // 最后缓冲区有1个以上的备用空间
    if (finish.cur != finish.last - 1) {
      construct(finish.cur, t);  // 直接在备用空间上构造元素
      ++finish.cur;              // 调整最后缓冲区的使用状态
    } else  // 最后缓冲区已无（或还有一个）元素备用空间
      push_back_aux(t);
  }
  // push_front
  void push_front(const value_type& t) {
    if (start.cur != start.first) {
      construct(start.cur - 1, t);
      --start.cur;
    } else
      push_front_aux(t);
  }
  // pop_back
  void pop_back() {
    if (empty())
      throw;
    if (finish.cur != finish.first) {
      --finish.cur;
      destroy(finish.cur);
    } else
      pop_back_aux();
  }
  // pop_front
  void pop_front() {
    if (empty())
      throw;
    if (start.cur != start.last - 1) {
      destroy(start.cur);
      ++start.cur;
    } else
      pop_front_aux();
  }
  // clear清除整个deque，deque的初始状态保有一个缓冲区
  void clear();
  // 清除pos所指向的元素
  iterator erase(iterator pos);
  // 重载 清除[first，last) 区间的元素
  iterator erase(iterator first, iterator last);
  // 在pos前插入一个元素，并设定初始值
  iterator insert(iterator position, const value_type& x);
};
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::fill_initialize(size_type n,
                                               const value_type& value) {
  create_map_and_nodes(n);  // 把deque的结构都生成并安排好
  map_pointer cur;
  try {
    // 为每个节点的缓冲区设定初值
    for (cur = start.node; cur < finish.node; ++cur)
      uninitialized_fill(*cur, *cur + iterator::buffer_size(), value);
    // 最后一个节点的设定不同（因为尾端可能有备用空间，不必设）
    uninitialized_fill(finish.first, finish.cur, value);
  } catch (...) {
    std::cout << "Memory initialization failed";
  }
}
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements) {
  // 需要节点数=（元素个数/每个缓冲区可容纳的元素个数）+1
  // 如果刚好，就多分配一个
  size_type num_nodes = num_elements / iterator::buffer_size() + 1;
  // 一个map要管理几个节点，最少8个，最多是 所需节点数+2
  // （前后各预留一个，扩充实可用）
  map_size = std::max(initial_map_size(), num_nodes + 2);
  map = map_allocator::allocate(map_size);
  // 以上配置出一个具有map_size个节点的map
  // 以下令nstart和nfinish指向map所拥有全部节点的最中间区域
  map_pointer nstart = map + (map_size - num_nodes) / 2;
  map_pointer nfinish = nstart + num_nodes - 1;
  map_pointer cur;
  try {
    // 为map内的每个现用节点配置缓冲区，所有缓冲区加起来就是deque
    // 可用空间
    for (cur = nstart; cur <= nfinish; ++cur)
      *cur = data_allocator::allocate(iterator::buffer_size());
  } catch (...) {
  }

  // 为start和end设置正确内容
  start.set_node(nstart);
  finish.set_node(nfinish);
  start.cur = start.first;
  finish.cur = finish.first + num_elements % iterator::buffer_size();
}
// 当 finish.cur == finish.last-1 才调用
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::push_back_aux(const value_type& t) {
  value_type t_copy = t;
  reserve_map_at_back();
  *(finish.node + 1) = data_allocator::allocate(iterator::buffer_size());
  try {
    construct(finish.cur, t_copy);     // 针对标的元素设置值
    finish.set_node(finish.node + 1);  // 改变finish，令其指向新节点
    finish.cur = finish.first;
  } catch (...) {
    finish.set_node(finish.node - 1);
    finish.cur = finish.last;
    data_allocator::deallocate(*(finish.node + 1), iterator::buffer_size());
  }
}
// 当start.cur == start.first才调用
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::push_front_aux(const value_type& t) {
  value_type t_copy = t;
  reserve_map_at_front();
  *(start.node - 1) = data_allocator::allocate(iterator::buffer_size());
  try {
    start.set_node(start.node - 1);  // 改变start，令其指向新节点
    start.cur = start.last - 1;      // 设定start的状态
    construct(start.cur, t_copy);
  } catch (...) {
    start.set_node(start.node + 1);
    start.cur = start.first;
    data_allocator::deallocate(*(start.node - 1), iterator::buffer_size());
    throw;
  }
}
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::reserve_map_at_back(size_type nodes_to_add) {
  if (nodes_to_add + 1 > map_size - (finish.node - map))
    // 如果map尾部节点备用空间不足
    // 符合以上条件则必须换一个map（配置更大的，拷贝原来，释放原来）
    reallocate_map(nodes_to_add, false);
}
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::reserve_map_at_front(size_type nodes_to_add) {
  if (nodes_to_add > start.node - map)
    // 如果map前端的节点备用空间不足
    // 符合以上条件则必须换一个map（配置更大的，拷贝原来，释放原来）
    reallocate_map(nodes_to_add, true);
}
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add,
                                              bool add_at_front) {
  size_type old_num_nodes = finish.node - start.node + 1;
  size_type new_num_nodes = old_num_nodes + nodes_to_add;

  map_pointer new_nstart;
  if (map_size > 2 * new_num_nodes) {
    new_nstart = map + (map_size - new_num_nodes) / 2 +
                 (add_at_front ? nodes_to_add : 0);
    if (new_nstart < start.node)
      std::copy(start.node, finish.node + 1, new_nstart);
    else
      std::copy_backward(start.node, finish.node + 1,
                         new_nstart + old_num_nodes);
  } else {
    size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;
    // 配置一块空间，给新map使用
    map_pointer new_map = map_allocator::allocate(new_map_size);
    new_nstart = new_map + (new_map_size - new_num_nodes) / 2 +
                 (add_at_front ? nodes_to_add : 0);
    // 把原来的map拷贝过来
    std::copy(start.node, finish.node + 1, new_nstart);
    // 释放原来map
    map_allocator::deallocate(map, map_size);
    // 设定新map
    map = new_map;
    map_size = new_map_size;
  }

  // 重新设定迭代器
  start.set_node(new_nstart);
  finish.set_node(new_nstart + old_num_nodes - 1);
}
// 只有当 finish.cur == finish.first 时会被调用
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::pop_back_aux() {
  data_allocator::deallocate(finish.first);  // 释放最后一个缓冲区
  finish.set_node(finish.node - 1);          // 调整finish
  finish.cur = finish.last - 1;  // 上个缓冲区的最后一个元素
  destroy(finish.cur);           // 将该元素析构
}
// 只有当start.cur == start.last-1时会被调用
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::pop_front_aux() {
  destroy(start.cur);                       // 将该元素析构
  data_allocator::deallocate(start.first);  // 释放第一缓冲区
  start.set_node(start.node + 1);           // 调整finish
  start.cur = start.first;                  // 下个缓冲区的第一个元素
}

// clear清除整个deque，deque的初始状态保有一个缓冲区
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::clear() {
  // 以下针对头尾以外的缓冲区
  for (map_pointer node = start.node + 1; node < finish.node; ++node) {
    // 将元素析构
    destroy(*node, *node + iterator::buffer_size());
    // 释放缓冲区
    data_allocator::deallocate(*node, iterator::buffer_size());
  }
  if (start.node != finish.node) {  // 至少有头尾两个缓冲区
    destroy(start.cur, start.last);  // 将头缓冲区的目前所有元素析构
    destroy(finish.first, finish.cur);  // 将尾缓冲区的目前所有元素析构
    // 以下释放尾部缓冲区，头部保留
    data_allocator::deallocate(finish.first, iterator::buffer_size());
  } else
    // 只有一个缓冲区
    destroy(start.cur, finish.cur);

  finish = start;
}
// 清除pos所指向的元素
template <class T, class Alloc, size_t BufSize>
typename deque<T, Alloc, BufSize>::iterator deque<T, Alloc, BufSize>::erase(
    iterator pos) {
  iterator next = pos;
  ++next;
  difference_type index = pos - start;  // 清除点之前的元素个数
  if (index < (size() >> 1)) {  // 如果清除点之前的元素比较少
    std::copy_backward(start, pos, next);  // 就移动清除点之前的元素
    pop_front();                   // 移动完毕，最前元素冗余，去除
  } else {                         // 清除点之后的元素
    std::copy(next, finish, pos);  // 就移动清除点之后的元素
    pop_back();                    // 移动完毕，最后元素冗余，去除
  }
  return start + index;
}
// 重载 清除[first，last) 区间的元素
template <class T, class Alloc, size_t BufSize>
typename deque<T, Alloc, BufSize>::iterator deque<T, Alloc, BufSize>::erase(
    iterator first,
    iterator last) {
  if (first == start && last == finish) {  // 如果清除区间就是整个deque
    clear();
    return finish;
  } else {
    difference_type n = last - first;              // 清除区间的长度
    difference_type elems_before = first - start;  // 清除区间前方的元素个数
    if (elems_before < (size() - n) / 2) {         // 如果前方元素较少
      std::copy_backward(start, first, last);  // 向后移动前方元素
      iterator new_start = start + n;          // 标记deque的新起点
      destroy(start, new_start);               // 销毁冗余元素
      // 以下将冗余的缓冲区释放
      for (map_pointer cur = start.node; cur < new_start.node; ++cur)
        data_allocator::deallocate(*cur, iterator::buffer_size());
      start = new_start;
    } else {  // 如果后方元素较少
      std::copy(last, finish, first);
      iterator new_finish = finish - n;
      destroy(new_finish, finish);
      for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
        data_allocator::deallocate(*cur, iterator::buffer_size());
      finish = new_finish;
    }
    return start + elems_before;
  }
}
// 在pos前插入一个元素，并设定初始值
template <class T, class Alloc, size_t BufSize>
typename deque<T, Alloc, BufSize>::iterator deque<T, Alloc, BufSize>::insert(
    iterator position,
    const value_type& x) {
  if (position.cur == start.cur) {  // 如果插入点是deque最前端
    push_front(x);
    return start;
  } else if (position.cur == finish.cur) {
    push_back(x);
    iterator tmp = finish;
    --tmp;
    return tmp;
  } else {
    return insert_aux(position, x);
  }
}
template <class T, class Alloc, size_t BufSize>
typename deque<T, Alloc, BufSize>::iterator
deque<T, Alloc, BufSize>::insert_aux(iterator pos, const value_type& x) {
  difference_type index = pos - start;
  value_type x_copy = x;
  if (index < size() / 2) {
    push_front(front());
    iterator front1 = start;
    ++front1;
    iterator front2 = front1;
    ++front2;
    pos = start + index;
    iterator pos1 = pos;
    ++pos1;
    std::copy(front2, pos1, front1);
  } else {
    push_back(back());
    iterator back1 = finish;
    --back1;
    iterator back2 = back1;
    --back2;
    pos = start + index;
    std::copy_backward(pos, back2, back1);
  }
  *pos = x_copy;
  return pos;
}

_MINISTL_END

#endif