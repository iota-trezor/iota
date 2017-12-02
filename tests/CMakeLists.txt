project("IOTA Core C functionality" C)
cmake_minimum_required(VERSION 3.0)

include(CTest)

add_definitions(-DKERL_TEST_VECTORS_PATH="${PROJECT_SOURCE_DIR}/kerl/test_vectors/")
add_definitions(-DTEST_N_ADDRESSES=10)

# trezor-mcu directory (for vendor/.... includes)
include_directories(${PROJECT_SOURCE_DIR}/../../../)

# iota directory
include_directories(${PROJECT_SOURCE_DIR}/../)

add_library(Unity STATIC
    Unity/src/unity.c
)

target_include_directories(Unity PUBLIC
    Unity/src
)

add_executable(conversion_test
	conversion_test.c
	../conversion.c
    ../bigint.c
)
target_link_libraries(conversion_test Unity)
add_test(conversion conversion_test)

add_executable(bigint_test
	bigint_test.c
	../bigint.c)
target_link_libraries(bigint_test Unity)
add_test(bigint bigint_test)

add_executable(kerl_test
	kerl_test.c
	../kerl.c
	../bigint.c
	../conversion.c
	../../trezor-crypto/sha3.c)
target_link_libraries(kerl_test Unity)
add_test(kerl kerl_test)

add_executable(kerl_trytes_and_hashes_test
	kerl_trytes_and_hashes_test.c
	../kerl.c
	../bigint.c
	../conversion.c
	../../trezor-crypto/sha3.c)
target_link_libraries(kerl_trytes_and_hashes_test Unity)
add_test(kerl_trytes_and_hashes kerl_trytes_and_hashes_test)

add_executable(kerl_trytes_and_multi_squeeze_test
	kerl_trytes_and_multi_squeeze_test.c
	../kerl.c
	../bigint.c
	../conversion.c
	../../trezor-crypto/sha3.c)
target_link_libraries(kerl_trytes_and_multi_squeeze_test Unity)
add_test(kerl_trytes_and_multi_squeeze kerl_trytes_and_multi_squeeze_test)

add_executable(kerl_multitrytes_and_hashes_test
        kerl_multitrytes_and_hashes_test.c
        ../kerl.c
        ../bigint.c
        ../conversion.c
        ../../trezor-crypto/sha3.c)
target_link_libraries(kerl_multitrytes_and_hashes_test Unity)
add_test(kerl_multitrytes_and_hashes kerl_multitrytes_and_hashes_test)

add_executable(address_generation_test
        address_generation_test.c
        ../kerl.c
        ../bigint.c
        ../conversion.c
        ../addresses.c
        ../../trezor-crypto/sha3.c)
target_link_libraries(address_generation_test Unity)
add_test(address_generation address_generation_test)

add_executable(transaction_test
	transaction_test.c
	../transaction.c
	../bigint.c
	../conversion.c
	../kerl.c
	../../trezor-crypto/sha3.c
	../addresses.c)
target_link_libraries(transaction_test Unity)
add_test(transaction_test transaction_test)
