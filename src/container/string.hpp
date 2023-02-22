#ifndef MINISTL_STRING_H
#define MINISTL_STRING_H

#include "../utils/util.hpp"
#include "basic_string.hpp"

_MINISTL_BEGIN

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

_MINISTL_END

#endif