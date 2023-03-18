#ifndef MINISTL_RB_TREE_H
#define MINISTL_RB_TREE_H

#include "../configurator/allocator.hpp"
#include "../iterator/iterator.hpp"
#include "../utils/util.hpp"

_MINISTL_BEGIN

typedef bool _rb_tree_color_type;
const _rb_tree_color_type _rb_tree_red = false;
const _rb_tree_color_type _rb_tree_black = true;

// 节点属性
struct _rb_tree_node_base {
  typedef _rb_tree_color_type color_type;
  typedef _rb_tree_node_base* base_ptr;

  color_type color;  // 节点颜色
  base_ptr parent;   // 父节点
  base_ptr left;     // 左节点
  base_ptr right;    // 右节点

  static base_ptr minimum(base_ptr x) {
    while (x->left != 0)
      x = x->left;  // 一直向左走就会找到最小值
    return x;
  }
  static base_ptr maximum(base_ptr x) {
    while (x->right != 0)
      x = x->right;
    return x;
  }
};
// 二叉树节点
template <class Value>
struct _rb_tree_node : public _rb_tree_node_base {
  typedef _rb_tree_node<Value>* link_type;
  Value value_field;  // 节点值
};
template <class Value, class Ref, class Ptr>
struct _rb_tree_const_iterator;
// 基层迭代器
struct _rb_tree_base_iterator {
  typedef _rb_tree_node_base::base_ptr base_ptr;
  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;

  base_ptr node;  // 它用来与容器产生一个链接
  // operator++
  void increment() {
    if (node->right != 0) {       // 如果有右子节点，情况1
      node = node->right;         // 向右走
      while (node->left != 0)     // 然后一直往左走
        node = node->left;        // 结果
    } else {                      // 如果没有右子节点，情况2
      base_ptr y = node->parent;  // 找出父节点
      while (node == y->right) {   // 如果现行节点本身是个右节点
        node = y;  // 就一直上溯，直到不是右节点为止
        y = y->parent;
      }
      if (node->right != y) {  // 若此时右子节点不等于此时的父节点
        node = y;              // 状况3，此时的父节点为解答
                               // 否则，此时node为解答，情况4
      }
    }
  }
  // operator--
  void decrement() {
    if (node->color == _rb_tree_red &&
        node->parent->parent == node)  // 如果是红节点且父节点的父节点等于自己
      node = node->right;         // 状况1，右节点即为结果
    else if (node->left != 0) {   // 如果有左节点，状况2
      base_ptr y = node->left;    // 令y指向左子节点
      while (y->right != 0)       // 当y有右子节点
        y = y->right;             // 一直往右子节点
      node = y;                   // 最后是结果
    } else {                      // 即非根节点，亦无左子节点
      base_ptr y = node->parent;  // 状况3，找出父节点
      while (node == y->left) {   // 当现行节点身为左子节点
        node = y;                 // 一直交替往上走，直到现行节点
        y = y->parent;            // 不为左子节点
      }
      node = y;  // 此时父节点即是答案
    }
  }

};
// RB-tree 的正向迭代器
template <class Value, class Ref, class Ptr>
struct _rb_tree_iterator : public _rb_tree_base_iterator {
  typedef Value value_type;
  typedef Ref reference;
  typedef Ptr pointer;
  typedef _rb_tree_iterator<Value, Value&, Value*> iterator;
  typedef _rb_tree_const_iterator<Value, Value&, Value*> const_iterator;
  typedef _rb_tree_iterator<Value, Ref, Ptr> self;
  typedef _rb_tree_node<Value>* link_type;

  _rb_tree_iterator() {}
  _rb_tree_iterator(link_type x) { node = x; }
  _rb_tree_iterator(const iterator& it) { node = it.node; }

  reference operator*() const { return link_type(node)->value_field; }
  pointer operator->() const { return &(operator*()); }

  self& operator++() {
    increment();
    return *this;
  }
  self operator++(int) {
    self tmp = *this;
    increment();
    return tmp;
  }

  self& operator--() {
    decrement();
    return *this;
  }
  self operator--(int) {
    self tmp = *this;
    decrement();
    return tmp;
  }

