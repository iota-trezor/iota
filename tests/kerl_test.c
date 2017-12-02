#include <unity.h>
#include "../iota/kerl.h"
#include "../iota/conversion.h"

void test_KerlHashOnce()
{
    char chars[81] = "GYOMKVTSNHVJNCNFBBAH9AAMXLPLLLROQY99QN9DLSJUHDPBLCFFAIQXZA9BKMBJCYSFHFPXAHDWZFEIZ";
    char expected_output[81] = "OXJCNFHUNAHWDLKKPELTBFUCVW9KLXKOGWERKTJXQMXTKFKNWNNXYD9DMJJABSEIONOSJTTEVKVDQEWTW";
    tryte_t trytes[81] = {0};
    trit_t trits_in[243] = {0};
    trit_t trits_out[243] = {0};
    chars_to_trytes(chars, trytes, 81);
    trytes_to_trits(trytes, trits_in, 81);

    kerl_initialize();
    kerl_absorb_trits(trits_in, 243);
    kerl_squeeze_trits(trits_out, 243);

    // convert output to char format
    tryte_t trytes_out[81] = {0};
    char chars_out[81] = {0};
    trits_to_trytes(trits_out, trytes_out, 243);
    trytes_to_chars(trytes_out, chars_out, 81);

    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_output, chars_out, 81);
}

int main(void)
{
    UNITY_BEGIN();

    // kerl hash
    RUN_TEST(test_KerlHashOnce);

    return UNITY_END();
}
