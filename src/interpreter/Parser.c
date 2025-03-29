#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ShortStack.h"
#include "ProcedureStack.h"
#include "InstructionStack.h"
#include "VMState.h"
#include "BracketStack.h"

typedef enum ExpectedChar
{
    chNone,
    chArgument,
    chBlock
} ExpectedChar;

void
append_instruction(VMState* vm, BracketStack* bstack, Instruction instruction)
{
    for (size_t i = bstack->size - 1; i >= 0; i--)
    {
        if (bstack->data[i].symbol == '{')
        {
            InstructionStack_append(bstack->data[i].procedure->iarr, instruction);
            return;
        }
    }
    InstructionStack_append(vm->all_procedures->data[0]->iarr, instruction);
}

VMState*
parse_file(FILE* file)
{
    VMState* vm = VMState_new();
    ProcedureStack_append(
        vm->all_procedures,
        Procedure_new(vm)
    );

    char chr;
    BracketStack* bstack = BracketStack_new(8);
    bool is_comment = false;
    ExpectedChar next_char = chNone;

    while ((chr = fgetc(file)) != EOF)
    {
        if (chr == '\n')
        {
            is_comment = false;
            continue;
        }
        if (chr == '\'')
        {
            is_comment = true;
            continue;
        }
        if ((chr == ' ') || (chr == '\t'))
        {
            continue;
        }
        if (next_char == chNone)
        {
            switch (chr)
            {
            case '^':
                append_instruction(vm, bstack, (Instruction) { .type = iPushZero });
                break;
            case '+':
                append_instruction(vm, bstack, (Instruction) { .type = iIncrement });
                break;
            case '-':
                append_instruction(vm, bstack, (Instruction) { .type = iDecrement });
                break;
            case '*':
                append_instruction(vm, bstack, (Instruction) { .type = iAdd });
                break;
            case '~':
                append_instruction(vm, bstack, (Instruction) { .type = iSubtract });
                break;
            }
        }
    }
    BracketStack_free(bstack);
    fclose(file);
    return vm;
}
