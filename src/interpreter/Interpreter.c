#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ShowStack.h"
#include "Runtime.h"

int
main(int argc, char** argv)
{
    if (argc < 2)
    {
        fputs("ERROR! Usage: QCL <filepath> [-ss | -d]\n", stderr);
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
    if ((argc >= 3) && (strcmp(argv[2], "-d") == 0))
    {
        execute(file, true);
        return EXIT_SUCCESS;
    }

    execute(file, false);
    return EXIT_SUCCESS;
}
