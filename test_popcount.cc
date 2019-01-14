#include <cstdlib>
#include <cassert>

#include <string>
#include <iostream>

#include <unistd.h>

#include <late/timer.h>
#include <lace/random.h>
#include <lace/singleton.h>
#include "popcount.h"

lace::random & rng = lace::singleton<lace::random>::instance();

template <typename T, unsigned char (*F)(T)>
float do_test(unsigned t) {
  const unsigned iterations = 1e6;
  unsigned operations = 0;

  late::timer_process pt;
  do {
    ++operations;
    for (unsigned i = 0 ; i < iterations ; ++i)
      volatile bits_t x = F(rng.l());
  } while (!pt.alarm((time_t)t));

  return (operations * iterations) / pt.elapsed();
};

int
main(int, char*[]) {

  for (unsigned i = 0 ; i < 4096 ; ++i) {
    long x = rng.l();
    assert(count_mask(x) == count_shift_mask(x));
    assert(count_shift_mask(x) == count_shift(x));
    assert(count_shift(x) == count_and_shear(x));
    assert(count_and_shear(x) == count_array_lookup(x));
    assert(count_array_lookup(x) == count_parallel(x));
    assert(count_parallel(x) == count_intrinsic(x));
    assert(count_intrinsic(x) == count_optimized(x));
  }

#define TEST(f) do { \
    std::cout << #f << std::flush \
              << '\t' << unsigned(do_test<uint8_t,f>(1)) << std::flush \
              << '\t' << unsigned(do_test<uint16_t,f>(1)) << std::flush \
              << '\t' << unsigned(do_test<uint32_t,f>(1)) << std::flush \
              << '\t' << unsigned(do_test<uint64_t,f>(1)) << std::flush \
              << std::endl; \
  } while(0)

TEST(count_null);
TEST(count_mask);
TEST(count_shift_mask);
TEST(count_shift);
TEST(count_and_shear);
TEST(count_array_lookup);
TEST(count_parallel);
TEST(count_intrinsic);
TEST(count_optimized);

#undef TEST

  return EXIT_SUCCESS;
}
//
