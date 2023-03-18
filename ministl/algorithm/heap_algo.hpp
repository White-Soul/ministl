#ifndef MINISTL_HEAP_H
#define MINISTL_HEAP_H

#include "../iterator/iterator.hpp"

_MINISTL_BEGIN
// heap算法
/*=========================================*/
// push_heap    接受两个迭代器，表示vector的头尾，并且新元素已经插入到尾端
/*=========================================*/
template <class RandomAccessIter>
inline void push_heap(RandomAccessIter first, RandomAccessIter last) {
  _push_heap_aux(first, last, distance_type(first), value_type(first));
}
template <class RandomAccessIter, class Distance, class T>
inline void _push_heap_aux(RandomAccessIter first,
                           RandomAccessIter last,
                           Distance*,
                           T*) {
  _push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
}
// 这组push_heap不允许指定 大小比较规则
template <class RandomAccessIter, class Distance, class T>
void _push_heap(RandomAccessIter first,
                Distance holeIndex,
                Distance topIndex,
                T value) {
  Distance parent = (holeIndex - 1) / 2;  // 找出父节点
  while (holeIndex > topIndex && *(first + parent) < value) {
    // 当未达到顶端，且父节点小于新值，（不符合heap的次序特性）
    // 由于以上使用operator<，可知STL heap 是一种大根堆
    // 上浮操作
    *(first + holeIndex) = *(first + parent);
    holeIndex = parent;
    parent = (holeIndex - 1) / 2;
  }
  *(first + holeIndex) = value;
}
/* 重载版本使用函数对象 comp 代替比较操作 */
template <class RandomAccessIter, class Compared>
inline void push_heap(RandomAccessIter first,
                      RandomAccessIter last,
                      Compared comp) {
  _push_heap_aux(first, last, distance_type(first), value_type(first), comp);
}
template <class RandomAccessIter, class Distance, class T, class Compared>
inline void _push_heap_aux(RandomAccessIter first,
                           RandomAccessIter last,
                           Distance*,
                           T*,
                           Compared comp) {
  _push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)),
             comp);
}
template <class RandomAccessIter, class Distance, class T, class Compared>
void _push_heap(RandomAccessIter first,
                Distance holeIndex,
                Distance topIndex,
                T value,
                Compared comp) {
  Distance parent = (holeIndex - 1) / 2;  // 找出父节点
  while (holeIndex > topIndex && comp(*(first + parent), value)) {
    // 当未达到顶端，且父节点小于新值，（不符合heap的次序特性）
    // 由于以上使用operator<，可知STL heap 是一种大根堆
    // 上浮操作
    *(first + holeIndex) = *(first + parent);
    holeIndex = parent;
    parent = (holeIndex - 1) / 2;
  }
  *(first + holeIndex) = value;
}
/*=========================================*/
/*这组pop_heap不允许指定 大小比较规则*/
/*=========================================*/
template <class RandomAccessIter>
inline void pop_heap(RandomAccessIter first, RandomAccessIter last) {
  _pop_heap_aux(first, last, value_type(first));
}
template <class RandomAccessIter, class T>
inline void _pop_heap_aux(RandomAccessIter first, RandomAccessIter last, T*) {
  _pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
  // 以上根据heap的次序特性，pop操作应为底部容器的第一个元素。
  // 因此，首先设定欲调整值为尾值，然后将首值调整至尾节点，然后重整[first，last）
}
template <class RandomAccessIter, class T, class Distance>
inline void _pop_heap(RandomAccessIter first,
                      RandomAccessIter last,
                      RandomAccessIter result,
                      T value,
                      Distance*) {
  *result = *first;  // 设定尾值为首值
  _adjust_heap(first, Distance(0), Distance(last - first), value);
  // 以上重新调整heap, 树根处欲调整值为value（原尾值）
}

