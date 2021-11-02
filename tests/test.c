#include <stdio.h>
#include <conio.h>

#include "time_parser.h"
#include <windows.h>

int main(int argc, char **argv)
{
    if(argc <= 1)
    {
        puts("You must provide a valid time");
        return -1;
    }

    size_t time = 0;

    if(argc == 2)
        time = parse_time(argv[1], TVAL_MS);
    else
    {
        char buff[255];
        sprintf(buff, "%s %s", argv[1], argv[2]);
        time = parse_time(buff, TVAL_MS);
    }

    puts("Timer set!");

    Sleep(time);

    puts("DING DONG TIMES UP!");

    return 0;
}

