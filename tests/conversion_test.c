#include <unity.h>
#include "../iota/conversion.h"
#include <string.h>

void test_ThreeTritsToSingleTryte(void)
{
    trit_t trits_9[] = { 0,  0,  0}; // 0
    trit_t trits_E[] = {-1, -1,  1}; // 5
    trit_t trits_N[] = {-1, -1, -1}; // -13
    trit_t trits_V[] = { 1,  1, -1}; // -5

    tryte_t trytes_out[1] = {0};
    int ret;
    ret = trits_to_trytes(trits_9, trytes_out, 3);
    TEST_ASSERT_EQUAL(0, trytes_out[0]);
    TEST_ASSERT_EQUAL(0, ret);

    ret = trits_to_trytes(trits_E, trytes_out, 3);
    TEST_ASSERT_EQUAL(5, trytes_out[0]);
    TEST_ASSERT_EQUAL(0, ret);

    ret = trits_to_trytes(trits_N, trytes_out, 3);
    TEST_ASSERT_EQUAL(-13, trytes_out[0]);
    TEST_ASSERT_EQUAL(0, ret);

    ret = trits_to_trytes(trits_V, trytes_out, 3);
    TEST_ASSERT_EQUAL(-5, trytes_out[0]);
    TEST_ASSERT_EQUAL(0, ret);
}

void test_InvalidNumberOfTrits(void)
{
    trit_t trits_V[] = { 1,  1, -1}; // -5
    tryte_t trytes_out[1] = {0};

    int ret;
    ret = trits_to_trytes(trits_V, trytes_out, 2);
    // Unchanged
    TEST_ASSERT_EQUAL(0, trytes_out[0]);
    // Error
    TEST_ASSERT_EQUAL(-1, ret);
}

void test_MultipleTritsToTrytes(void)
{
    trit_t trits_EN[] = { -1, -1,  1, -1, -1, -1}; // 5, -13
    tryte_t trytes_out[2] = {0, 0};

    int ret;
    ret = trits_to_trytes(trits_EN, trytes_out, 6);
    TEST_ASSERT_EQUAL(5, trytes_out[0]);
    TEST_ASSERT_EQUAL(-13, trytes_out[1]);
    TEST_ASSERT_EQUAL(0, ret);
}

void test_Int32ToTrits(void)
{
    int32_t value = 10;
    trit_t trits_out[10] = {0};

    int ret;
    ret = int32_to_trits(value, trits_out, 10);

    TEST_ASSERT_EQUAL(1, trits_out[0]);
    TEST_ASSERT_EQUAL(0, trits_out[1]);
    TEST_ASSERT_EQUAL(1, trits_out[2]);
    // No error
    TEST_ASSERT_EQUAL(0, ret);

    value = -17;
    ret = int32_to_trits(value, trits_out, 10);

    TEST_ASSERT_EQUAL(1, trits_out[0]);
    TEST_ASSERT_EQUAL(0, trits_out[1]);
    TEST_ASSERT_EQUAL(1, trits_out[2]);
    TEST_ASSERT_EQUAL(-1, trits_out[3]);
    // No error
    TEST_ASSERT_EQUAL(0, ret);
}

void test_Int32ToTritsMaxLengthExceeded()
{
    int32_t value = -17;
    trit_t trits_out[10] = {0};

    int ret;
    ret = int32_to_trits(value, trits_out, 3);

    // Values are partly written
    TEST_ASSERT_EQUAL(1, trits_out[0]);
    TEST_ASSERT_EQUAL(0, trits_out[1]);
    TEST_ASSERT_EQUAL(1, trits_out[2]);
    TEST_ASSERT_EQUAL(0, trits_out[3]); // This one is not written

    // Max length exceeded, give error
    TEST_ASSERT_EQUAL(-1, ret);
}

void test_TrytesToTrits(void)
{
    tryte_t trytes_in[2] = {-5, 13}; // [1,1,-1] [1,1,1]
    trit_t trits_out[6] = {0};

    int ret;
    ret = trytes_to_trits(trytes_in, trits_out, 2);

    TEST_ASSERT_EQUAL(1, trits_out[0]);
    TEST_ASSERT_EQUAL(1, trits_out[1]);
    TEST_ASSERT_EQUAL(-1, trits_out[2]);
    TEST_ASSERT_EQUAL(1, trits_out[3]);
    TEST_ASSERT_EQUAL(1, trits_out[4]);
    TEST_ASSERT_EQUAL(1, trits_out[5]);
    TEST_ASSERT_EQUAL(0, ret);
}

