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

typedef long long ssize_t;

bool
is_a_letter(const char name)
{
    if (('A' <= name) && (name <= 'Z'))
    {
        return true;
    }
    if (('a' <= name) && (name <= 'z'))
    {
        return true;
    }
    return false;
}

bool
try_comment(ReadBuffer* buffer)
{
    static bool is_comment;
    char last_char = ReadBuffer_peek(buffer);
    if (is_comment)
    {
        ReadBuffer_pop(buffer);
        is_comment = (bool) (last_char != '\n');
        return true;
    }
    switch (last_char)
    {
    case '\n':
    case '\t':
    case '\'':
    case ' ':
        ReadBuffer_pop(buffer);
        is_comment = (bool) (last_char == '\'');
        return true;
        break;
    default:
        return false;
        break;
    }
}

size_t
get_current_address(VMState* vm, BracketStack* bracket_stack)
{
    for (ssize_t i = bracket_stack->length - 1; i >= 0; i--)
    {
        if (bracket_stack->data[i].symbol == '{')
        {
            return bracket_stack->data[i].procedure->instruction_array->length;
        }
    }
    return vm->all_procedures->data[0]->instruction_array->length;
}

void
append_instruction(VMState* vm, BracketStack* bracket_stack, Instruction instruction)
{
    for (ssize_t i = bracket_stack->length - 1; i >= 0; i--)
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
    for (ssize_t i = bracket_stack->length - 1; i >= 0; i--)
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

Instruction
get_instruction(VMState* vm, BracketStack* bracket_stack, const size_t address)
{
    for (ssize_t i = bracket_stack->length - 1; i >= 0; i--)
    {
        if (bracket_stack->data[i].symbol == '{')
        {
            InstructionStack* instruction_array = bracket_stack->data[i].procedure->instruction_array;
            return instruction_array->data[address];
        }
    }
    InstructionStack* instruction_array = vm->all_procedures->data[0]->instruction_array;
    return instruction_array->data[address];
}

void
overwrite_instruction(VMState* vm, BracketStack* bracket_stack, const size_t address, Instruction instruction)
{
    for (ssize_t i = bracket_stack->length - 1; i >= 0; i--)
    {
        if (bracket_stack->data[i].symbol == '{')
        {
            InstructionStack* instruction_array = bracket_stack->data[i].procedure->instruction_array;
            instruction_array->data[address] = instruction;
            return;
        }
    }
    InstructionStack* instruction_array = vm->all_procedures->data[0]->instruction_array;
    instruction_array->data[address] = instruction;
}

bool
try_simple_instruction(VMState* vm, BracketStack* bracket_stack, ReadBuffer* buffer)
{
    Instruction instruction;

    switch (ReadBuffer_peek(buffer))
    {
    case '^':
        instruction.type = iPushZero;
        break;
    case '+':
        instruction.type = iIncrement;
        break;
    case '-':
        instruction.type = iDecrement;
        break;
    case '*':
        instruction.type = iAdd;
        break;
    case '~':
        instruction.type = iSubtract;
        break;
    case '#':
        instruction.type = iEnd;
        instruction.argument = get_context(bracket_stack);
        instruction.optional.jump_address = get_context_jump_address(bracket_stack);
        break;
    case ':':
        instruction.type = iContinue;
        instruction.argument = get_context(bracket_stack);
        instruction.optional.jump_address = get_context_jump_address(bracket_stack);
        break;
    default:
        return false;
        break;
    }
    append_instruction(vm, bracket_stack, instruction);
    ReadBuffer_clear(buffer);
    return true;
}

bool
try_name_instruction(VMState* vm, BracketStack* bracket_stack, ReadBuffer* buffer)
{
    char first_char = buffer->buffer[0];
    if (!(is_a_letter(first_char)) && !(first_char == '_'))
    {
        return false;
    }
    char second_char = buffer->buffer[1];
    switch (second_char)
    {
    case '{':
        Procedure* new_procedure = Procedure_new(vm); 
        append_instruction(
            vm, bracket_stack,
            (Instruction)
            {
                .type = iDefineProcedure,
                .argument = first_char,
                .optional.procedure = new_procedure
            }
        );
        ProcedureStack_append(vm->all_procedures, new_procedure);
        BracketStack_append(
            bracket_stack,
            (BracketPos)
            {
                .symbol = '{',
                .address = get_current_address(vm, bracket_stack) - 1,
                .procedure = new_procedure
            }
        );
        break;
    case '(':
        append_instruction(
            vm, bracket_stack,
            (Instruction)
            {
                .type = iInfiniteStart,
                .argument = first_char
            }
        );
        BracketStack_append(
            bracket_stack,
            (BracketPos)
            {
                .symbol = '(',
                .address = get_current_address(vm, bracket_stack) - 1,
                .procedure = NULL
            }
        );
        break;
    case '[':
        append_instruction(
            vm, bracket_stack,
            (Instruction)
            {
                .type = iRepeatStart,
                .argument = first_char
            }
        );
        BracketStack_append(
            bracket_stack,
            (BracketPos)
            {
                .symbol = '[',
                .address = get_current_address(vm, bracket_stack) - 1,
                .procedure = NULL
            }
        );
        break;
    case '?':
        append_instruction(
            vm, bracket_stack,
            (Instruction)
            {
                .type = iConditionalStart,
                .argument = first_char
            }
        );
        BracketStack_append(
            bracket_stack,
            (BracketPos)
            {
                .symbol = '?',
                .address = get_current_address(vm, bracket_stack) - 1,
                .procedure = NULL
            }
        );
        break;
    default:
        return false;
        break;
    }
    ReadBuffer_clear(buffer);
    return true;
}

bool
try_argument_instruction(VMState* vm, BracketStack* bracket_stack, ReadBuffer* buffer)
{
    char first_char = buffer->buffer[0];
    char second_char = buffer->buffer[1];
    Instruction instruction;

    switch (first_char)
    {
    case '%':
        instruction.type = second_char != '_' ? iReverse : iReverseNull;
        break;
    case '=':
        instruction.type = second_char != '_' ? iAssign : iAssignNull;
        break;
    case '!':
        instruction.type = second_char != '_' ? iDelete : iDeleteNull;
        break;
    case '$':
        instruction.type = second_char != '_' ? iPush : iPushNull;
        break;
    case '&':
        instruction.type = second_char != '_' ? iAssignLocal : iAssignLocalNull;
        break;
    case '<':
        instruction.type = second_char != '_' ? iOutput : iOutputNull;
        break;
    case '>':
        instruction.type = second_char != '_' ? iInput : iInputNull;
        break;
    case '@':
        instruction.type = second_char != '_' ? iCall : iCallNull;
        break;
    default:
        return false;
        break;
    }
    instruction.argument = second_char;
    append_instruction(vm, bracket_stack, instruction);
    ReadBuffer_clear(buffer);
    return true;
}

bool
try_end_instruction(VMState* vm, BracketStack* bracket_stack, ReadBuffer* buffer)
{
    switch (ReadBuffer_peek(buffer))
    {
    case '}':
    case ')':
    case ']':
    case ';':
        break;
    default:
        return false;
        break;
    }
    Instruction instruction;
    size_t current_address = get_current_address(vm, bracket_stack);
    BracketPos start_bracket = BracketStack_pop(bracket_stack);

    switch (ReadBuffer_peek(buffer))
    {
    case '}':
        ReadBuffer_clear(buffer);
        return true;
        break;
    case ')':
        instruction.type = iInfiniteEnd;
        break;
    case ']':
        instruction.type = iRepeatEnd;
        break;
    case ';':
        instruction.type = iConditionalEnd;
        break;
    }
    Instruction start_instruction = get_instruction(vm, bracket_stack, start_bracket.address);
    start_instruction.optional.jump_address = current_address;
    overwrite_instruction(vm, bracket_stack, start_bracket.address, start_instruction);
    instruction.optional.jump_address = start_bracket.address;

    append_instruction(vm, bracket_stack, instruction);

    ReadBuffer_clear(buffer);
    return true;
}

bool
try_unknown_symbol(ReadBuffer* buffer)
{
    char current_char = ReadBuffer_peek(buffer);
    if (is_a_letter(current_char))
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
try_unexpected_end(BracketStack* bracket_stack, ReadBuffer* buffer)
{
    if (bracket_stack->length > 0)
    {
        return false;
    }
    switch (ReadBuffer_peek(buffer))
    {
    case '}':
    case ')':
    case ']':
    case ';':
        return true;
        break;
    default:
        return false;
        break;
    }
}

bool
try_invalid_end(BracketStack* bracket_stack, ReadBuffer* buffer)
{
    if (bracket_stack->length == 0)
    {
        return false;
    }
    char expected_bracket = bracket_stack->data[bracket_stack->length - 1].symbol;
    char end_bracket;

    switch (ReadBuffer_peek(buffer))
    {
    case '}':
        end_bracket = '{';
        break;
    case ')':
        end_bracket = '(';
        break;
    case ']':
        end_bracket = '[';
        break;
    case ';':
        end_bracket = '?';
        break;
    default:
        return false;
        break;
    }
    if (expected_bracket != end_bracket)
    {
        return true;
    }
    return false;
}

bool
try_name_error(ReadBuffer* buffer)
{
    char first_char = buffer->buffer[0];
    if (!(is_a_letter(first_char)) && !(first_char == '_'))
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
        return false;
        break;
    default:
        return true;
        break;
    }
}

bool
try_argument_error(ReadBuffer* buffer)
{
    char first_char = buffer->buffer[0];
    switch (first_char)
    {
    case '%':
    case '=':
    case '!':
    case '$':
    case '&':
    case '<':
    case '>':
    case '@':
        break;
    default:
        return false;
        break;
    }
    char second_char = buffer->buffer[1];
    if (!(is_a_letter(second_char)) && !(second_char == '_'))
    {
        return true;
    }
    return false;
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

    while (ReadBuffer_read(buffer))
    {

        if (try_comment(buffer))
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
                fprintf(
                    stderr,
                    "ERROR! Symbol '%c' is unknown.\n",
                    ReadBuffer_peek(buffer)
                );
                exit(EXIT_FAILURE);
            }
            if (try_unexpected_end(bracket_stack, buffer))
            {
                fprintf(
                    stderr,
                    "ERROR! Unexpected '%c'.\n",
                    ReadBuffer_peek(buffer)
                );
                exit(EXIT_FAILURE);
            }
            if (try_invalid_end(bracket_stack, buffer))
            {
                fprintf(
                    stderr,
                    "ERROR! Closing '%c' does not match opening '%c'.\n",
                    ReadBuffer_peek(buffer), bracket_stack->data[bracket_stack->length - 1].symbol
                );
                exit(EXIT_FAILURE);
            }
            if (try_end_instruction(vm, bracket_stack, buffer))
            {
                continue;
            }
        }
        else
        {
            if (try_name_error(buffer))
            {
                fprintf(
                    stderr,
                    "ERROR! Expected '{', '(', '[' or '?' after '%c', got '%c' instead.\n",
                    buffer->buffer[0], buffer->buffer[1]
                );
                exit(EXIT_FAILURE);
            }
            if (try_argument_error(buffer))
            {
                fprintf(
                    stderr,
                    "ERROR! Expected a name or '_' after '%c', got '%c' instead.\n",
                    buffer->buffer[0], buffer->buffer[1]
                );
                exit(EXIT_FAILURE);
            }
            if (try_name_instruction(vm, bracket_stack, buffer))
            {
                continue;
            }
            if (try_argument_instruction(vm, bracket_stack, buffer))
            {
                continue;
            }
            fprintf(
                stderr,
                "ERROR! Could not form an instruction: '%c%c'.\n",
                buffer->buffer[0], buffer->buffer[1]
            );
            exit(EXIT_FAILURE);
        }
    }

    if (bracket_stack->length > 0)
    {
        fprintf(
            stderr,
            "ERROR! Bracket '%c' was never closed.\n",
            bracket_stack->data[bracket_stack->length - 1].symbol
        );
        exit(EXIT_FAILURE);
    }

    ReadBuffer_free(buffer);
    BracketStack_free(bracket_stack);
    return vm;
}
