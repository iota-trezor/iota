#ifndef BIGINT_H
#define BIGINT_H

#include "iota_types.h"

int bigint_add_int(int32_t bigint_in[], int32_t int_in, int32_t bigint_out[], uint8_t len);
int bigint_add_bigint(int32_t bigint_one[], int32_t bigint_two[], int32_t bigint_out[], uint8_t len);

#endif // BIGINT_H
