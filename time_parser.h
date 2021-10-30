#include <stdint.h>

#pragma once

typedef enum
{
    TVAL_MS,
    TVAL_SEC,
    TVAL_MIN
} TIME_VALUE;

size_t parse_time(const char* str, TIME_VALUE return_value);

void time_str(char* buff, size_t amount, TIME_VALUE amount_value);