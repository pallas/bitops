#ifndef POPCOUNT_H
#define POPCOUNT_H

#include <climits>
#include <stdint.h>

#include "bits.h"
#include "bitlog.h"
#include "builtin_popcount.h"
#include "cast_to_unsigned.h"
#include "magic_bits.h"

template <typename T>
inline bits_t count_null(T t) {
  return 0;
}

template <typename T>
inline bits_t count_mask(T t) {
  unsigned count = 0;

  for (bits_t i = 0 ; i < bits<T>::size ; ++i)
    if (t & (T(1)<<i))
      ++count;

  return count;
}

template <typename T>
inline bits_t count_shift_mask(T t) {
  bits_t count = 0;
  T m = 1;
  for (bits_t i = 0 ; i < bits<T>::size ; ++i, m<<=1)
    if (t & m)
      ++count;

  return count;
}

template <typename T>
inline bits_t count_shift(T t) {
  bits_t count = 0;

  for (bits_t i = 0 ; i < bits<T>::size ; ++i, t>>=1)
    count += t&1;

  return count;
}

template <typename T>
inline bits_t count_and_shear(T t) {
  bits_t count = 0;

  while (t) {
    t &= (t-1);
    ++count;
  }

  return count;
}

template <typename T>
inline bits_t count_array_lookup(T t) {
  bits_t count = 0;

  const static bits_t a[] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
  };

  const bits_t * d = reinterpret_cast<const bits_t *>(&t);
  for (size_t i = 0 ; i < sizeof(t) ; ++i)
    count += a[*(d++)];

  return count;
}

template <typename T>
inline bits_t count_parallel(T t) {
  typedef typename cast_to_unsigned<T>::type UT;
  for (bits_t i = 0 ; i < bitlog<T>::n ; ++i) {
    const bits_t shift = 1<<i;
    const UT mask = magic_mask<UT>(i);
    t = (t & mask) + ((t>>shift) & mask);
  }
  return t;
}

typedef uint8_t count_optimized_shift_t;
typedef uint64_t count_optimized_mask_t;

template <typename T, count_optimized_shift_t S, count_optimized_mask_t M,
          bool elide = false>
struct count_optimized_counter {
  static inline T count(const T t) { return (t&M) + ((t>>S)&M); }
};

template <typename T, count_optimized_shift_t S, count_optimized_mask_t M>
struct count_optimized_counter<T,S,M,true> {
  static inline T count(const T t) { return t; }
};

template <typename T, count_optimized_shift_t S, count_optimized_mask_t M>
static inline T count_optimized_helper(const T t) {
  return count_optimized_counter<T,S,M,(sizeof(t)*CHAR_BIT<=S)>::count(t);
}

template <typename T>
inline bits_t count_optimized(T t) {
  typedef typename cast_to_unsigned<T>::type UT;

  t = count_optimized_helper<T,  1, UT(0x5555555555555555ULL)>(t);
  t = count_optimized_helper<T,  2, UT(0x3333333333333333ULL)>(t);
  t = count_optimized_helper<T,  4, UT(0x0f0f0f0f0f0f0f0fULL)>(t);
  t = count_optimized_helper<T,  8, UT(0x00ff00ff00ff00ffULL)>(t);
  t = count_optimized_helper<T, 16, UT(0x0000ffff0000ffffULL)>(t);
  t = count_optimized_helper<T, 32, UT(0x00000000ffffffffULL)>(t);
  t = count_optimized_helper<T, 64, UT(0xffffffffffffffffULL)>(t);

  return t;
}

template <typename T>
inline bits_t count_intrinsic(T t) { return popcount<T>(t); }

#endif//POPCOUNT_H
