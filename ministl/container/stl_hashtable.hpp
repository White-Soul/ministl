#ifndef MINISTL_HASHTABLE_H
#define MINISTL_HASHTABLE_H
#include "../configurator/allocator.hpp"
#include "../functor/hash_func.hpp"
#include "../iterator/iterator.hpp"
#include "../utils/util.hpp"
#include "stl_vector.hpp"

_MINISTL_BEGIN

template <class Value>
struct _hashtable_node {
  _hashtable_node* next;
  Value val;
};
template <class Value,
          class Key,
          class HashFcn,
          class ExtractKey,
          class EqualKey,
          class Alloc = alloc>
class hashtable;
template <class Value,
          class Key,
          class HashFcn,
          class ExtractKey,
          class EqualKey,
          class Alloc>
struct _hashtable_iterator;
// const 迭代器
template <class Value,
          class Key,
          class HashFcn,
          class ExtractKey,
          class EqualKey,
          class Alloc>
struct _hashtable_const_iterator {
  typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
  typedef _hashtable_const_iterator<Value,
                                    Key,
                                    HashFcn,
                                    ExtractKey,
                                    EqualKey,
                                    Alloc>
      const_iterator;
  typedef _hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
      iterator;
  typedef _hashtable_node<Value> node;
  typedef forward_iterator_tag iterator_category;
  typedef Value value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef const Value& reference;
  typedef const Value* pointer;
  typedef Key key_type;
  node* cur;
  hashtable* ht;
  _hashtable_const_iterator(node* n, hashtable* tab) : cur(n), ht(tab) {}
  _hashtable_const_iterator() {}
  reference operator*() const { return cur->val; }
  pointer operator->() const { return &(operator*()); }
  iterator& operator++();
  iterator operator++(int);
};

template <class Value,
          class Key,
          class HashFcn,
          class ExtractKey,
          class EqualKey,
          class Alloc>
typename _hashtable_const_iterator<Value,
                                   Key,
                                   HashFcn,
                                   ExtractKey,
                                   EqualKey,
                                   Alloc>::iterator&
_hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
operator++() {
  const node* old = cur;
  cur = cur->next;
  if (!cur) {
    size_type bucket = ht->bkt_num(old->val);
    while (!cur && ++bucket < ht->buckets.size())
      cur = ht->buckets[bucket];
  }
  return *this;
}

template <class Value,
          class Key,
          class HashFcn,
          class ExtractKey,
          class EqualKey,
          class Alloc>
typename _hashtable_const_iterator<Value,
                                   Key,
                                   HashFcn,
                                   ExtractKey,
                                   EqualKey,
                                   Alloc>::iterator
_hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
operator++(int) {
  iterator tmp = *this;
  ++*this;
  return tmp;
}

// 迭代器
template <class Value,
          class Key,
          class HashFcn,
          class ExtractKey,
          class EqualKey,
          class Alloc>
struct _hashtable_iterator {
  typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
  typedef _hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
      iterator;
  typedef _hashtable_const_iterator<Value,
                                    Key,
                                    HashFcn,
                                    ExtractKey,
                                    EqualKey,
                                    Alloc>
      const_iterator;
  typedef _hashtable_node<Value> node;
  typedef forward_iterator_tag iterator_category;
  typedef Value value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef Value& reference;
  typedef Value* pointer;
  typedef Key key_type;

  node* cur;
  hashtable* ht;
  _hashtable_iterator(node* n, hashtable* tab) : cur(n), ht(tab) {}
  _hashtable_iterator() {}
  reference operator*() const { return cur->val; }
  pointer operator->() const { return &(operator*()); }
  iterator& operator++();
  iterator operator++(int);
  bool operator==(const iterator& it) const { return cur == it.cur; }
  bool operator!=(const iterator& it) const { return cur != it.cur; }
};

template <class Value,
          class Key,
          class HashFcn,
          class ExtractKey,
          class EqualKey,
          class Alloc>