void test_TritsToWordsSmallPositiveNumber()
{
    trit_t trits_in[243] = {0};
    trit_t first_trits[3] = {1, 0, 1}; // 10
    memcpy(trits_in, first_trits, sizeof(first_trits));
    int32_t words_out[12] = {0};

    int ret;
    ret = trits_to_words(trits_in, words_out);
    TEST_ASSERT_EQUAL_HEX32(0x0000000A, words_out[0]);
    TEST_ASSERT_EQUAL_HEX32(0x00000000, words_out[1]);
    TEST_ASSERT_EQUAL(0, ret);
}

void test_TritsToWordsPositiveNumber()
{
    trit_t trits_in[243] = {0};
    trit_t first_trits[24] = { 1, 0,  1, -1, -1, 1, 0, -1,  0, 1, 1, 1,
                              -1, 0, -1, -1, -1, 1, 1,  1, -1, 1, 0, 0};
    memcpy(trits_in, first_trits, sizeof(first_trits));
    int32_t words_out[12] = {0};

    int ret;
    ret = trits_to_words(trits_in, words_out);

    TEST_ASSERT_EQUAL_HEX32(0x00000080, words_out[0]);
    TEST_ASSERT_EQUAL_HEX32(0x00000002, words_out[1]);
    TEST_ASSERT_EQUAL_HEX32(0x00000000, words_out[2]);

    TEST_ASSERT_EQUAL(0, ret);
}

void test_TritsToWordsSmallNegativeNumber()
{
    trit_t trits_in[243] = {0};
    trit_t first_trits[3] = {-1, 0, -1}; // -10 = 0xF6
    memcpy(trits_in, first_trits, sizeof(first_trits));
    int32_t words_out[12] = {0};

    int ret;
    ret = trits_to_words(trits_in, words_out);
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFF6, words_out[0]);
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFF, words_out[1]);
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFF, words_out[2]);
    TEST_ASSERT_EQUAL(0, ret);
}

void testTritsToWordsLargeNegativeNumber()
{
    trit_t trits_in[243] = {0};
    trit_t first_trits[24] = { -1, 0,  -1, +1, +1, -1, 0, +1,  0, -1, -1, -1,
                              +1, 0, +1, +1, +1, -1, -1,  -1, +1, -1, 0, 0};
    memcpy(trits_in, first_trits, sizeof(first_trits));
    int32_t words_out[12] = {0};

    int ret;
    ret = trits_to_words(trits_in, words_out);

    TEST_ASSERT_EQUAL_HEX32(0xFFFFFF80, words_out[0]);
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFD, words_out[1]);
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFF, words_out[2]);

    TEST_ASSERT_EQUAL(0, ret);
}


void test_TritsToWordsLastTritIsIgnored()
{
    // Last trit is 1, but will be ignored by the conversion.
    // It is considered to be zero
    trit_t trits_in[243] = {0};
    trits_in[242] = 1;
    int32_t words_out[12] = {0};
    int ret;

    ret = trits_to_words(trits_in, words_out);

    TEST_ASSERT_EQUAL_HEX32(0x00000000, words_out[0]);
    TEST_ASSERT_EQUAL_HEX32(0x00000000, words_out[1]);
    TEST_ASSERT_EQUAL_HEX32(0x00000000, words_out[11]);

    TEST_ASSERT_EQUAL(0, ret);
}

void testTritsToWordsLargestPossibleNumber()
{
    // Last trit is 1, but will be ignored by the conversion.
    // It is considered to be zero
    trit_t trits_in[243];
    for (int i = 0; i < 243; i++) { trits_in[i] = 1; }
    int32_t words_out[12];
    int ret;

    ret = trits_to_words(trits_in, words_out);

    TEST_ASSERT_EQUAL_HEX32(0xA5CE8964, words_out[0]);
    TEST_ASSERT_EQUAL_HEX32(0x9F007669, words_out[1]);
    TEST_ASSERT_EQUAL_HEX32(0x5E69EBEF, words_out[11]);
}

void testTritsToWordsAllZeros()
{
    trit_t trits_in[243] = {0};
    int32_t words_out[12];
    int ret;

    ret = trits_to_words(trits_in, words_out);

    TEST_ASSERT_EQUAL_HEX32(0x00000000, words_out[0]);
    TEST_ASSERT_EQUAL_HEX32(0x00000000, words_out[1]);
    TEST_ASSERT_EQUAL_HEX32(0x00000000, words_out[11]);

    TEST_ASSERT_EQUAL(0, ret);
}