/* 重载版本使用函数对象 comp 代替比较操作*/
template <class RandomAccessIter, class Compared>
inline void pop_heap(RandomAccessIter first,
                     RandomAccessIter last,
                     Compared comp) {
  _pop_heap_aux(first, last, value_type(first), comp);
}
template <class RandomAccessIter, class T, class Compared>
inline void _pop_heap_aux(RandomAccessIter first,
                          RandomAccessIter last,
                          T*,
                          Compared comp) {
  _pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first),
            comp);
  // 以上根据heap的次序特性，pop操作应为底部容器的第一个元素。
  // 因此，首先设定欲调整值为尾值，然后将首值调整至尾节点，然后重整[first，last）
}
template <class RandomAccessIter, class T, class Distance, class Compared>
inline void _pop_heap(RandomAccessIter first,
                      RandomAccessIter last,
                      RandomAccessIter result,
                      T value,
                      Distance*,
                      Compared comp) {
  *result = *first;  // 设定尾值为首值
  _adjust_heap(first, Distance(0), Distance(last - first), value, comp);
  // 以上重新调整heap, 树根处欲调整值为value（原尾值）
}
/*=========================================*/
// // 这组_adjust_heap不允许指定 大小比较规则
/*=========================================*/
template <class RandomAccessIter, class Distance, class T>
void _adjust_heap(RandomAccessIter first,
                  Distance holeIndex,
                  Distance len,
                  T value) {
  Distance topIndex = holeIndex;
  Distance secondChild = 2 * holeIndex + 2;  // 右子节点
  while (secondChild < len) {
    // 比较左右两个节点，以secondChild表示最大的
    if (*(first + secondChild) < *(first + (secondChild - 1)))
      secondChild--;
    // 令较大值为根，令根下移至较大子节点处
    *(first + holeIndex) = *(first + secondChild);
    holeIndex = secondChild;
    // 找出新根节点的右子节点
    secondChild = 2 * (secondChild + 1);
  }
  if (secondChild == len) {  // 没有右字节点，只有左子节点
    *(first + holeIndex) = *(first + (secondChild - 1));
    holeIndex = secondChild - 1;
  }
  // 将要调整值放入目前节点内,即
  // *(first+holeIndex) = value
  _push_heap(first, holeIndex, topIndex, value);
}
/*重载版本使用函数对象 comp 代替比较操作*/
template <class RandomAccessIter, class Distance, class T, class Compared>
void _adjust_heap(RandomAccessIter first,
                  Distance holeIndex,
                  Distance len,
                  T value,
                  Compared comp) {
  Distance topIndex = holeIndex;
  Distance secondChild = 2 * holeIndex + 2;  // 右子节点
  while (secondChild < len) {
    // 比较左右两个节点，以secondChild表示最大的
    if (comp(*(first + secondChild), *(first + (secondChild - 1))))
      secondChild--;
    // 令较大值为根，令根下移至较大子节点处
    *(first + holeIndex) = *(first + secondChild);
    holeIndex = secondChild;
    // 找出新根节点的右子节点
    secondChild = 2 * (secondChild + 1);
  }
  if (secondChild == len) {  // 没有右字节点，只有左子节点
    *(first + holeIndex) = *(first + (secondChild - 1));
    holeIndex = secondChild - 1;
  }
  // 将要调整值放入目前节点内,即
  // *(first+holeIndex) = value
  _push_heap(first, holeIndex, topIndex, value, comp);
}
/*=========================================*/
// sort_heap 不允许指定 大小比较标准
/*=========================================*/
template <class RandomAccessIter>
void sort_heap(RandomAccessIter first, RandomAccessIter last) {
  // 以下，没执行一次pop，极大值便被放在尾部
  // 扣除尾部在执行pop，一直下去即可得到排序结果
  while (last - first > 1)
    pop_heap(first, last--);
}
/*重载版本使用函数对象 comp 代替比较操作*/
template <class RandomAccessIter, class Compared>
void sort_heap(RandomAccessIter first, RandomAccessIter last, Compared comp) {
  // 以下，没执行一次pop，极大值便被放在尾部
  // 扣除尾部在执行pop，一直下去即可得到排序结果
  while (last - first > 1)
    pop_heap(first, last--, comp);
}
/*=========================================*/
// make_heap 用来将现有数据转换为一个heap
/*=========================================*/
template <class RandomAccessIter>
inline void make_heap(RandomAccessIter first, RandomAccessIter last) {
  _make_heap(first, last, value_type(first), distance_type(first));
}
// 这组_make_heap不允许指定 大小比较规则
template <class RandomAccessIter, class T, class Distance>
void _make_heap(RandomAccessIter first, RandomAccessIter last, T*, Distance*) {
  if (last - first < 2)
    return;
  Distance len = last - first;
  // 找出第一个需要重新排的子树头部，以parent标识出，
  Distance parent = (len - 2) / 2;

  while (true) {
    // 重排以parent为首的子树
    _adjust_heap(first, parent, len, T(*(first + parent)));
    if (parent == 0)
      return;  // 走完根节点就结束
    parent--;
  }
}

/*重载版本使用函数对象 comp 代替比较操作*/
template <class RandomAccessIter, class Compared>
inline void make_heap(RandomAccessIter first,
                      RandomAccessIter last,
                      Compared comp) {
  _make_heap(first, last, value_type(first), distance_type(first), comp);
}
// 这组_make_heap不允许指定 大小比较规则
template <class RandomAccessIter, class T, class Distance, class Compared>
void _make_heap(RandomAccessIter first,
                RandomAccessIter last,
                T*,
                Distance*,
                Compared comp) {
  if (last - first < 2)
    return;
  Distance len = last - first;
  // 找出第一个需要重新排的子树头部，以parent标识出，
  Distance parent = (len - 2) / 2;

  while (true) {
    // 重排以parent为首的子树
    _adjust_heap(first, parent, len, T(*(first + parent)), comp);
    if (parent == 0)
      return;  // 走完根节点就结束
    parent--;
  }
}

_MINISTL_END

#endif