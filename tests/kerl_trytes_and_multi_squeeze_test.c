#include <unity.h>
#include <stdio.h>
#include "../iota/kerl.h"
#include "../iota/conversion.h"

void test_KerlHashAndMultiSqueeze()
{
    // Open the test-vector file
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    char full_filename[1024];
    char kerl_testvectors_path[] = KERL_TEST_VECTORS_PATH;
    size_t kerl_testvectors_path_size = sizeof(kerl_testvectors_path);
    char filename[] = "generateTrytesAndMultiSqueeze";
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
    char hash_in[81];
    char squeeze[3][81];
    while((read = getline(&line, &len, fp)) != -1) {
        memcpy(hash_in, line, 81);
        memcpy(squeeze[0], &line[82], 81);
        memcpy(squeeze[1], &line[164], 81);
        memcpy(squeeze[2], &line[246], 81);

        tryte_t trytes[81] = {0};
        trit_t trits_in[243] = {0};
        trit_t trits_out[243] = {0};

        chars_to_trytes(hash_in, trytes, 81);
        trytes_to_trits(trytes, trits_in, 81);

        kerl_initialize();
        kerl_absorb_trits(trits_in, 243);

        static int cnt = 0;
        for (uint8_t i = 0; i < 3; i++) {

            kerl_squeeze_trits(trits_out, 243);

            // convert output to char format
            tryte_t trytes_out[81] = {0};
            char chars_out[81] = {0};
            trits_to_trytes(trits_out, trytes_out, 243);
            trytes_to_chars(trytes_out, chars_out, 81);

            char fail_msg[100];
            sprintf(fail_msg, "fail squeeze set %i [%u]", cnt, i);
            TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(squeeze[i], chars_out, 81, fail_msg);
        }
        cnt++;
    }
}

int main(void)
{
    UNITY_BEGIN();

    // kerl hash
    RUN_TEST(test_KerlHashAndMultiSqueeze);

    return UNITY_END();
}
