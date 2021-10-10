# time-parser

when i want to procrastinate i write terrible C code this is proof

## usage
works with ms, sec and min i was too lazy to add other time values 
```C
#include "time_parser.h"
#include <stdio.h>

int main(void)
{
    uint32_t time = parse_time_ms("5 seconds");
    printf("Value: %d", time);
    return 0;
}
```
