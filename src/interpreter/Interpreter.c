#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DataTypes.h"
#include "Parser.h"
#include "ShowStack.h"

char
get_index(const char name)
{
    if (('A' <= name) && (name <= 'Z'))
    {
        return name - 65;
    }
    return name - 71;
}

int
main(int argc, char** argv)
{
    if (argc < 2)
    {
        fputs("ERROR! Usage: QCL <filepath> [-ss]\n", stderr);
        return EXIT_FAILURE;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fputs("ERROR! Could not open file\n", stderr);
        return EXIT_FAILURE;
    }

    if ((argc >= 3) && (strcmp(argv[2], "-ss") == 0))
    {
        showstack(file);
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}
