#ifndef MINISTL_HASH_FUNC_H
#define MINISTL_HASH_FUNC_H
#include "../utils/util.hpp"

_MINISTL_BEGIN

static const int _ministl_num_primes = 28;
static const unsigned long _ministl_prime_list[_ministl_num_primes] = {
    53,        97,         193,          389,         769,       1543,
    3079,      6151,       12289,        24593,       49157,     98317,
    196613,    393241,     786433,       1572869,     3145739,   6291469,
    12582917,  25165843,   50331653,     100663319,   201326611, 402653189,
    805306457, 1610612741, 3221225473ul, 4294967291ul};

inline unsigned long _ministl_next_prime(unsigned long n) {
  const unsigned long* first = _ministl_prime_list;
  const unsigned long* last = _ministl_prime_list + _ministl_num_primes;
  const unsigned long* pos = std::lower_bound(first, last, n);
  return pos == last ? *(last - 1) : *pos;
}

template <class Key>
struct hash {};

inline size_t _ministl_hash_string(const char* s) {
  unsigned long h = 0;
  for (; *s; ++s)
    h = 5 * h + *s;

  return size_t(h);
}

template <>
struct hash<char*> {
  size_t operator()(const char* s) const { return _ministl_hash_string(s); }
};
template <>
struct hash<const char*> {
  size_t operator()(const char* s) const { return _ministl_hash_string(s); }
};
template <>
struct hash<char> {
  size_t operator()(char x) const { return x; }
};
template <>
struct hash<unsigned char> {
  size_t operator()(unsigned char x) const { return x; }
};
template <>
struct hash<signed char> {
  size_t operator()(unsigned char x) const { return x; }
};
template <>
struct hash<short> {
  size_t operator()(short x) const { return x; }
};
template <>
struct hash<unsigned short> {
  size_t operator()(unsigned short x) const { return x; }
};
template <>
struct hash<int> {
  size_t operator()(int x) const { return x; }
};
template <>
struct hash<unsigned int> {
  size_t operator()(unsigned int x) const { return x; }
};
template <>
struct hash<long> {
  size_t operator()(long x) const { return x; }
};
template <>
struct hash<unsigned long> {
  size_t operator()(unsigned long x) const { return x; }
};

_MINISTL_END

#endif