typename _hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    iterator&
    _hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    operator++() {
  const node* old = cur;
  cur = cur->next;
  if (!cur) {
    size_type bucket = ht->bkt_num(old->val);
    while (!cur && ++bucket < ht->buckets.size())
      cur = ht->buckets[bucket];
  }
  return *this;
}

template <class Value,
          class Key,
          class HashFcn,
          class ExtractKey,
          class EqualKey,
          class Alloc>
typename _hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    iterator
    _hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::
    operator++(int) {
  iterator tmp = *this;
  ++*this;
  return tmp;
}
// hashtable定义
template <class Value,       // 节点的实值类型
          class Key,         // 节点键值型别
          class HashFcn,     // hash function 的函数型别
          class ExtractKey,  // 从节点中取出键值方法（仿函数）
          class EqualKey,    // 判断键值是否相同（仿函数）
          class Alloc>
class hashtable {
 public:
  typedef HashFcn hasher;
  typedef EqualKey key_equal;
  typedef size_t size_type;
  typedef _hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
      iterator;
  typedef _hashtable_const_iterator<Value,
                                    Key,
                                    HashFcn,
                                    ExtractKey,
                                    EqualKey,
                                    Alloc>
      const_iterator;
  typedef ministl::reverse_iterator<iterator> reverse_iterator;
  typedef ministl::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef typename iterator::iterator_category iterator_category;
  typedef typename const_iterator::pointer const_pointer;
  typedef typename const_iterator::reference const_reference;
  typedef typename iterator::value_type value_type;
  typedef typename iterator::pointer pointer;
  typedef typename iterator::reference reference;
  typedef typename iterator::difference_type difference_type;
  typedef typename iterator::key_type key_type;

 private:
  /// 以下三者都是function object
  hasher hash;
  key_equal equals;
  ExtractKey get_key;

  typedef _hashtable_node<Value> node;
  typedef allocator<node, Alloc> node_allocator;

  vector<node*, Alloc> buckets;
  size_type num_elements;

 public:
  size_type bucket_count() const { return buckets.size(); }
  // 构造函数
  hashtable(size_type n, const HashFcn& hf, const EqualKey& eql)
      : hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0) {
    initialize_buckets(n);
  }
  // 析构函数
  ~hashtable() { clear(); }
  size_type max_bucket_count() const {
    return _ministl_prime_list[_ministl_num_primes - 1];
  }
  // 插入元素，不重复
  pair<iterator, bool> insert_unique(const value_type& obj) {
    resize(num_elements + 1);
    return insert_unique_noresize(obj);
  }
  // 以下函数判断是否需要重建表格
  void resize(size_type num_elements_hint);
  // 删除
  void clear();
  // copy
  void copy_from(const hashtable& ht);
  // find
  iterator find(const key_type& key) {
    size_type n = bkt_num_key(key);
    node* first;
    for (first = buckets[n]; first && !equals(get_key(first->val), key);
         first = first->next) {
    }
    return iterator(first, this);
  }
  // count
  size_type count(const key_type& key) const {
    const size_type n = bkt_num_key(key);
    size_type result = 0;
    for (const node* cur = buckets[n]; cur; cur = cur->next) {
      if (equals(get_key(cur->val), key))
        ++result;
    }
    return result;
  }

 private:
  node* new_node(const value_type& obj) {
    node* n = node_allocator::allocate();
    n->next = 0;
    try {
      construct(&n->val, obj);
      return n;
    } catch (...) {
      node_allocator::deallocate(n);
    }
  }

  void delete_node(node* n) {
    destroy(&n->val);
    node_allocator::deallocate(n);
  }

  void initialize_buckets(size_type n) {
    const size_type n_bucktes = next_size(n);
    buckets.reserve(n_bucktes);
    buckets.insert(buckets.end(), n_bucktes, (node*)0);
    num_elements = 0;
  }

  size_type next_size(size_type n) const { return _ministl_next_prime(n); }

  pair<iterator, bool> insert_unique_noresize(const value_type& obj);

  size_type bkt_num(const value_type& obj, size_t n) const {
    return bkt_num_key(get_key(obj), n);
  }
  size_type bkt_num(const value_type& obj) const {
    return bkt_num_key(get_key(obj));
  }
  size_type bkt_num_key(const key_type& key) const {
    return bkt_num_key(key, buckets.size());
  }
  size_type bkt_num_key(const key_type& key, size_t n) const {
    return hash(key) % n;
  }
};

