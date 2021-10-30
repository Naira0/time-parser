# time-parser

A time literal parser

### string to int
```C
#include "time_parser.h"
#include <stdio.h>

// valid types of literals:
// 5s 5sec 5 second 5 seconds

parse_time("120000MS", TVAL_MIN); // 2
parse_time("3 minutes", TVAL_SEC); // 180
parse_time("5sec", TVAL_MS); // 5000

int main(void)
{
    uint32_t time = parse_time("5 seconds", TVAL_MS); 
    printf("Value: %d", time); // Value: 5000
    return 0;
}
```

### int to string

```c
#include "time_parser.h"
#include <stdio.h>

int main(void)
{
    char str[255];
    time_str(str, 135, TVAL_SEC);
    printf("%s\n", str); // 2 minutes, 15 seconds
}
```