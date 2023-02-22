
#pragma once

#ifndef MINISTL_ALLOC_H
#define MINISTL_ALLOC_H
#include <cstdlib>

#if 0
#include <new>
#define _THROW_BAD_ALLOC throw std::bad_alloc
#elif !defined(_THROW_BAR_ALLOC)
#include <iostream>
#define _THROW_BAD_ALLOC                     \
  std::cerr << "out of memory" << std::endl; \
  exit(1)
#endif
#define _NODE_ALLOCATOR_THREADS 1

_MINISTL_BEGIN

template <int inst>
class _malloc_alloc_template;
template <bool threads, int inst>
class _default_alloc_template;
typedef _malloc_alloc_template<0> malloc_alloc;
#ifdef _USE_MALLOC
typedef malloc_alloc alloc;
#else
typedef _default_alloc_template<_NODE_ALLOCATOR_THREADS, 0> alloc;
#endif
// 一级配置器
template <int inst>
class _malloc_alloc_template {
 private:
  static void* oom_malloc(size_t);
  static void* oom_realloc(void*, size_t);
  static void (*_malloc_alloc_oom_handler)();

 public:
  // 第一级配置器使用melloc/realloc和free
  static void* allocate(size_t n) {
    void* result = malloc(n);
    if (0 == result)
      result = oom_malloc(n);
    return result;
  }
  static void deallocate(void* p, size_t) { free(p); }

  static void* reallocate(void* p, size_t, size_t new_sz) {
    void* result = realloc(p, new_sz);
    if (0 == result)
      result = oom_realloc(p, new_sz);
    return result;
  }
  static void (*set_malloc_handler(void (*f)()))() {
    void (*old)() = _malloc_alloc_oom_handler;
    _malloc_alloc_oom_handler = f;
    return (old);
  }
};

template <int inst>
void (*_malloc_alloc_template<inst>::_malloc_alloc_oom_handler)() = 0;

template <int inst>
void* _malloc_alloc_template<inst>::oom_malloc(size_t n) {
  void (*my_malloc_handler)();
  void* result;
  for (;;) {
    my_malloc_handler = _malloc_alloc_oom_handler;
    if (0 == my_malloc_handler) {
      _THROW_BAD_ALLOC;
    }
    (*my_malloc_handler)();
    result = malloc(n);
    if (result)
      return result;
  }
}

template <int inst>
void* _malloc_alloc_template<inst>::oom_realloc(void* p, size_t n) {
  void (*my_malloc_handler)();
  void* result;
  for (;;) {
    my_malloc_handler = _malloc_alloc_oom_handler;
    if (0 == my_malloc_handler) {
      _THROW_BAD_ALLOC;
    }
    (*my_malloc_handler)();
    result = realloc(p, n);
    if (result)
      return result;
  }
}

enum { _ALIGN = 8 };        // 小型区块的上调边界
enum { _MAX_BYTES = 128 };  // 小型区块的上限
enum {
  _NFREELISTS = _MAX_BYTES / _ALIGN
};  // free-lists个数
    // 二级配置器
template <bool threads, int inst>
class _default_alloc_template {
 private:
  // 将bytes上调至8的倍数
  static size_t ROUND_UP(size_t bytes) {
    return (((bytes) + _ALIGN - 1) & ~(_ALIGN - 1));
  }

 private:
  union obj {  // free_lists的节点构造
    union obj* free_list_link;
    char client_data[1];
  };

 private:
  // 16个free-lists
  static obj* volatile free_list[_NFREELISTS];
  // 根据区块大小，决定使用n号free_list，n从1起算
  static size_t FREELIST_INDEX(size_t bytes) {
    return (((bytes) + _ALIGN - 1) / _ALIGN - 1);
  }
  // 返回一个大小为n的对象，并可能加入大小为n的其他区块到free_list
  static void* refill(size_t n);
  // 配置一大块空间，可以容纳nobjs个大小为size的区块
  // 如果配置nobjs个区块有所不便，nobjs可能会降低
  static char* chunk_alloc(size_t size, int& nobjs);

  // chunk allocation state
  static char* start_free;  // 内存池起始位置，只在chunk_alloc中变化
  static char* end_free;  // 内存池结束位置，只在chunk_alloc中变化
  static size_t heap_size;

 public:
  static void* allocate(size_t n);
  static void deallocate(void* p, size_t n);
  static void* reallocate(void* p, size_t old_sz, size_t new_sz);
};

// 下面是static data member的定义与初值设置
template <bool threads, int inst>
char* _default_alloc_template<threads, inst>::start_free = 0;

template <bool threads, int inst>
char* _default_alloc_template<threads, inst>::end_free = 0;

template <bool threads, int inst>
size_t _default_alloc_template<threads, inst>::heap_size = 0;

template <bool threads, int inst>
typename _default_alloc_template<threads, inst>::
    obj* volatile _default_alloc_template<threads,
                                          inst>::free_list[_NFREELISTS] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <bool threads, int inst>