  bool operator!=(const iterator& s){
    return this->node != s.node;
  }
  bool operator==(const iterator& s){
    return this->node == s.node;
  }
};
// rb-tree 的const迭代器
template <class Value, class Ref, class Ptr>
struct _rb_tree_const_iterator : public _rb_tree_base_iterator {
  typedef Value value_type;
  typedef const Ref reference;
  typedef const Ptr pointer;
  typedef _rb_tree_iterator<Value, Value&, Value*> iterator;
  typedef _rb_tree_const_iterator<Value, Value&, Value*> const_iterator;
  typedef const_iterator self;
  typedef _rb_tree_node<Value>* link_type;

  _rb_tree_const_iterator() {}
  _rb_tree_const_iterator(link_type x) { node = x; }
  _rb_tree_const_iterator(const iterator& it) { node = it.node; }
  _rb_tree_const_iterator(const const_iterator& it) { node = it.node; }

  reference operator*() const { return link_type(node)->value_field; }
  pointer operator->() const { return &(operator*()); }

  self& operator++() {
    increment();
    return *this;
  }
  self operator++(int) {
    self tmp = *this;
    increment();
    return tmp;
  }

  self& operator--() {
    decrement();
    return *this;
  }
  self operator--(int) {
    self tmp = *this;
    decrement();
    return tmp;
  }
};

// rb_tree
template <class Key,
          class Value,
          class KeyOfValue,
          class Compare,
          class Alloc = alloc>
class rb_tree {
 protected:
  typedef void* void_pointer;
  typedef _rb_tree_node_base* base_ptr;
  typedef _rb_tree_node<Value> rb_tree_node;
  typedef allocator<rb_tree_node, Alloc> rb_tree_node_allocator;
  typedef _rb_tree_color_type color_type;

 public:
  typedef Key key_type;
  typedef Value value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef rb_tree_node* link_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

 protected:
  link_type get_node() { return rb_tree_node_allocator::allocate(); }
  void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

  link_type create_node(const value_type& x) {
    link_type tmp = get_node();  // 配置空间
    try {
      construct(&tmp->value_field, x);  // 构造空间
    } catch (...) {
      put_node(tmp);
    }
    return tmp;
  }

  link_type clone_node(link_type x) {  // 复制一个节点的值和颜色
    link_type tmp = create_node(x->value_field);
    tmp->color = x->color;
    tmp->left = 0;
    tmp->right = 0;
    return tmp;
  }

  void destroy_node(link_type p) {
    destroy(&p->value_field);
    put_node(p);
  }

 protected:
  // RB-tree 只能以三笔数据表现
  size_type node_count;  // 追踪记录数的大小
  link_type header;
  Compare key_compare;  // 节点间的键值大小比较
  // 取得header成员
  link_type& root() const { return (link_type&)header->parent; }
  link_type& leftmost() const { return (link_type&)header->left; }
  link_type& rightmost() const { return (link_type&)header->right; }
  // 取得x的成员
  static link_type& left(link_type x) { return (link_type&)(x->left); }
  static link_type& right(link_type x) { return (link_type&)(x->right); }
  static link_type& parent(link_type x) { return (link_type&)(x->parent); }
  static reference value(link_type x) { return x->value_field; }
  static const Key& key(link_type x) { return KeyOfValue()(value(x)); }
  static color_type& color(link_type x) { return (color_type&)(x->color); }
  //==============================
  static link_type& left(base_ptr x) { return (link_type&)(x->left); }
  static link_type& right(base_ptr x) { return (link_type&)(x->right); }
  static link_type& parent(base_ptr x) { return (link_type&)(x->parent); }
  static reference value(base_ptr x) { return ((link_type)x)->value_field; }
  static const Key& key(base_ptr x) {
    return KeyOfValue()(value(link_type(x)));
  }
  static color_type& color(base_ptr x) { return (color_type&)(x->color); }
  // 求极大值和极小值
  static link_type minimun(link_type x) {
    return (link_type)_rb_tree_node_base::minimum(x);
  }
  static link_type maximum(link_type x) {
    return (link_type)_rb_tree_node_base::maximum(x);
  }

