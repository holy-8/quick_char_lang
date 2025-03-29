#include <stdio.h>

char
get_index(const char name)
{
    if ('A' <= name <= 'Z')
        return name - 65;
    return name - 71;
}

int
main(int argc, char** argv)
{
    puts("Hello, world!");
    return 0;
}
