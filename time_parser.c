#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

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

// prehashed bucket
// valid suffix's are ms, millisecond, millisecond, s, sec, second, seconds, min, minute, minutes
// it is possible for collisions to happen if the suffix is some random string but the worst that will happen is
// just getting a random time suffix
static const int mod_bucket[] = {
        2,
        2,
        3,
        3,
        3,
        3,
        3,
        1,
        0,
        3,
        2,
        3,
        3,
        3,
        3,
        3,
        3,
        2,
        3,
        0,
        3,
        3,
        3,
        3,
        1,
        3,
        0,
        1,
        3,
        3,
        3,
        1,
};

int hash_str(const char* str)
{
    int c = str[0], h = 0;

    for(int i = 0; c != 0 ; i++, c = str[i])
        h += c;

    return (h % 36);
}

TIME_MOD get_mod(const char* str_mod)
{
    for(char *p = str_mod; *p; ++p)
        *p = (char)tolower(*p);

    int hash = hash_str(str_mod);

    if(hash > 36)
        return MOD_BAD;
    else
        return mod_bucket[hash];
}

TimeData parse_data(const char* str)
{
    // is the size of a unsigned 64 bit int
    char prefix[21];
    // the index that the suffix starts at
    int  split = 0;

    for(int i = 0; str[i] != '\0'; i++)
    {
        char c = str[i];

        if(i < 21 && isdigit(c))
            prefix[i] = c;
        if(isalpha(c))
        {
            split = c == ' ' ? i+1 : i; // accounts for possible space separation
            break;
        }
    }

    // uses strdup so the string can be written to inside get_mod when it converts it to lowercase
    // otherwise it would create a segfault on some compilers
    const char *suffix = strdup(str + split);
    int mod = get_mod(suffix);
    free(suffix);

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

size_t parse_time(const char* str, TIME_VALUE return_value)
{
    TimeData data = parse_data(str);

    size_t value = 1;

    switch(return_value)
    {
        case TVAL_MS:
        {
            switch(data.mod)
            {
                case MOD_MS:  value  = data.amount;           break;
                case MOD_SEC: value  = data.amount * 1000;    break;
                case MOD_MIN: value  = data.amount * 60000;   break;
            }
            break;
        }
        case TVAL_SEC:
        {
            switch(data.mod)
            {
                case MOD_MS:  value  = data.amount / 1000; break;
                case MOD_SEC: value  = data.amount;        break;
                case MOD_MIN: value  = data.amount * 60;   break;
            }
            break;
        }
        case TVAL_MIN:
        {
            switch(data.mod)
            {
                case MOD_MS:  value  = data.amount / 60000; break;
                case MOD_SEC: value  = data.amount / 60;    break;
                case MOD_MIN: value  = data.amount;         break;
            }
        }
    }

    return value;
}

void time_str(char* buff, size_t amount, TIME_VALUE amount_value)
{
    if(buff == NULL)
        return;

    if(amount_value == TVAL_MS && amount < 1000)
    {
        sprintf(buff, "%zuMS", amount);
        return;
    }

    size_t to_minute;
    size_t to_second;
    size_t to_hour;

    switch(amount_value)
    {
        case TVAL_MS:
        {
            to_minute = (amount % 3600000) / 60000;
            to_second = (amount % 60000) / 1000;
            to_hour   = amount / 3600000;
            break;
        }
        case TVAL_SEC:
        {
            to_minute = (amount % 3600) / 60;
            to_second = amount % 60;
            to_hour   = amount / 3600;
            break;
        }
        case TVAL_MIN:
        {
            to_hour   = amount / 60;
            to_minute = amount % 60;
            to_second = (amount * 60) % 60;
        }
    }

    if(to_hour != 0)
        sprintf(buff, (to_minute != 0 || to_second != 0 ? "%zu hours, " : "%zu hours"), to_hour);
    if(to_minute != 0)
        sprintf(buff + strlen(buff), (to_second != 0 ? "%zu minutes, " : "%zu minutes"), to_minute);
    if(to_second != 0)
        sprintf(buff + strlen(buff), "%zu seconds", to_second);
}