 public:
  typedef _rb_tree_iterator<value_type, reference, pointer> iterator;  // 迭代器
  typedef _rb_tree_const_iterator<value_type, reference, pointer>
      const_iterator;                                            // 迭代器
  typedef ministl::reverse_iterator<iterator> reverse_iterator;  // 反向迭代器
  typedef ministl::reverse_iterator<const_iterator>
      const_reverse_iterator;  // 反向迭代器
 private:
  iterator _insert(base_ptr x, base_ptr y, const value_type& v);
  void _erase(link_type x);
  link_type _copy(link_type x, link_type p);
  void _clear(link_type x) {
    while (x != 0) {
      _erase(right(x));
      link_type y = left(x);
      destroy_node(x);
      x = y;
    }
  }
  void init() {
    header = get_node();
    color(header) = _rb_tree_red;  // 令header为红色，用来区分header
    root() = 0;
    leftmost() = header;
    rightmost() = header;
  }

 public:
  rb_tree(const Compare& comp = Compare()) : node_count(0), key_compare(comp) {
    init();
  }
  rb_tree(const rb_tree& x) : node_count(0), key_compare(x.key_compare) {
    header = get_node();  // 產生一個節點空間，令 header 指向它
    color(header) = _rb_tree_red;  // 令 header 為紅色
    if (x.root() == 0) {           //  如果 x 是個空白樹
      root() = 0;
      leftmost() = header;   // 令 header 的左子節點為自己。
      rightmost() = header;  // 令 header 的右子節點為自己。
    } else {                 //  x 不是一個空白樹
      try {
        root() = _copy(x.root(), header);  // 调用copy函数
      } catch (...) {
        put_node(header);
      }
      leftmost() = minimum(root());  // 令 header 的左子節點為最小節點
      rightmost() = maximum(root());  // 令 header 的右子節點為最大節點
    }
    node_count = x.node_count;
  }
  ~rb_tree() {
    clear();
    put_node(header);
  }
  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& operator=(
      const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x) {
    iterator first = x.begin();
    iterator last = x.end();
    while (first != last) {
      insert_equal(*first);
      first++;
    }
  }

 public:
  Compare key_comp() const { return key_compare; }
  // 迭代器
  iterator begin() { return leftmost(); }  // RB树的起点是最左节点处
  const_iterator begin() const { return begin(); }
  const_iterator cbegin() const { return begin(); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }  // 反向
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator crbegin() const { return rbegin(); }

  iterator end() { return header; }  // 终点为header处
  const_iterator end() const { return end(); }
  const_iterator cend() const { return end(); }
  reverse_iterator rend() { return reverse_iterator(begin()); }  // 反向
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }  // 反向
  const_reverse_iterator crend() const { return rend(); }

  bool empty() const { return node_count == 0; }
  size_type size() const { return node_count; }
  size_type max_size() const { return size_type(-1); }
  void swap(rb_tree& x) {
    rb_tree tmp = x;
    x = *this;
    *this = tmp;
  }

 public:
  // 将x插入rb-tree中（节点独一无二)
  pair<iterator, bool> insert_unique(const value_type& x);
  iterator insert_unique(iterator pos, const value_type& x) {
    return _insert(pos.node, pos.node->parent, x);
  }
  template <class Iter>
  void insert_unique(Iter first, Iter last) {
    while (first < last) {
      insert_unique(*first);
      first++;
    }
  }
  // 将x插入rb-tree中（允许节点重复）
  iterator insert_equal(const value_type& x);
  // clear
  void clear() {
    if (node_count != 0) {
      _clear(root());
      leftmost() = header;
      root() = 0;
      rightmost() = header;
      node_count = 0;
    }
  }
  // erase
  size_type erase(const link_type& x) {
    _erase(x);
    return node_count;
  }
  void erase(iterator pos) { _erase(*pos); }
  void erase(iterator first, iterator last) {
    if ((last - first) == node_count)
      clear();
    else {
      while (first != last) {
        erase(*first);
        first++;
      }
    }
  }
  // find
  iterator find(const Key& k);
  // 计算键值为x的节点的个数
  size_type count(const Key& x);
  // 提供了查询与某个键值相等的节点迭代器范围
  pair<iterator, iterator> equal_range(const Key& x);
  // 返回不小于k的第一个节点迭代器
  iterator lower_bound(const Key& x);
  // 返回大于k的第一个节点迭代器
  iterator upper_bound(const Key& x);
};

