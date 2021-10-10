#include <stdint.h>

#ifndef TIME_PARSER
#define TIME_PARSER

size_t parse_time_ms(const char* str);
size_t parse_time_sec(const char* str);
size_t parse_time_min(const char* str);

#endif