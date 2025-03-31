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
    switch (type)
    {
    case iNone:
        return "iNone";
        break;
    case iPushZero:
        return "iPushZero";
        break;
    case iIncrement:
        return "iIncrement";
        break;
    case iDecrement:
        return "iDecrement";
        break;
    case iAdd:
        return "iAdd";
        break;
    case iSubtract:
        return "iSubtract";
        break;
    case iReverse:
    case iReverseNull:
        return "iReverse";
        break;
    case iAssign:
    case iAssignNull:
        return "iAssign";
        break;
    case iDelete:
    case iDeleteNull:
        return "iDelete";
        break;
    case iPush:
    case iPushNull:
        return "iPush";
        break;
    case iAssignLocal:
    case iAssignLocalNull:
        return "iAssignLocal";
        break;
    case iOutput:
    case iOutputNull:
        return "iOutput";
        break;
    case iInput:
    case iInputNull:
        return "iInput";
        break;
    case iCall:
    case iCallNull:
        return "iCall";
        break;
    case iEnd:
        return "iEnd";
        break;
    case iContinue:
        return "iContinue";
        break;
    case iDefineProcedure:
        return "iDefineProcedure";
        break;
    case iInfiniteStart:
        return "iInfiniteStart";
        break;
    case iRepeatStart:
        return "iRepeatStart";
        break;
    case iConditionalStart:
        return "iConditionalStart";
        break;
    case iInfiniteEnd:
        return "iInfiniteEnd";
        break;
    case iRepeatEnd:
        return "iRepeatEnd";
        break;
    case iConditionalEnd:
        return "iConditionalEnd";
        break;
    default:
        return "Invalid instruction type";
        break;
    }
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
