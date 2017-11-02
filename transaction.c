#include "transaction.h"
#include "conversion.h"
#include "kerl.h"
#include <string.h>

/* Private function(s) prototype(s) */
void get_bundle_essence_trits(const char address[], uint64_t value, const char tag[], uint64_t timestamp, uint32_t current_index, uint32_t last_index, trit_t bundle_essence_trits[]);

void get_bundle_essence_trits(const char address[], uint64_t value, const char tag[], uint64_t timestamp, uint32_t current_index, uint32_t last_index, trit_t bundle_essence_trits[])
{
    // Set all to zero
    memset(bundle_essence_trits, 0, 162*3);

    // Address
    {
        trit_t* address_trits = &bundle_essence_trits[0];
        tryte_t address_trytes[81] = {0};
        chars_to_trytes(address, address_trytes, 81);
        trytes_to_trits(address_trytes, address_trits, 81);
    }

    // Value
    {
        trit_t* value_trits = &bundle_essence_trits[3*81];
        int32_to_trits(value, value_trits, 81);
    }

    // Tag
    {
        trit_t* tag_trits = &bundle_essence_trits[3*(81+27)];
        tryte_t tag_trytes[27];
        chars_to_trytes(tag, tag_trytes, 27);
        trytes_to_trits(tag_trytes, tag_trits, 27);
    }

    // Timestamp
    {
        trit_t* timestamp_trits = &bundle_essence_trits[3*(81+27+27)];
        int32_to_trits(timestamp, timestamp_trits, 3*9);
    }

    // Current index
    {
        trit_t* curr_idx_trits = &bundle_essence_trits[3*(81+27+27+9)];
        int32_to_trits(current_index, curr_idx_trits, 3*9);
    }

    // Last index
    {
        trit_t* last_idx_trits = &bundle_essence_trits[3*(81+27+27+9+9)];
        int32_to_trits(last_index, last_idx_trits, 3*9);
    }
}

void calculate_standard_bundle_hash(const char from_addr[], const char to_addr[], const char remainder_addr[], uint64_t balance, uint64_t transaction_amount, const char tag[], uint64_t timestamp, tryte_t bundle_hash_out[])
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

    trit_t trits_out[243];
    kerl_squeeze_trits(trits_out, 243);
    trits_to_trytes(trits_out, bundle_hash_out, 243);
}
