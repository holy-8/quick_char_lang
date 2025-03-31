#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DataTypes.h"
#include "Parser.h"

char
get_index(const char name)
{
    if (('A' <= name) && (name <= 'Z'))
    {
        return name - 65;
    }
    return name - 71;
}

char*
repr_instruction_type(InstructionType type)
{
    char* buffer = calloc(32, sizeof(char));
    switch (type)
    {
    case iNone:
        strcpy(buffer, "iNone");
        break;
    case iPushZero:
        strcpy(buffer, "iPushZero");
        break;
    case iIncrement:
        strcpy(buffer, "iIncrement");
        break;
    case iDecrement:
        strcpy(buffer, "iDecrement");
        break;
    case iAdd:
        strcpy(buffer, "iAdd");
        break;
    case iSubtract:
        strcpy(buffer, "iSubtract");
        break;
    case iReverse:
    case iReverseNull:
        strcpy(buffer, "iReverse");
        break;
    case iAssign:
    case iAssignNull:
        strcpy(buffer, "iAssign");
        break;
    case iDelete:
    case iDeleteNull:
        strcpy(buffer, "iDelete");
        break;
    case iPush:
    case iPushNull:
        strcpy(buffer, "iPush");
        break;
    case iAssignLocal:
    case iAssignLocalNull:
        strcpy(buffer, "iAssignLocal");
        break;
    case iOutput:
    case iOutputNull:
        strcpy(buffer, "iOutput");
        break;
    case iInput:
    case iInputNull:
        strcpy(buffer, "iInput");
        break;
    case iCall:
    case iCallNull:
        strcpy(buffer, "iCall");
        break;
    case iEnd:
        strcpy(buffer, "iEnd");
        break;
    case iContinue:
        strcpy(buffer, "iContinue");
        break;
    case iDefineProcedure:
        strcpy(buffer, "iDefineProcedure");
        break;
    case iInfiniteStart:
        strcpy(buffer, "iInfiniteStart");
        break;
    case iRepeatStart:
        strcpy(buffer, "iRepeatStart");
        break;
    case iConditionalStart:
        strcpy(buffer, "iConditionalStart");
        break;
    case iInfiniteEnd:
        strcpy(buffer, "iInfiniteEnd");
        break;
    case iRepeatEnd:
        strcpy(buffer, "iRepeatEnd");
        break;
    case iConditionalEnd:
        strcpy(buffer, "iConditionalEnd");
        break;
    }
    return buffer;
}

void
repr_instruction(const size_t index, Instruction instruction)
{
    char* type_repr = repr_instruction_type(instruction.type);
    switch (instruction.type)
    {
    case iNone:
    case iPushZero:
    case iIncrement:
    case iDecrement:
    case iAdd:
    case iSubtract:
        printf(
            "  %lld: type=%s\n", index, type_repr
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
            "  %lld: type=%s, argument='%c'\n", index, type_repr, instruction.argument
        );
        break;
    case iEnd:
    case iContinue:
        printf(
            "  %lld: type=%s, context=%d\n", index, type_repr, instruction.argument
        );
        break;
    case iDefineProcedure:
        printf(
            "  %lld: type=%s, argument='%c', procedure=%p\n", index, type_repr, instruction.argument, instruction.optional.procedure
        );
        break;
    case iInfiniteStart:
    case iRepeatStart:
    case iConditionalStart:
        printf(
            "  %lld: type=%s, argument='%c', jump_address=%lld\n", index, type_repr, instruction.argument, instruction.optional.jump_address
        );
        break;
    case iInfiniteEnd:
    case iRepeatEnd:
    case iConditionalEnd:
        printf(
            "  %lld: type=%s, jump_address=%lld\n", index, type_repr, instruction.optional.jump_address
        );
        break;
    default:
        printf(
            "  %lld: ERROR! Invalid instruction type %d\n", index, instruction.type
        );
        break;
    }

    free(type_repr);
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
