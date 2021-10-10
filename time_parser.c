#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "time_parser.h"

typedef enum TIME_MOD 
{
    MOD_MS,
    MOD_SEC,
    MOD_MIN,
    MOD_BAD,
} TIME_MOD;

typedef struct TimeData 
{
    size_t      amount;
    TIME_MOD    mod;
} TimeData;

// string comparison for suffix that can determine if the values are the same without the pesky plural 
// first arg should be the str_mod second should be the literal
int suffix_cmp
(
    const char* str1,
    const char* str2,
    size_t len
)
{
    for(int i = 0; i < len; i++)
    {
        if(str1[i] != str2[i])
            return 0;

        if(i+1 == len && str1[i] == 's')
            return 1;
    }

    return 1;
}

TIME_MOD get_mod(const char* str_mod, size_t len)
{
    if(suffix_cmp(str_mod, "milliseconds", len) == 1)
        return MOD_MS;
    else if(suffix_cmp(str_mod, "seconds", len) == 1)
        return MOD_SEC;
    else if(suffix_cmp(str_mod, "minutes", len) == 1)
        return MOD_MIN;
    else 
        return MOD_BAD;
}

TimeData parse_data(const char* str)
{
    char prefix[20];
    int i;
    int split;

    for(i = 0; str[i] != '\0'; i++) 
    {
        if(str[i] != ' ' && i < 20)
            prefix[i] = str[i];
        if(str[i] == ' ')
            split = i+1;
    }

    const char *suffix = str + split;
    int mod = get_mod(suffix, abs(split-i));

    if(mod == MOD_BAD)
    {
        return (TimeData) {
            .amount = 0,
            .mod    = mod,
        };
    }

    size_t to_i = strtoull(prefix, NULL, 10);

    TimeData data = 
    {
        .amount = to_i,
        .mod    = mod,
    };

    return data;
} 

// accepts time in the form of (int)prefix (string)suffix like 5 seconds and would output 5000
size_t parse_time_ms(const char* str)
{
    TimeData data = parse_data(str);

    size_t value = 0;

    switch(data.mod)
    {
        case MOD_MS:  value  = data.amount;           break;
        case MOD_SEC: value  = data.amount * 1000;    break;
        case MOD_MIN: value  = data.amount * 60000;   break;
        default:      value  = 0;
    }

    return value;
}

size_t parse_time_sec(const char* str)
{
    TimeData data = parse_data(str);

    size_t value = 0;

    switch(data.mod)
    {
        case MOD_MS:  value  = data.amount / 1000; break;
        case MOD_SEC: value  = data.amount;        break;
        case MOD_MIN: value  = data.amount / 60;   break;
        default:      value  = 0;
    }

    return value;
}

size_t parse_time_min(const char* str)
{
    TimeData data = parse_data(str);

    size_t value = 0;

    switch(data.mod)
    {
        case MOD_MS:  value  = data.amount * 60000; break;
        case MOD_SEC: value  = data.amount / 60;    break;
        case MOD_MIN: value  = data.amount;         break;
        default:      value  = 0;
    }

    return value;
}