void* _default_alloc_template<threads, inst>::allocate(size_t n) {
  obj* volatile* my_free_list;
  obj* result;
  // n> 128 就调用第一级配置器
  if (n > (size_t)_MAX_BYTES) {
    return (malloc_alloc::allocate(n));
  }
  // 寻找16个freelist中适当的一个
  my_free_list = free_list + FREELIST_INDEX(n);
  result = *my_free_list;
  if (result == 0) {
    // 没找到可用的freelist，准备重新填充freelist
    void* r = refill(ROUND_UP(n));
    return r;
  }
  // 调整freelist
  *my_free_list = result->free_list_link;
  return (result);
}

template <bool threads, int inst>
void _default_alloc_template<threads, inst>::deallocate(void* p, size_t n) {
  obj* q = (obj*)p;
  obj* volatile* my_free_list;
  // n> 128 就调用第一级配置器
  if (n > (size_t)_MAX_BYTES) {
    malloc_alloc::deallocate(p, n);
    return;
  }
  // 寻找对应的freelist
  my_free_list = free_list + FREELIST_INDEX(n);
  q->free_list_link = *my_free_list;
  *my_free_list = q;
}
// 返回一个大小为n的对象，并有时候会为适当的freelist增加节点
template <bool threads, int inst>
void* _default_alloc_template<threads, inst>::refill(size_t n) {
  int nobjs = 20;
  // 调用chunk_alloc，尝试取得nobjs个区块作为freelist的节点
  char* chunk = chunk_alloc(n, nobjs);
  obj* volatile* my_free_list;
  obj* result;
  obj *current_obj, *next_obj;
  int i;
  // 如果只获得一个区块，这个区块就分配给调用者，freelist无新节点
  if (1 == nobjs)
    return (chunk);
  // 否则，准备调整freelist，纳入新节点
  my_free_list = free_list + FREELIST_INDEX(n);
  // 以下在chunk空间内建立freelist
  result = (obj*)chunk;  // 这一块准备返回给客端
  // 以下引导freelist指向新配置的空间，取自内存池
  *my_free_list = next_obj = (obj*)(chunk + n);
  // 以下将freelist的各节点串联起来
  for (i = 1;; i++)  // 从1开始，第0个返回给客端
  {
    current_obj = next_obj;
    next_obj = (obj*)((char*)next_obj + n);
    if (nobjs - 1 == i) {
      current_obj->free_list_link = 0;
      break;
    } else {
      current_obj->free_list_link = next_obj;
    }
  }
  return result;
}

template <bool threads, int inst>
char* _default_alloc_template<threads, inst>::chunk_alloc(size_t size,
                                                          int& nobjs) {
  char* result;
  size_t total_bytes = size * nobjs;
  size_t bytes_left = end_free - start_free;  // 内存池剩余空间
  if (bytes_left >= total_bytes) {
    // 内存池剩余空间完全满足需求
    result = start_free;
    start_free += total_bytes;
    return result;
  } else if (bytes_left >= size) {
    // 内存池剩余空间不能完全满足，但够供应一个以上的区块
    nobjs = bytes_left / size;
    total_bytes = size * nobjs;
    result = start_free;
    start_free += total_bytes;
    return result;
  } else {
    // 内存池剩余空间连一个都不能满足
    size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
    // 以下试着让内存池中残余的还有价值
    if (bytes_left > 0) {
      // 内存池还有零头分配给适当的freelist
      // 找出适当的freelist
      obj* volatile* my_free_list = free_list + FREELIST_INDEX(bytes_left);
      // 调整freelist，将内存池中的残余空间编入
      ((obj*)start_free)->free_list_link = *my_free_list;
      *my_free_list = (obj*)start_free;
    }
    // 配置heap空间，用来补充内存池
    start_free = (char*)malloc(bytes_to_get);
    if (0 == start_free) {
      // heap空间不足，malloc分配失败
      int i;
      obj *volatile *my_free_list, *p;
      // 以下搜寻适当的freelist
      for (i = size; i <= _MAX_BYTES; i += _ALIGN) {
        my_free_list = free_list + FREELIST_INDEX(i);
        p = *my_free_list;
        if (0 != p) {  // freelist内尚有未用区块
                       // 调整freelist以释放未用区块
          *my_free_list = p->free_list_link;
          start_free = (char*)p;
          end_free = start_free + i;
          // 递归调用自己，为了修正nobjs
          return (chunk_alloc(size, nobjs));
        }
      }
      end_free = 0;  // 如果出现意外，没内存可用
      // 调用第一级配置器
      start_free = (char*)malloc_alloc::allocate(bytes_to_get);
      // 这会导致抛出异常
    }
    heap_size += bytes_to_get;
    end_free = start_free + bytes_to_get;
    return (chunk_alloc(size, nobjs));
  }
}

_MINISTL_END
#endif