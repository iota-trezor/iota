#ifndef CONVERSION_H
#define CONVERSION_H

#include "iota_types.h"

int trits_to_trytes(const trit_t trits_in[], tryte_t trytes_out[], uint32_t trit_len);
int trytes_to_trits(const tryte_t trytes_in[], trit_t trits_out[], uint32_t tryte_len);

int int32_to_trits(const int32_t value, trit_t trits_out[], uint8_t max_len);

#endif // CONVERSION_H
