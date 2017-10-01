#include "kerl.h"
#include "conversion.h"
#include "trezor-crypto/sha3.h"
#include <stdio.h>

// Keccak 384 object
SHA3_CTX ctx;

int kerl_initialize()
{
    keccak_384_Init(&ctx);
}

int kerl_absorb_trits(const trit_t trits_in[], uint16_t len)
{
    // Last trit to zero
    trit_t trits[243] = {0} ;
    memcpy(trits, trits_in, 242);

    // First, convert to bytes
    int32_t words[12];
    char bytes[48];
    trits_to_words(trits, words);
    words_to_bytes(words, bytes, 12);
    keccak_Update(&ctx, bytes, 48);
}

int kerl_squeeze_trits(trit_t trits_out[], uint16_t len)
{
    char bytes_out[48] = {0};
    keccak_Final(&ctx, bytes_out);

    // Convert to trits
    int32_t words[12];
    bytes_to_words(bytes_out, words, 12);
    words_to_trits(words, trits_out);

    // Last trit zero
    trits_out[242] = 0;

    // Flip bytes
    for (uint8_t i = 0; i < 48; i++) {
        bytes_out[i] = bytes_out[i] ^ 0xFF;
    }

    keccak_384_Init(&ctx);
    keccak_Update(&ctx, bytes_out, 48);

    return 0;
}
