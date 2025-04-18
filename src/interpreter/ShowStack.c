#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DataTypes.h"
#include "Parser.h"

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
    case iDefineProcedure:
    case iDefineProcedureNull:
        return "iDefineProcedure";
        break;
    case iCall:
    case iCallNull:
        return "iCall";
        break;
    case iInfiniteStart:
    case iInfiniteStartNull:
        return "iInfiniteStart";
        break;
    case iInfiniteEnd:
        return "iInfiniteEnd";
        break;
    case iRepeatStart:
    case iRepeatStartNull:
        return "iRepeatStart";
        break;
    case iRepeatEnd:
        return "iRepeatEnd";
        break;
    case iEnd:
        return "iEnd";
        break;
    case iContinue:
        return "iContinue";
        break;
    case iConditionalStart:
    case iConditionalStartNull:
        return "iConditionalStart";
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
            "  %lld: type=%s, context=%d, jump_address=%lld\n", index, type_repr, instruction.argument, instruction.optional.jump_address
        );
        break;
    case iDefineProcedure:
    case iDefineProcedureNull:
        printf(
            "  %lld: type=%s, argument='%c', procedure=%p\n", index, type_repr, instruction.argument, instruction.optional.procedure
        );
        break;
    case iInfiniteStart:
    case iInfiniteStartNull:
    case iRepeatStart:
    case iRepeatStartNull:
    case iConditionalStart:
    case iConditionalStartNull:
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
}

void
repr_procedure(const size_t index, Procedure* procedure)
{
    printf("%lld: Procedure (address: %p)\n", index, procedure);
    for (size_t i = 0; i < procedure->instruction_array->length; i++)
    {
        repr_instruction(i, procedure->instruction_array->data[i]);
    }
    printf("\n");
}

void
showstack(FILE* file)
{
    VMState* vm = parse_file(file);

    for (size_t i = 0; i < vm->all_procedures->length; i++)
    {
        repr_procedure(i, vm->all_procedures->data[i]);
    }

    VMState_free(vm);
}
