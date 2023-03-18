#ifndef MINISTL_VECTOR_H
#define MINISTL_VECTOR_H

#include "../algorithm/stl_algorithm.hpp"
#include "../configurator/allocator.hpp"
#include "../iterator/iterator.hpp"
#include "../utils/exceptdef.hpp"
#include "../utils/util.hpp"

_MINISTL_BEGIN

template <class T, class Alloc = alloc>
class vector {
 protected:
  typedef allocator<T, Alloc> data_allocator;

 public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef value_type* iterator;
  typedef const value_type* const_iterator;
  typedef ministl::reverse_iterator<iterator> reverse_iterator;
  typedef ministl::reverse_iterator<const_iterator> const_reverse_iterator;

 protected:
  iterator start;           // 目前使用的空间头
  iterator finish;          // 目前使用的空间尾
  iterator end_of_storage;  // 目前可用的空间尾

  void insert_aux(iterator position, const T& x);
  void deallocate() {
    if (start)
      data_allocator::deallocate(start, end_of_storage - start);
  }
  void fill_initialize(size_type n, const T& value) {
    start = allocate_and_fill(n, value);
    finish = start + n;
    end_of_storage = finish;
  }
  template <class Iter>
  void range_init(Iter first, Iter last);

 public:
  // 构造函数
  vector() : start(0), finish(0), end_of_storage(0) {}
  vector(size_type n, const T& value) { fill_initialize(n, value); }
  explicit vector(size_type n) { fill_initialize(n, T()); }
  vector(const vector& v) { range_init(v.start, v.finish); }
  vector(vector&& v) noexcept
      : start(v.start), finish(v.finish), end_of_storage(v.end_of_storage) {
    v.start = 0;
    v.finish = 0;
    v.end_of_storage = 0;
  }
  vector& operator=(const vector& v) {
    if (this != v) {
      const auto len = v.size();
      if (len > capacity()) {
        vector tmp(v.begin(), v.end());
        swap(tmp);
      } else if (size() >= len) {
        auto i = std::copy(v.begin(), v.end());
        data_allocator::destroy(i, finish);
        finish = start + len;
      } else {
        std::copy(v.begin(), v.begin() + size(), start);
        uninitialized_copy(v.begin() + size(), v.end(), finish);
        end_of_storage = finish = start + len;
      }
    }
    return *this;
  }
  ~vector() {
    destroy(start, finish);
    deallocate();
  }
  // 迭代器
  iterator begin() { return start; }
  const_iterator begin() const { return start; }
  iterator end() { return finish; }
  const_iterator end() const { return finish; }
  // r迭代器
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }
  // const迭代器
  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }
  const_reverse_iterator crbegin() const { return rbegin(); }
  const_reverse_iterator crend() const { return rend(); }
  // 容量
  size_type max_size() const { return size_type(-1) / sizeof(T); }
  size_type size() const { return size_type(end() - begin()); }
  size_type capacity() const { return size_type(end_of_storage - begin()); }
  bool empty() const { return begin() == end(); }
  // 访问操作
  reference operator[](size_type n) {
    THROW_OUT_OF_RANGE_IF(n >= size(), "range out");
    return *(begin() + n);
  }
  const_reference operator[](size_type n) const {
    THROW_OUT_OF_RANGE_IF(n >= size(), "range out");
    return *(begin() + n);
  }
  reference at(size_type n) {
    MINISTL_DEBUE(n < size());
    return (*this)[n];
  }
  const_reference at(size_type n) const {
    MINISTL_DEBUE(n < size());
    return (*this)[n];
  }

  reference front() {
    MINISTL_DEBUE(!empty());
    return *begin();
  }
  const_reference front() const {
    MINISTL_DEBUE(!empty());
    return *begin();
  }
  reference back() {
    MINISTL_DEBUE(!empty());
    return *(end() - 1);
  }
  const_reference back() const {
    MINISTL_DEBUE(!empty());
    return *(end() - 1);
  }

  // 将元素插入尾部
  void push_back(const T& x) {
    if (finish != end_of_storage) {
      construct(finish, x);
      ++finish;
    } else {
      insert_aux(end(), x);
    }
  }
  // 去除尾端元素
  void pop_back() {
    --finish;
    destroy(finish);
  }
  // 清除某位置上的元素
  iterator erase(iterator position) {
    if (position + 1 != end()) {
      std::copy(position + 1, finish, position);
    }
    --finish;
    destroy(finish);
    return position;
  }
  // 重置大小
  void resize(size_type new_size, const T& x) {
    if (new_size < size()) {
      erase(begin() + new_size, end());
    } else {
      insert(end(), new_size - size(), x);
    }
  }
  void resize(size_type new_size) { resize(new_size, T()); }
  void clear() { erase(begin(), end()); }
  void insert(iterator position, size_type n, const T& x);
  void insert(iterator position, const T& x) { insert(position, 1, x); }
  void swap(vector& v) {
    if (this != &v) {
      std::swap(start, v.start);
      std::swap(finish, v.finish);
      std::swap(end_of_storage, v.end_of_storage);
    }
  }
  template <class Iter>
  void insert(const_iterator pos, Iter first, Iter last);
  void reserve(size_type n);

 protected:
  iterator allocate_and_fill(size_type n, const T& x) {
    iterator result = data_allocator::allocate(n);
    uninitialized_fill_n(result, n, x);
    return result;
  }
};
template <class T, class Alloc>
void vector<T, Alloc>::reserve(size_type n) {
  if (capacity() < n) {
    auto old_size = size();
    auto tmp = data_allocator::allocate(n);
    uninitialized_copy(start, finish, tmp);
    destroy(start, finish);
    data_allocator::deallocate(start, end_of_storage - start);
    start = tmp;
    finish = tmp + old_size;
    end_of_storage = start + n;
  }
}