// 计算RBTree的节点值为k的节点个数
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const Key& k) {
  pair<const_iterator, const_iterator> p = equal_range(k);
  size_type n = 0;
  distance(p.first, p.second, n);
  return n;
}
// 查询与键值k相等的节点迭代器范围
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
            typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const Key& k) {
  return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}
// 返回不小于k的第一个节点迭代器
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key& k) {
  link_type y = header; /* Last node which is not less than k. */
  link_type x = root(); /* Current node. */

  while (x != 0)
    if (!key_compare(key(x), k))
      y = x, x = left(x);
    else
      x = right(x);

  return iterator(y);
}
// 返回大于k的第一个节点迭代器
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key& k) {
  link_type y = header; /* Last node which is greater than k. */
  link_type x = root(); /* Current node. */

  while (x != 0)
    if (key_compare(k, key(x)))
      y = x, x = left(x);
    else
      x = right(x);

  return iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(
    const value_type& v) {
  link_type y = header;
  link_type x = root();  // 从根节点开始
  while (x != 0) {
    y = x;
    x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
    // 遇到大的则往左走，小于等于往右走
  }
  return _insert(x, y, v);
  // x为新值插入点，y为插入点父节点，v为新值
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(
    const value_type& v) {
  link_type y = header;
  link_type x = root();
  bool comp = true;
  while (x != 0) {
    y = x;
    comp = key_compare(KeyOfValue()(v), key(x));  // v小于目前节点
    x = comp ? left(x) : right(x);  // 大于的则往左走，小于等于往右走
  }
  // 离开之后，y所指就是插入点的父节点
  iterator j = iterator(y);
  if (comp)  // 如果离开while时comp为真（表示遇到大的，插入左侧）
    if (j == begin())  // 如果插入节点的父节点为最左节点
      return pair<iterator, bool>(_insert(x, y, v), true);
    // x为新值插入点，y为插入点父节点，v为新值
    else    // 否则
      --j;  // 调整j
  if (key_compare(key(j.node), KeyOfValue()(v)))
    // 小于新值，插入右侧
    return pair<iterator, bool>(_insert(x, y, v), true);
  // x为新值插入点，y为插入点父节点，v为新值
  // 到此，新值一定与树种键值重复，那么就不插入
  return pair<iterator, bool>(j, false);
}

// 真正插入
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_insert(base_ptr x_,
                                                         base_ptr y_,
                                                         const value_type& v) {
  link_type x = (link_type)x_;
  link_type y = (link_type)y_;
  link_type z;
  // key_compare是键值大小比较
  if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y))) {
    z = create_node(v);
    left(y) = z;  // 这使得当y为header时，leftmost = z
    if (y == header) {
      root() = z;
      rightmost() = z;
    } else if (y == leftmost())  // 如果y为最左节点
      leftmost() = z;            // 维护leftmost，使它永远指向最左
  } else {
    z = create_node(v);
    right(y) = z;  // 令新节点成为插入节点父节点的右子节点
    if (y == rightmost())
      rightmost() = z;  // 维护rightmost，使它永远指向最右
  }
  parent(z) = y;  // 设定新节点的父节点
  left(z) = 0;
  right(z) = 0;
  // 颜色调整
  _rb_tree_rebalance(z, header->parent);  // 参数一为新增节点，参数2为root
  ++node_count;
  return iterator(z);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) {
  link_type y = header;
  link_type x = root();

  while (x != 0) {
    if (!key_compare(key(x), k))
      y = x, x = left(x);
    else
      x = right(x);
  }
  iterator j = iterator(y);
  return (j == end() || key_compare(k, key(j.node))) ? end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_erase(link_type x) {
  if (x->left == 0 && x->right == 0) {
    if (x->color == _rb_tree_red) {  // 组合1，删除红色叶节点，直接删除
      if (leftmost() == x)
        leftmost() = (link_type)x->parent;
      else if (rightmost() == x)
        rightmost() = (link_type)x->parent;
    } else {              // 组合2，最麻烦的情况
      if (root() == x) {  // 删除根节点
        root() = 0;
        leftmost() = header;
        rightmost() = header;
      } else {  // 否则调用处理函数，此时x一定有兄弟节点
        _erase_rebalance(x, header->parent);

        if (leftmost() == x)
          leftmost() = (link_type)x->parent;
        if (rightmost() == x)
          rightmost() = (link_type)x->parent;
      }
    }
    if (x->parent->left == x)
      x->parent->left = 0;
    else
      x->parent->right = 0;
    destroy_node(x);
    --node_count;
  } else if (
      x->left ==
      0) {  // 组合4，x是黑节点，它的右孩子是红节点（因为组合3不可能存在）
    link_type son = (link_type)x->right;
    son->parent = x->parent;
    son->color = _rb_tree_black;

    if (x->parent->left == x)
      x->parent->left = son;
    else
      x->parent->right = son;

    if (root() == x) {  // 如果删除的是根节点，要调整
      root() = son;
    }
    if (leftmost() == x)
      leftmost() = son;
    destroy_node(x);
    --node_count;
  } else if (
      x->right ==
      0) {  // 一定是组合4：x是黑节点，它的左孩子是红节点（因为组合3不可能存在）
    link_type son = (link_type)x->left;
    son->parent = x->parent;
    son->color = _rb_tree_black;

    if (x->parent->left == x)
      x->parent->left = son;
    else
      x->parent->right = son;

    if (root() == x) {
      root() = son;
    }
    destroy_node(x);
    --node_count;
  } else {
    iterator it = iterator(x);
    ++it;
    x->value_field = *it;

    _erase((link_type)it.node);
  }
}

template <class K, class V, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<K, V, KeyOfValue, Compare, Alloc>::link_type
rb_tree<K, V, KeyOfValue, Compare, Alloc>::_copy(link_type x, link_type p) {
  link_type top = clone_node(x);  // 克隆root节点
  top->parent = p;                // 将root节点父节点指向p
                                  // 以下为非递归的二叉树复制过程
  try {
    if (x->right)
      top->right = __copy(right(x), top);  // 一直copy右子节点
    p = top;
    x = left(x);  // 取左节点

    while (x != 0) {                // 左子节点不为空
      link_type y = clone_node(x);  // 克隆左子节点
      p->left = y;                  // p的左子节点设为y
      y->parent = p;                // y的父节点设为p
      if (x->right)  // 如果左子节点还有右子节点，继续复制
        y->right = __copy(right(x), y);
      p = y;  // 直到没有左子节点
      x = left(x);
    }
  } catch (...) {
    _erase(top);
  };

  return top;
}

// 全局函数，用于树平衡

inline void _rb_tree_rotate_left(_rb_tree_node_base* x,
                                 _rb_tree_node_base*& root);
inline void _rb_tree_rotate_right(_rb_tree_node_base* x,
                                  _rb_tree_node_base*& root);
//===============================
inline void _erase_rebalance(_rb_tree_node_base* x, _rb_tree_node_base*& root) {
  _rb_tree_node_base* brother;
  if (x == x->parent->left)
    brother = x->parent->right;
  else
    brother = x->parent->left;
  if (brother->color == _rb_tree_black) {
    if ((brother->left && brother->left->color == _rb_tree_red) ||
        (brother->right && brother->right->color == _rb_tree_red)) {
      if (brother == x->parent->right) {
        if (brother->right != 0) {
          _rb_tree_rotate_left(x->parent, root);
          brother->right->color = _rb_tree_black;
          brother->color = x->parent->color;
          x->parent->color = _rb_tree_black;
        } else {
          _rb_tree_rotate_right(brother, root);
          brother->color = _rb_tree_red;
          brother->parent->color = _rb_tree_black;

          _erase_rebalance(x, root);
        }
      } else {
        if (brother->left != 0) {
          _rb_tree_rotate_right(x->parent, root);
          brother->left->color = _rb_tree_black;
          brother->color = x->parent->color;
          x->parent->color = _rb_tree_black;
        } else {
          _rb_tree_rotate_left(brother, root);
          brother->color = _rb_tree_red;
          brother->parent->color = _rb_tree_black;

          _erase_rebalance(x, root);
        }
      }
    } else {
      if (x == root)
        return;
      if (x->parent->color == _rb_tree_red) {
        x->parent->color = _rb_tree_black;
        brother->color = _rb_tree_red;
      } else {
        brother->color = _rb_tree_red;
        _erase_rebalance(x->parent, root);
      }
    }
  } else {
    if (brother == x->parent->right) {
      _rb_tree_rotate_left(x->parent, root);
    } else {
      _rb_tree_rotate_right(x->parent, root);
    }
    brother->color = _rb_tree_black;
    x->parent->color = _rb_tree_red;

    _erase_rebalance(x, root);
  }
}
// ==============================

inline void _rb_tree_rebalance(_rb_tree_node_base* x,
                               _rb_tree_node_base*& root) {
  x->color = _rb_tree_red;                                 // 新节点必为红
  while (x != root && x->parent->color == _rb_tree_red) {  // 父节点为红
    if (x->parent == x->parent->parent->left) {  // 父节点为祖父节点左子节点
      _rb_tree_node_base* y = x->parent->parent->right;  // 令y为伯父节点
      if (y && y->color == _rb_tree_red) {  // 伯父节点存在，且为红
        x->parent->color = _rb_tree_black;  // 更改父节点为黑
        y->color = _rb_tree_black;          // 更改伯父节点为黑
        x->parent->parent->color = _rb_tree_red;  // 祖父节点为红
        x = x->parent->parent;
      } else {                        // 伯父节点不存在，或为黑
        if (x == x->parent->right) {  // 如果新节点为父节点的右节点
          x = x->parent;
          _rb_tree_rotate_left(x, root);  // 第一参数为左旋点
        }
        x->parent->color = _rb_tree_black;
        x->parent->parent->color = _rb_tree_red;
        _rb_tree_rotate_right(x->parent->parent, root);  // 第一参数为右旋点
      }
    } else {  // 父节点为祖父节点右节点
      _rb_tree_node_base* y = x->parent->parent->left;  // 令y为伯父节点
      if (y && y->color == _rb_tree_red) {  // 伯父节点存在，且为红
        x->parent->color = _rb_tree_red;    // 更改父节点为黑
        y->color = _rb_tree_red;            // 更改伯父节点为黑
        x->parent->parent->color = _rb_tree_red;  // 祖父节点为红
        x = x->parent->parent;                    // 准备继续往上检查
      } else {                       // 伯父节点不存在，或为黑
        if (x == x->parent->left) {  // 如果新节点为父节点的左节点
          x = x->parent;
          _rb_tree_rotate_right(x, root);  // 第一参数为右旋点
        }
        x->parent->color = _rb_tree_black;
        x->parent->parent->color = _rb_tree_red;
        _rb_tree_rotate_left(x->parent->parent, root);  // 第一参数为左旋点
      }
    }
  }                              // while end
  root->color = _rb_tree_black;  // 根节点永远为黑
}

inline void _rb_tree_rotate_left(_rb_tree_node_base* x,
                                 _rb_tree_node_base*& root) {
  _rb_tree_node_base* y = x->right;  // y为旋转点的右节点
  x->right = y->left;
  if (y->left != 0)
    y->left->parent = x;
  y->parent = x->parent;

  // 令y完全替代x的位置
  if (x == root)  // x为根节点
    root = y;
  else if (x == x->parent->left)  // x为其父节点的左子节点
    x->parent->left = y;
  else  // x为其父节点的右子节点
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

inline void _rb_tree_rotate_right(_rb_tree_node_base* x,
                                  _rb_tree_node_base*& root) {
  _rb_tree_node_base* y = x->left;  // y为旋转点的左子节点
  x->left = y->right;
  if (y->right != 0)
    y->right->parent = x;
  y->parent = x->parent;

  if (x == root)
    root = y;
  else if (x == x->parent->right)
    x->parent->right = y;
  else
    x->parent->left = y;
  y->right = x;
  x->parent = y;
}

_MINISTL_END

#endif