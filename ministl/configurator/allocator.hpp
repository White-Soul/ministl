#pragma once

#ifndef MINISTL_ALLOCATOR_H
#define MINISTL_ALLOCATOR_H
#include "../utils/util.hpp"
#include "alloc.hpp"
#include "construct.hpp"
#include "uninitialized.h"

_MINISTL_BEGIN

template <class T, class Alloc>
class allocator {
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  static T* allocate(size_t n) {
    return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
  }
  static T* allocate(void) { return (T*)Alloc::allocate(sizeof(T)); }
  static void deallocate(T* p, size_t n) {
    if (0 != n)
      Alloc::deallocate(p, n * sizeof(T));
  }
  static void deallocate(T* p) { Alloc::deallocate(p, sizeof(T)); }
};

_MINISTL_END

#endif