#ifndef BUILTIN_POPCOUNT_H
#define BUILTIN_POPCOUNT_H

template <typename T> inline int popcount(T x) { return __builtin_popcount(x); };

template <> inline int popcount<unsigned long>(unsigned long x) { return __builtin_popcountl(x); }
template <> inline int popcount<unsigned long long>(unsigned long long x) { return __builtin_popcountll(x); }
template <> inline int popcount<long>(long x) { return __builtin_popcountl(x); }
template <> inline int popcount<long long>(long long x) { return __builtin_popcountll(x); }

#endif//BUILTIN_POPCOUNT_H
