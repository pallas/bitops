#ifndef BITLOG_H
#define BITLOG_H

#include <stdint.h>
#include "bits.h"

template <typename T> struct bitlog { };

template <> struct bitlog<int8_t>  { static const bits_t n = 3; };
template <> struct bitlog<int16_t> { static const bits_t n = 4; };
template <> struct bitlog<int32_t> { static const bits_t n = 5; };
template <> struct bitlog<int64_t> { static const bits_t n = 6; };

template <> struct bitlog<uint8_t>  { static const bits_t n = 3; };
template <> struct bitlog<uint16_t> { static const bits_t n = 4; };
template <> struct bitlog<uint32_t> { static const bits_t n = 5; };
template <> struct bitlog<uint64_t> { static const bits_t n = 6; };

#endif//BITLOG_H
