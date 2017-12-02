#include <unity.h>
#include <stdio.h>
#include "../iota/kerl.h"
#include "../iota/conversion.h"
#include "../iota/addresses.h"

#ifndef TEST_N_ADDRESSES
#define TEST_N_ADDRESSES 100
#endif

void progress_callback(uint32_t progress)
{
    (void) progress;
}

void test_AddIndexToSeed()
{
    trit_t trits[243] = {0};
    trit_t expected_output[243] = {0};
    int32_to_trits(1234, expected_output, 243);

    add_index_to_seed(trits, 1234);

    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_output, trits, 243);
}

void test_AddIndexToSeedOverflow()
{
    trit_t trits[244] = {0};
    trit_t expected_output[244] = {0};
    for (uint8_t i = 0; i < 243; i++) {
        trits[i] = 1;
        expected_output[i] = -1;
    }
    trits[0] = -1; // third addition to this will overflow

    // index 243 is outside region that should be written to
    trits[243] = 0xBA;
    expected_output[243] = 0xBA;

    add_index_to_seed(trits, 3);

    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_output, trits, 244);
}

void test_AddressGeneration()
{
    if(0) TEST_IGNORE_MESSAGE("Not running because it takes much time.");
    // Open the test-vector file
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    char full_filename[1024];
    char kerl_testvectors_path[] = KERL_TEST_VECTORS_PATH;
    size_t kerl_testvectors_path_size = sizeof(kerl_testvectors_path);
    char filename[] = "generateNAddressesForSeed";
    memcpy(full_filename, kerl_testvectors_path, kerl_testvectors_path_size);
    memcpy(&full_filename[kerl_testvectors_path_size-1], filename, sizeof(filename));

    fp = fopen(full_filename, "r");
    if (fp == NULL) {
        TEST_ASSERT_FALSE_MESSAGE(true, "could not open test vectors file");
        return;
    }

    // read first line (headers)
    read = getline(&line, &len, fp);
    // go through all hashes
    char seed[81];
    char expected_address[4][81];
    uint32_t cnt = 0;
    while(cnt < TEST_N_ADDRESSES && (read = getline(&line, &len, fp)) != -1) {
        cnt++;
        memcpy(seed, line, 81);
        memcpy(expected_address[0], &line[82], 81);
        memcpy(expected_address[1], &line[164], 81);
        memcpy(expected_address[2], &line[246], 81);
        memcpy(expected_address[3], &line[328], 81);

        tryte_t trytes[81] = {0};
        trit_t seed_trits[243] = {0};
        chars_to_trytes(seed, trytes, 81);
        trytes_to_trits(trytes, seed_trits, 81);

        for (uint8_t i = 0; i < 4; i++) {

            trit_t private_key[2*243*27];
            generate_private_key(seed_trits, i, private_key);

            trit_t public_addr[243];
            generate_public_address(private_key, public_addr, progress_callback);

            // convert output to char format
            tryte_t trytes_out[81] = {0};
            char chars_out[81] = {0};
            trits_to_trytes(public_addr, trytes_out, 243);
            trytes_to_chars(trytes_out, chars_out, 81);
            char fail_msg[200];
            sprintf(fail_msg, "address[%u]", i);
            TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected_address[i], chars_out, 81, fail_msg);
        }
    }
}

int main(void)
{
    UNITY_BEGIN();

    // add index to seed
    RUN_TEST(test_AddIndexToSeed);
    RUN_TEST(test_AddIndexToSeedOverflow);

    // address generation
    RUN_TEST(test_AddressGeneration);

    return UNITY_END();
}
