#include "conversion.h"
#include "bigint.h"

#include <stdio.h>

static const int32_t HALF_3[12] = { 0xa5ce8964,
                                    0x9f007669,
                                    0x1484504f,
                                    0x3ade00d9,
                                    0x0c24486e,
                                    0x50979d57,
                                    0x79a4c702,
                                    0x48bbae36,
                                    0xa9f6808b,
                                    0xaa06a805,
                                    0xa87fabdf,
                                    0x5e69ebef};

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
        int8_t idx = (int8_t) trytes_in[i] + 13;
        trits_out[i*3+0] = trits_mapping[idx][0];
        trits_out[i*3+1] = trits_mapping[idx][1];
        trits_out[i*3+2] = trits_mapping[idx][2];
        //printf("%c", seed[i]);
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

int trits_to_bytes(const trit_t trits_in[], int32_t bytes_out[])
{
    int32_t base[12] = {0};
    int32_t size = 12;
    for (int16_t i = 243 - 1; i-- > 0;) {
        // multiply
        {
            int32_t sz = size;
            int32_t carry = 0;

            for (int32_t j = 0; j < sz; j++) {
                int64_t v = ((int64_t)base[j]&0xFFFFFFFF) * ((int64_t)3) + ((int64_t)carry&0xFFFFFFFF);
                carry = (int32_t)((v >> 32) & 0xFFFFFFFF);
                //printf("[%i]carry: %u\n", i, carry);
                base[j] = (int32_t) (v & 0xFFFFFFFF);
            }

            if (carry > 0) {
                printf("ERR");
                base[sz] = carry;
                size++;
            }
        }

        // add
        {
            int32_t tmp[12];
            bigint_add_int(base, trits_in[i]+1, tmp, 12);
            memcpy(base, tmp, 48);
            // todo sz>size stuff
        }
    }

    if(0) {
        for (int32_t i = 0; i < 12; i++) {
            printf("%08x ", base[i]);
        }
        printf("\n");
    }

    if (bigint_cmp_bigint(HALF_3, base, 12) <= 0 ) {
        int32_t tmp[12];
        bigint_sub_bigint(base, HALF_3, tmp, 12);
        memcpy(base, tmp, 48);
    } else {
        int32_t tmp[12];
        bigint_sub_bigint(HALF_3, base, tmp, 12);
        bigint_not(tmp, 12);
        bigint_add_int(tmp, 1, base, 12);
    }

    memcpy(bytes_out, base, 48);
    return 0;
}
