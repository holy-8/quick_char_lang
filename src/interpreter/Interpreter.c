#include <stdio.h>
#include <stdlib.h>

#include "DataTypes.h"
#include "Parser.h"

char
get_index(const char name)
{
    if ('A' <= name <= 'Z')
    {
        return name - 65;
    }
    return name - 71;
}

void
repr_instruction(const size_t index, Instruction instruction)
{
    switch (instruction.type)
    {
        case iNone:
        case iPushZero:
        case iIncrement:
        case iDecrement:
        case iAdd:
        case iSubtract:
            printf(
                "  %lld: type=%d\n", index, instruction.type
            );
            break;
        case iReverse:
        case iReverseNull:
        case iAssign:
        case iAssignNull:
        case iDelete:
        case iDeleteNull:
        case iPush:
        case iPushNull:
        case iAssignLocal:
        case iAssignLocalNull:
        case iOutput:
        case iOutputNull:
        case iInput:
        case iInputNull:
        case iCall:
        case iCallNull:
            printf(
                "  %lld: type=%d, argument='%c'\n", index, instruction.type, instruction.argument
            );
            break;
        case iEnd:
        case iContinue:
            printf(
                "  %lld: type=%d, context='%c'\n", index, instruction.type, instruction.argument
            );
            break;
        case iDefineProcedure:
            printf(
                "  %lld: type=%d, argument='%c', procedure=%p\n", index, instruction.type, instruction.argument, instruction.optional.procedure
            );
            break;
        case iInfiniteStart:
        case iRepeatStart:
        case iConditionalStart:
            printf(
                "  %lld: type=%d, argument='%c', jump_address=%lld\n", index, instruction.type, instruction.argument, instruction.optional.jump_address
            );
            break;
        case iInfiniteEnd:
        case iRepeatEnd:
        case iConditionalEnd:
            printf(
                "  %lld: type=%d, jump_address=%lld\n", index, instruction.type, instruction.optional.jump_address
            );
            break;
    }
}

void
repr_procedure(const size_t index, Procedure* procedure)
{
    printf("%lld: Procedure (address: %p)\n", index, procedure);

    for (size_t i = 0; i < procedure->instruction_array->length; i++)
    {
        repr_instruction(i, procedure->instruction_array->data[i]);
    }
}

int
main(int argc, char** argv)
{
    if (argc < 2)
    {
        fputs("Usage: QCL <filepath>\n", stderr);
        return EXIT_FAILURE;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fputs("ERROR! Could not open file\n", stderr);
        return EXIT_FAILURE;
    }

    VMState* vm = parse_file(file);

    for (size_t i = 0; i < vm->all_procedures->length; i++)
    {
        repr_procedure(i, vm->all_procedures->data[i]);
    }

    VMState_free(vm);
    return 0;
}
