#ifndef CAST_TO_UNSIGNED_H
#define CAST_TO_UNSIGNED_H

#include <stdint.h>

template <typename T> struct cast_to_unsigned { };

template <> struct cast_to_unsigned<int8_t> { typedef uint8_t type; };
template <> struct cast_to_unsigned<int16_t> { typedef uint16_t type; };
template <> struct cast_to_unsigned<int32_t> { typedef uint32_t type; };
template <> struct cast_to_unsigned<int64_t> { typedef uint64_t type; };

template <> struct cast_to_unsigned<uint8_t> { typedef uint8_t type; };
template <> struct cast_to_unsigned<uint16_t> { typedef uint16_t type; };
template <> struct cast_to_unsigned<uint32_t> { typedef uint32_t type; };
template <> struct cast_to_unsigned<uint64_t> { typedef uint64_t type; };


#endif//CAST_TO_UNSIGNED_H