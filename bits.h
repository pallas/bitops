#ifndef BITS_H
#define BITS_H

#include <stdint.h>

typedef uint_fast8_t bits_t;

template <typename T>
struct bits {
  static const bits_t size = sizeof(T) * CHAR_BIT;
};

#endif//BITS_H
