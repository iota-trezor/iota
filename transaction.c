#include "transaction.h"
#include "conversion.h"
#include "kerl.h"
#include <string.h>

/* Private function(s) prototype(s) */
void get_bundle_essence_trits(const char address[], uint64_t value, const char tag[], uint64_t timestamp, uint32_t current_index, uint32_t last_index, trit_t bundle_essence_trits[]);

void get_bundle_essence_trits(const char address[], uint64_t value, const char tag[], uint64_t timestamp, uint32_t current_index, uint32_t last_index, trit_t bundle_essence_trits[])
{
    char bundle_essence[162];
    memcpy(bundle_essence, address, 81);

    {
        trit_t value_trits[81] = {0};
        int32_to_trits(value, value_trits, 81);
        tryte_t value_trytes[27] = {0};
        trits_to_trytes(value_trits, value_trytes, 81);
        trytes_to_chars(value_trytes, &bundle_essence[81], 27);
    }

    memcpy(&bundle_essence[81+27], tag, 27);

    {
        trit_t timestamp_trits[27] = {0};
        int32_to_trits(timestamp, timestamp_trits, 27);
        tryte_t timestamp_trytes[9] = {0};
        trits_to_trytes(timestamp_trits, timestamp_trytes, 27);
        trytes_to_chars(timestamp_trytes, &bundle_essence[81+27+27], 9);
    }

    {
        trit_t curr_idx_trits[27] = {0};
        int32_to_trits(current_index, curr_idx_trits, 27);
        tryte_t curr_idx_trytes[9] = {0};
        trits_to_trytes(curr_idx_trits, curr_idx_trytes, 27);
        trytes_to_chars(curr_idx_trytes, &bundle_essence[81+27+27+9], 9);
    }

    {
        trit_t last_idx_trits[27] = {0};
        int32_to_trits(last_index, last_idx_trits, 27);
        tryte_t last_idx_trytes[9] = {0};
        trits_to_trytes(last_idx_trits, last_idx_trytes, 27);
        trytes_to_chars(last_idx_trytes, &bundle_essence[81+27+27+9+9], 9);
    }

    {
        // Convert to trits
        tryte_t bundle_essence_trytes[162];
        chars_to_trytes(bundle_essence, bundle_essence_trytes, 162);
        trytes_to_trits(bundle_essence_trytes, bundle_essence_trits, 162);
    }
}

void calculate_standard_bundle_hash(const char from_addr[], const char to_addr[], const char remainder_addr[], uint64_t balance, uint64_t transaction_amount, const char tag[], uint64_t timestamp, char bundle_hash_out[])
{
    kerl_initialize();

    // A standard bundle contains 4 transactions and is security level 2
    trit_t bundle_essence_trits[162*3];
    get_bundle_essence_trits(to_addr, transaction_amount, tag, timestamp, 0, 3, bundle_essence_trits);
    kerl_absorb_trits(bundle_essence_trits, 486);
    get_bundle_essence_trits(from_addr, -balance, tag, timestamp, 1, 3, bundle_essence_trits);
    kerl_absorb_trits(bundle_essence_trits, 486);
    get_bundle_essence_trits(from_addr, 0, tag, timestamp, 2, 3, bundle_essence_trits);
    kerl_absorb_trits(bundle_essence_trits, 486);
    get_bundle_essence_trits(remainder_addr, balance-transaction_amount, tag, timestamp, 3, 3, bundle_essence_trits);
    kerl_absorb_trits(bundle_essence_trits, 486);

    {
        trit_t trits_out[243];
        kerl_squeeze_trits(trits_out, 243);
        tryte_t trytes_out[81];
        trits_to_trytes(trits_out, trytes_out, 243);
        trytes_to_chars(trytes_out, bundle_hash_out, 81);
    }

    //memcpy(bundle_hash_out, &bundle_essence_trits[81], 27);
}
