#if !defined(MINISTL_EXCEPTDEF_H)
#define MINISTL_EXCEPTDEF_H

#include <cassert>
#include <stdexcept>
#include "util.hpp"

_MINISTL_BEGIN

#define MINISTL_DEBUE(expr)  assert(expr)

#define THROW_LENGTH_ERROR_IF(expr, what) \
  if ((expr))                             \
  throw std::length_error(what)

#define THROW_OUT_OF_RANGE_IF(expr, what) \
  if ((expr))                             \
  throw std::out_of_range(what)

#define THROW_RUNTINE_ERROR_IF(expr, what) \
  if ((expr))                              \
    throw std::runtime_error(what);

_MINISTL_END

#endif  // MINISTL_EXCEPTDEF_H