template <class V, class K, class HF, class Ex, class Eq, class A>
void hashtable<V, K, HF, Ex, Eq, A>::resize(size_type num_elements_hint) {
  const size_type old_n = buckets.size();
  if (num_elements_hint > old_n) {  // 确定需要配置
    const size_type n = next_size(num_elements_hint);
    if (n > old_n) {
      vector<node*, A> tmp(n, (node*)0);
      try {
        for (size_type bucket = 0; bucket < old_n; ++bucket) {
          node* first = buckets[bucket];  // 指向节点所对应之串行的起始节点
          while (first) {  // 处理每一个旧的bucket所含(串行)的每一个节点
            size_type new_bucket = bkt_num(first->val, n);
            // 1.令旧bucket指向其所对应之串行的下一个节点
            buckets[bucket] = first->next;
            // 2.3.将当前节点插入到新bucket内，成为其对应串行的第一个节点
            first->next = tmp[new_bucket];
            // 4. 回到旧bucket所指的待处理串行
            first = buckets[bucket];
          }
        }
        buckets.swap(tmp);
      } catch (...) {
      }
    }
  }
}

template <class V, class K, class HF, class Ex, class Eq, class A>
pair<typename hashtable<V, K, HF, Ex, Eq, A>::iterator, bool>
hashtable<V, K, HF, Ex, Eq, A>::insert_unique_noresize(const value_type& obj) {
  const size_type n = bkt_num(obj);  // 决定obj位于 #n bucket
  node* first = buckets[n];  // 令 first指向 bucket对应之链表头部
  // 如果buckets[n] 被占用，此时first不为0， 于是进入循环
  for (node* cur = first; cur; cur = cur->next) {
    if (equals(get_key(cur->val), get_key(obj))) {
      // 如果发现链表中的某键相同，马上插入
      node* tmp = new_node(obj);
      tmp->next = cur->next;
      cur->next = tmp;
      ++num_elements;
      return iterator(tmp, this);  // 返回一个迭代器指向新增节点
    }
    // 如果没有重复
    node* tmp = new_node(obj);
    tmp->next = first;
    buckets[n] = tmp;
    ++num_elements;
    return iterator(tmp, this);
  }
}
template <class V, class K, class HF, class Ex, class Eq, class A>
void hashtable<V, K, HF, Ex, Eq, A>::clear() {
  for (size_type i = 0; i < buckets.size(); ++i) {
    node* cur = buckets[i];
    while (cur != 0) {
      node* next = cur->next;
      delete_node(cur);
      cur = next;
    }
    buckets[i] = nullptr;
  }
  num_elements = 0;
}
template <class V, class K, class HF, class Ex, class Eq, class A>
void hashtable<V, K, HF, Ex, Eq, A>::copy_from(const hashtable& ht) {
  buckets.clear();
  buckets.reserve(ht.buckets.size());
  buckets.insert(buckets.end(), ht.buckets.size(), (node*)0);
  try {
    for (size_type i = 0; i < ht.buckets.size(); ++i) {
      if (const node* cur = ht.buckets[i]) {
        node* copy = new_node(cur->val);
        buckets[i] = copy;

        for (node* next = cur->next; next; cur = next, next = cur->next) {
          copy->next = new_node(next->val);
          copy = copy->next;
        }
      }
    }
    num_elements = ht.num_elements;
  } catch (...) {
    clear();
  }
}

_MINISTL_END

#endif
