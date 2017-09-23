#include "conversion.h"

int trits_to_trytes(const trit_t trits_in[], tryte_t trytes_out[], uint32_t trit_len)
{
    if (trit_len % 3 != 0) {
        return -1;
    }
    uint32_t tryte_len = trit_len / 3;

    for (uint32_t i = 0; i < tryte_len; i++) {
        trytes_out[i] = trits_in[i*3+0] + trits_in[i*3+1]*3 + trits_in[i*3+2]*9;
    }
    return 0;
}

int trytes_to_trits(const tryte_t trytes_in[], trit_t trits_out[], uint32_t tryte_len)
{
    for (uint32_t i = 0; i < tryte_len; i++) {
        int32_to_trits((int32_t)trytes_in[i], &trits_out[i*3], 3);
    }
    return 0;
}

int int32_to_trits(const int32_t value, trit_t trits_out[], uint8_t max_len)
{
    int32_t quotient = value / 3;
    int8_t remainder = value % 3;
    if (remainder == 2) {
        quotient++;
        remainder = -1;
    }
    if (remainder == -2) {
        quotient--;
        remainder = 1;
    }
    trits_out[0] = remainder;

    if (quotient == 0) {
        return 0;
    }

    int ret = 0;
    if (max_len > 1) {
        ret = int32_to_trits(quotient, &trits_out[1], max_len-1);
    } else {
        // Not sufficient trits available for representation
        return -1;
    }

    return ret;
}
