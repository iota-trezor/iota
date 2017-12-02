## Running the tests

Tests only execute when this repository is in trezor-mcu as a submodule, because it uses trezor cryptographic functions.

Make sure you are in the tests directory. Run the following commands:
```
mkdir build
cd build
cmake ..
make
ctest
```

The output should be similar to:
```
Test project [..]/trezor-mcu/vendor/iota/tests/build
    Start 1: conversion
1/8 Test #1: conversion .......................   Passed    0.00 sec
    Start 2: bigint
2/8 Test #2: bigint ...........................   Passed    0.00 sec
    Start 3: kerl
3/8 Test #3: kerl .............................   Passed    0.00 sec
    Start 4: kerl_trytes_and_hashes
4/8 Test #4: kerl_trytes_and_hashes ...........   Passed    0.95 sec
    Start 5: kerl_trytes_and_multi_squeeze
5/8 Test #5: kerl_trytes_and_multi_squeeze ....   Passed    1.57 sec
    Start 6: kerl_multitrytes_and_hashes
6/8 Test #6: kerl_multitrytes_and_hashes ......   Passed    2.01 sec
    Start 7: address_generation
7/8 Test #7: address_generation ...............   Passed    5.00 sec
    Start 8: transaction_test
8/8 Test #8: transaction_test .................   Passed    0.07 sec

100% tests passed, 0 tests failed out of 8

Total Test time (real) =   9.60 sec
```