void test_WordsToTritsAllZeros()
{
    int32_t words_in[12] = {0};
    trit_t trits_out[243] = {0};
    // initialize with wrong value
    for (int i = 0; i < 243; i++) { trits_out[i] = 127; }
    int ret;

    ret = words_to_trits(words_in, trits_out);

    TEST_ASSERT_EACH_EQUAL_INT8(0, trits_out, 243);
    TEST_ASSERT_EQUAL(0, ret);
}

void test_WordsToTritsSmallPositiveNumber()
{
    int32_t words_in[12] = {0};
    words_in[0] = 0x0000000A; // 10
    trit_t trits_out[243] = {0};
    int ret;

    ret = words_to_trits(words_in, trits_out);

    TEST_ASSERT_EQUAL(1, trits_out[0]);
    TEST_ASSERT_EQUAL(0, trits_out[1]);
    TEST_ASSERT_EQUAL(1, trits_out[2]);
}

void test_WordsToTritsLargePositiveNumber()
{
    int32_t words_in[12] = {0};
    words_in[0] = 0x00000080;
    words_in[1] = 0x00000002;
    trit_t trits_out[243] = {0};
    int ret;

    ret = words_to_trits(words_in, trits_out);

    trit_t expected_trits[24] = { 1, 0,  1, -1, -1, 1, 0, -1,  0, 1, 1, 1,
                                 -1, 0, -1, -1, -1, 1, 1,  1, -1, 1, 0, 0};
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_trits, trits_out, 24);
}

void test_WordsToBytes()
{
    int32_t words_in[2] = {0x12345678, 0xAABBCCDD};
    char bytes_out[8];
    char expected_bytes[] = {0xAA, 0xBB, 0xCC, 0xDD, 0x12, 0x34, 0x56, 0x78};
    int ret;

    ret = words_to_bytes(words_in, bytes_out, 2);

    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_bytes, bytes_out, 8);

    TEST_ASSERT_EQUAL(0, ret);
}

void test_BytesToWords()
{
    char bytes_in[] = {0xAA, 0xBB, 0xCC, 0xDD, 0x12, 0x34, 0x56, 0x78};
    int32_t words_out[2];
    int32_t expected_words[2] = {0x12345678, 0xAABBCCDD};
    int ret;

    ret = bytes_to_words(bytes_in, words_out, 2);

    TEST_ASSERT_EQUAL_HEX32_ARRAY(expected_words, words_out, 2);
}

void test_CharsToTrytes()
{
    char chars_in[] = "9ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    tryte_t trytes_out[27] = {0};
    tryte_t expected_trytes[27] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                            -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1};
    int ret;

    ret = chars_to_trytes(chars_in, trytes_out, 27);

    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_trytes, trytes_out, 27);

    TEST_ASSERT_EQUAL(0, ret);
}

void test_TrytesToChars()
{
    tryte_t trytes_in[27] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                            -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1};
    char chars_out[27] = {'\0'};
    char expected_chars[] = "9ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int ret;

    ret = trytes_to_chars(trytes_in, chars_out, 27);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_chars, chars_out, 27);

    TEST_ASSERT_EQUAL(0, ret);

}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_ThreeTritsToSingleTryte);
    RUN_TEST(test_InvalidNumberOfTrits);
    RUN_TEST(test_MultipleTritsToTrytes);
    RUN_TEST(test_Int32ToTrits);
    RUN_TEST(test_Int32ToTritsMaxLengthExceeded);
    RUN_TEST(test_TrytesToTrits);

    // trits_to_words
    RUN_TEST(test_TritsToWordsSmallPositiveNumber);
    RUN_TEST(test_TritsToWordsPositiveNumber);
    RUN_TEST(test_TritsToWordsSmallNegativeNumber);
    RUN_TEST(testTritsToWordsLargeNegativeNumber);
    RUN_TEST(test_TritsToWordsLastTritIsIgnored);
    RUN_TEST(testTritsToWordsLargestPossibleNumber);
    RUN_TEST(testTritsToWordsAllZeros);

    // words_to_trits
    RUN_TEST(test_WordsToTritsAllZeros);
    RUN_TEST(test_WordsToTritsSmallPositiveNumber);
    RUN_TEST(test_WordsToTritsLargePositiveNumber);

    // words_to_bytes
    RUN_TEST(test_WordsToBytes);

    // bytes_to_words
    RUN_TEST(test_BytesToWords);

    // chars to trytes
    RUN_TEST(test_CharsToTrytes);

    // trytes to chars
    RUN_TEST(test_TrytesToChars);

    return UNITY_END();
}