template <class T, class Alloc>
template <class Iter>
void vector<T, Alloc>::insert(const_iterator position, Iter first, Iter last) {
  MINISTL_DEBUE(position >= begin() && position <= end() && !(last < first));
  iterator pos = const_cast<iterator>(position);
  if (first == last)
    return;
  const auto n = distance(first, last);
  if ((end_of_storage - finish) >= n) {  // 如果备用空间大小足够
    const auto after_elems = finish - pos;
    auto old_end = finish;
    if (after_elems > n) {
      finish = uninitialized_copy(finish - n, finish, finish);
      std::move_backward(pos, old_end - n, old_end);
      uninitialized_copy(first, last, pos);
    } else {
      auto mid = first;
      advance(mid, after_elems);
      finish = uninitialized_copy(mid, last, finish);
      finish = uninitialized_move(pos, old_end, finish);
      uninitialized_copy(first, mid, pos);
    }
  } else {  // 备用空间不足
    const auto new_size = n != 0 ? 2 * n : 1;
    auto new_begin = data_allocator::allocate(new_size);
    auto new_end = new_begin;
    try {
      new_end = uninitialized_move(start, pos, new_begin);
      new_end = uninitialized_copy(first, last, new_end);
      new_end = uninitialized_move(pos, finish, new_end);
    } catch (...) {
      destroy(new_begin, new_end);
      data_allocator::deallocate(new_begin, n);
      throw;
    }
    data_allocator::deallocate(start, end_of_storage - finish);
    start = new_begin;
    finish = new_end;
    end_of_storage = start + new_size;
  }
}

template <class T, class Alloc>
template <class Iter>
void vector<T, Alloc>::range_init(Iter first, Iter last) {
  size_type len = distance(first, last);
  size_type init_size = std::max(len, size_type(16));
  try {
    start = data_allocator::allocate(init_size);
    finish = start + len;
    end_of_storage = start + init_size;
  } catch (...) {
    start = 0;
    finish = 0;
    end_of_storage = 0;
    throw;
  }
  uninitialized_copy(first, last, start);
}

