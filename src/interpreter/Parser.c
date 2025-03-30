#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "DataTypes.h"
#include "ShortStack.h"
#include "ProcedureStack.h"
#include "InstructionStack.h"
#include "VMState.h"
#include "BracketStack.h"
#include "ReadBuffer.h"

bool
try_comment(bool* is_comment, ReadBuffer* buffer)
{
    char last_char = ReadBuffer_peek(buffer);
    if ((last_char == '\'') || (last_char == '\n'))
    {
        ReadBuffer_pop(buffer);
        *is_comment = (bool) last_char == '\'';
        return true;
    }
    if ((last_char == ' ') || (last_char == '\t'))
    {
        ReadBuffer_pop(buffer);
        return true;
    }
    return *is_comment;
}

void
append_instruction(VMState* vm, BracketStack* bracket_stack, Instruction instruction)
{
    for (size_t i = bracket_stack->length - 1; i >= 0; i--)
    {
        if (bracket_stack->data[i].symbol == '{')
        {
            InstructionStack_append(
                bracket_stack->data[i].procedure->instruction_array, instruction
            );
            return;
        }
    }
    InstructionStack_append(
        vm->all_procedures->data[0]->instruction_array, instruction
    );
}

InstructionType
get_context(BracketStack* bracket_stack)
{
    if (bracket_stack->length == 0)
    {
        return iNone;
    }
    for (size_t i = bracket_stack->length - 1; i >= 0; i--)
    {
        switch (bracket_stack->data[i].symbol)
        {
        case '{':
            return iDefineProcedure;
            break;
        case '[':
            return iRepeatStart;
            break;
        case '(':
            return iInfiniteStart;
            break;
        }
    }
}

size_t
get_context_jump_address(BracketStack* bracket_stack)
{
    if (bracket_stack->length == 0)
    {
        return 0;
    }
    return bracket_stack->data[bracket_stack->length - 1].address;
}

bool
try_simple_instruction(VMState* vm, BracketStack* bracket_stack, ReadBuffer* buffer)
{
    switch (ReadBuffer_peek(buffer))
    {
    case '^':
        append_instruction(
            vm, bracket_stack,
            (Instruction) { .type = iPushZero }
        );
        goto success;
        break;
    case '+':
        append_instruction(
            vm, bracket_stack,
            (Instruction) { .type = iIncrement }
        );
        goto success;
        break;
    case '-':
        append_instruction(
            vm, bracket_stack,
            (Instruction) { .type = iDecrement }
        );
        goto success;
        break;
    case '*':
        append_instruction(
            vm, bracket_stack,
            (Instruction) { .type = iAdd }
        );
        goto success;
        break;
    case '~':
        append_instruction(
            vm, bracket_stack,
            (Instruction) { .type = iSubtract }
        );
        goto success;
        break;
    case '#':
        append_instruction(
            vm, bracket_stack,
            (Instruction) {
                .type = iEnd,
                .argument = get_context(bracket_stack),
                .optional.jump_address = get_context_jump_address(bracket_stack)
            }
        );
        goto success;
        break;
    case ':':
        append_instruction(
            vm, bracket_stack,
            (Instruction) {
                .type = iContinue,
                .argument = get_context(bracket_stack),
                .optional.jump_address = get_context_jump_address(bracket_stack)
            }
        );
        goto success;
        break;
    default:
        goto fail;
        break;
    }
    success:
        ReadBuffer_clear(buffer);
        return true;
    fail:
        return false;
}

bool
try_unknown_symbol(ReadBuffer* buffer)
{
    char current_char = ReadBuffer_peek(buffer);
    if (('A' <= current_char <= 'Z') || ('a' <= current_char <= 'z'))
    {
        return false;
    }
    switch (current_char)
    {
        case '_':
        case '%':
        case '=':
        case '!':
        case '$':
        case '&':
        case '<':
        case '>':
        case '{':
        case '}':
        case '@':
        case '(':
        case ')':
        case '[':
        case ']':
        case '?':
        case ';':
            return false;
            break;
        default:
            return true;
            break;
    }
}

bool
try_name_error(ReadBuffer* buffer)
{
    char first_char = buffer->buffer[0];
    if (!('A' <= first_char <= 'Z') && !('a' <= first_char <= 'z') && !(first_char == '_'))
    {
        return false;
    }
    char second_char = buffer->buffer[1];
    switch (second_char)
    {
        case '{':
        case '(':
        case '[':
        case '?':
            
    }
}

VMState*
parse_file(FILE* file)
{
    VMState* vm = VMState_new();
    ProcedureStack_append(
        vm->all_procedures,
        Procedure_new(vm)
    );

    ReadBuffer* buffer = ReadBuffer_new(file, 2);
    BracketStack* bracket_stack = BracketStack_new(8);
    bool is_comment = false;

    while (ReadBuffer_read(buffer))
    {
        if (try_comment(&is_comment, buffer))
        {
            continue;
        }
        if (buffer->length == 1)
        {
            if (try_simple_instruction(vm, bracket_stack, buffer))
            {
                continue;
            }
            if (try_unknown_symbol(buffer))
            {
                fprintf(stderr, "ERROR! Symbol '%c' is unknown.\n", ReadBuffer_peek(buffer));
                exit(EXIT_FAILURE);
            }
        }
        
    }

    ReadBuffer_free(buffer);
    BracketStack_free(bracket_stack);
    return vm;
}
