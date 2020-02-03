# misc_function_dynamic_resolution

[Stackoverflow question page](https://stackoverflow.com/questions/60015568/dynamic-function-resolution-at-runtime)

### Intro
This repo contains a solution for:
```
1. loading multiple modules, each contain multiple functions.
2. functions have interdependence
3. scenario allows one time resolution after all functions are present.  
```
`hash` directory contains a naive solution using hash map.

`ptr` directory contains a solution using one time resolution.

### Build
All libraries are present in `lib`, no need to install boost
```
mkdir build && cd build
cmake ..
make all
```


### Performance:

Benchmark: 1.28e8 possible combinations for a mixed boolean & numeric SAT problem 

Platform: i7 6700HQ, single thread

```
cmake-build-debug/test_ptr  1.63s user 0.01s system 99% cpu 1.641 total
cmake-build-debug/test_hash  10.80s user 0.00s system 99% cpu 10.803 total
```

Hotspot & function runtime from perf:

test_ptr:
```
  53.17%  test_ptr  test_ptr       [.] main
  35.38%  test_ptr  test_ptr       [.] module_1_init(Domain&)::__internal_func_some_circuit::operator()
   8.02%  test_ptr  test_ptr       [.] module_2_init(Domain&)::__internal_func_and_circuit::operator()
   1.90%  test_ptr  test_ptr       [.] module_2_init(Domain&)::__internal_func_or_circuit::operator()
   0.18%  test_ptr  libc-2.23.so   [.] _int_malloc
   0.15%  test_ptr  ld-2.23.so     [.] do_lookup_x
   0.15%  test_ptr  test_ptr       [.] module_2_init(Domain&)::__internal_func_xor_circuit::operator()

```

test_hash:
```
  33.11%  test_hash  test_hash            [.] Domain::call<char const (&) [11], Domain::Variable&, Domain::Variable&>
  25.37%  test_hash  test_hash            [.] main
  21.46%  test_hash  libstdc++.so.6.0.26  [.] std::_Hash_bytes
   5.10%  test_hash  libc-2.23.so         [.] __memcmp_sse4_1
   4.64%  test_hash  test_hash            [.] std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_construct<char const*>
   3.41%  test_hash  test_hash            [.] module_1_init(Domain&)::__internal_func_some_circuit::operator()
   1.86%  test_hash  libc-2.23.so         [.] strlen
   1.44%  test_hash  test_hash            [.] module_2_init(Domain&)::__internal_func_and_circuit::operator()
   1.39%  test_hash  libc-2.23.so         [.] __memcpy_avx_unaligned
   0.55%  test_hash  test_hash            [.] std::_Hash_bytes@plt

```

The hashmap implementation has a very high overhead coming from repeated hashing and function lookup.