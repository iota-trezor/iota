#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "iota_types.h"

void calculate_standard_bundle_hash(const char from_addr[], const char to_addr[], const char remainder_addr[], uint64_t balance, uint64_t transaction_amount, const char tag[], uint64_t timestamp, tryte_t bundle_hash_out[]);
void normalize_hash(const tryte_t hash_in[], tryte_t normalized_hash_out[]);

#endif // TRANSACTION_H