template <class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& x) {
  if (finish != end_of_storage) {  // 还有备用空间
    // 在备用空间起始处构造一个元素，并以vector最后一个元素值为其初值
    construct(finish, *(finish - 1));
    // 调整水位
    ++finish;
    T x_copy = x;
    std::copy_backward(position, finish - 2, finish - 1);
    *position = x_copy;
  } else {  // 已无备用空间
    const size_type old_size = size();
    const size_type len = old_size != 0 ? 2 * old_size : 1;
    // 以上配置原则：如果原大小为0，则配置1；
    // 如果原大小不为0，则配置为原两倍大小，
    // 前半段用来放数据，后半段放新数据
    iterator new_start = data_allocator::allocate(len);  // 实际配置
    iterator new_finish = new_start;
    try {
      // 将原vector的内容拷贝到新vector
      new_finish = uninitialized_copy(start, position, new_start);
      // 为新元素设初值
      construct(new_finish, x);
      // 调整水位
      ++new_finish;
      // 将原vector的备用空间中的内容也拷贝过来
      new_finish = uninitialized_copy(position, finish, new_finish);
    } catch (...) {
      destroy(new_start, new_finish);
      data_allocator::deallocate(new_start, len);
      throw;
    }
    // 析构并释放原vector
    destroy(begin(), end());
    deallocate();

    // 调整迭代器，指向新vector
    start = new_start;
    finish = new_finish;
    end_of_storage = new_start + len;
  }
}

template <class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& x) {
  if (n != 0) {  // 当n != 0，才进行以下操作
    if (size_type(end_of_storage - finish) >= n) {
      // 备用空间大于 新增元素个数
      T x_copy = x;
      // 以下计算插入点之后的现有元素个数
      const size_type elems_after = finish - position;
      iterator old_finish = finish;
      if (elems_after > n) {
        // 插入点之后的现有元素个数 大于 新增元素个数
        uninitialized_copy(finish - n, finish, finish);
        finish += n;  // 将vector尾端标记后移
        std::copy_backward(position, old_finish - n, old_finish);
        std::fill(position, position + n, x_copy);  // 从插入点开始填入新值
      } else {
        /// 插入点之后的现有元素个数 小于等于 新增元素个数
        uninitialized_fill_n(finish, n - elems_after, x_copy);
        finish += n - elems_after;
        uninitialized_copy(position, old_finish, finish);
        finish += elems_after;
        std::fill(position, old_finish, x_copy);
      }
    } else {
      // 备用空间 小于 新增元素个数
      // 首先决定新长度：旧长度的2倍，或旧长度+新增元素个数
      const size_type old_size = size();
      const size_type len = old_size + std::max(old_size, n);
      // 配置新的vector空间
      iterator new_start = data_allocator::allocate(len);
      iterator new_finish = new_start;
      try {
        // 将旧的vector的插入点之前的元素复制到新空间
        new_finish = uninitialized_copy(start, position, new_start);
        // 将新增元素填入新空间
        new_finish = uninitialized_fill_n(new_finish, n, x);
        // 将旧的vector插入点之后的元素复制到新空间
        new_finish = uninitialized_copy(position, finish, new_finish);
      }
      // #ifdef _STL_USE_EXCEPTIONS
      catch (...) {
        // 如果有异常发生，
        destroy(new_start, new_finish);
        data_allocator::deallocate(new_start, len);
        throw;
      }
      // #endif
      destroy(start, finish);
      deallocate();
      start = new_start;
      finish = new_finish;
      end_of_storage = new_start + len;
    }
  }
}
/*****************************************************************************************/
// 重载比较操作符

template <class T>
bool operator==(const vector<T>& lhs, const vector<T>& rhs) {
  return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T>
bool operator<(const vector<T>& lhs, const vector<T>& rhs) {
  return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                 rhs.end());
}

template <class T>
bool operator!=(const vector<T>& lhs, const vector<T>& rhs) {
  return !(lhs == rhs);
}

template <class T>
bool operator>(const vector<T>& lhs, const vector<T>& rhs) {
  return rhs < lhs;
}

template <class T>
bool operator<=(const vector<T>& lhs, const vector<T>& rhs) {
  return !(rhs < lhs);
}

template <class T>
bool operator>=(const vector<T>& lhs, const vector<T>& rhs) {
  return !(lhs < rhs);
}

_MINISTL_END

#endif