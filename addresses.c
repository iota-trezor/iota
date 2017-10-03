#include "addresses.h"
#include "kerl.h"

int generate_private_key(const trit_t seed_trits[], const uint32_t index, trit_t private_key[])
{
    trit_t tmp[243];
    memcpy(tmp, seed_trits, 243);
    kerl_initialize();
    kerl_absorb_trits(tmp, 243);
    kerl_squeeze_trits(tmp, 243);

    kerl_initialize();
    kerl_absorb_trits(tmp, 243);

    int8_t level = 2;
    for (uint8_t i = 0; i < level; i++) {
        for (uint8_t j = 0; j < 27; j++) {
            kerl_squeeze_trits(&private_key[i*243*27+j*243], 243);
        }
    }
}

int generate_public_address(const trit_t private_key[], trit_t address_out[])
{
    // Get digests
    trit_t digests[243*2];

    for (uint8_t i = 0; i < 2; i++) {
        trit_t key_fragment[243*27];
        memcpy(key_fragment, &private_key[i*243*27], 243*27);

        for (uint8_t j = 0; j < 27; j++) {
            for (uint8_t k = 0; k < 26; k++) {
                kerl_initialize();
                kerl_absorb_trits(&key_fragment[j*243], 243);
                kerl_squeeze_trits(&key_fragment[j*243], 243);
            }
        }

        kerl_initialize();
        kerl_absorb_trits(key_fragment, 243*27);
        kerl_squeeze_trits(&digests[i*243], 243);
    }

    // Get address
    kerl_initialize();
    kerl_absorb_trits(digests, 243*2);
    kerl_squeeze_trits(address_out, 243);
}
