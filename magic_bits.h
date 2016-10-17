#ifndef MAGIC_BITS_H
#define MAGIC_BITS_H

#include "bits.h"

template <typename T> inline T all_ones() { return ~T(0); }

template <typename T>
inline T magic_base(bits_t n) {
  return T(1)<<(bits_t(1)<<n);
}

template <typename T>
inline T magic_mask(bits_t n) {
  T base = magic_base<T>(n);
  typedef int static_assert_base[(base*base-1) ? 1 : -1];
  return all_ones<T>()/T(base*base-1)*T(base-1);
}

#endif//MAGIC_BITS_H
