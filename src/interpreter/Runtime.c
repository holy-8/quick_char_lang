#include <stdio.h>
#include <stdlib.h>

#include "DataTypes.h"
#include "Parser.h"
#include "ShowStack.h"
#include "ProcedureStack.h"
#include "ShortStack.h"

char
get_index(const char name)
{
    if (('A' <= name) && (name <= 'Z'))
    {
        return name - 65;
    }
    return name - 71;
}

char
get_name(const char index)
{
    if (index < 26)
    {
        return index + 65;
    }
    return index + 71;
}

bool
is_global_scope(VMState* vm)
{
    return (bool) (vm->call_stack->length <= 1);
}

Procedure*
get_current_procedure(VMState* vm)
{
    return vm->call_stack->data[vm->call_stack->length - 1];
}

void
assert_stack_length(VMState* vm, const int length, Instruction instruction)
{
    if (vm->stack->length < length)
    {
        fprintf(
            stderr,
            "ERROR! Instruction %s Expected %d elements on the stack, got %lld instead.\n",
            repr_instruction_type(instruction.type), length, vm->stack->length
        );
        exit(EXIT_FAILURE);
    }
}

short
get_variable_value(VMState* vm, Procedure* procedure, Instruction instruction)
{
    char index = get_index(instruction.argument);

    if (procedure->local_vars[index].is_defined)
    {
        return procedure->local_vars[index].value;
    }
    if (vm->global_vars[index].is_defined)
    {
        return vm->global_vars[index].value;
    }

    fprintf(
        stderr,
        "ERROR! Instruction %s tried to access undefined variable '%c'.\n",
        repr_instruction_type(instruction.type), instruction.argument
    );
    exit(EXIT_FAILURE);
}

void
set_variable(VMState* vm, Procedure* procedure, Instruction instruction, short value)
{
    char index = get_index(instruction.argument);

    if (procedure->local_vars[index].is_defined)
    {
        procedure->local_vars[index].value = value;
        return;
    }

    vm->global_vars[index].is_defined = true;
    vm->global_vars[index].value = value;
}

void
delete_variable(VMState* vm, Procedure* procedure, Instruction instruction)
{
    char index = get_index(instruction.argument);

    if (procedure->local_vars[index].is_defined)
    {
        procedure->local_vars[index].is_defined = false;
        procedure->local_vars[index].value = 0;
        return;
    }
    if (vm->global_vars[index].is_defined)
    {
        vm->global_vars[index].is_defined = false;
        vm->global_vars[index].value = 0;
        return;
    }

    fprintf(
        stderr,
        "ERROR! Instruction %s tried to access undefined variable '%c'.\n",
        repr_instruction_type(instruction.type), instruction.argument
    );
    exit(EXIT_FAILURE);
}

void
create_local_variable(VMState* vm, Procedure* procedure, Instruction instruction)
{
    if (is_global_scope(vm))
    {
        fprintf(stderr, "ERROR! Local variables cannot be creates in global scope.\n");
        exit(EXIT_FAILURE);
    }
    char index = get_index(instruction.argument);

    procedure->local_vars[index].is_defined = true;
    procedure->local_vars[index].value = 0;
}

void
global_variables_to_local(VMState* vm, Procedure* procedure)
{
    if (is_global_scope(vm))
    {
        fprintf(stderr, "ERROR! Local variables cannot be creates in global scope.\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < 52; i++)
    {
        if (vm->global_vars[i].is_defined)
        {
            procedure->local_vars[i].is_defined = true;
            procedure->local_vars[i].value = vm->global_vars[i].value;
        }
    }
}

void
clear_variables_in_scope(VMState* vm, Procedure* procedure)
{
    if (is_global_scope(vm))
    {
        for (size_t i = 0; i < 52; i++)
        {
            vm->global_vars[i].is_defined = false;
            vm->global_vars[i].value = 0;
        }
    }
    else
    {
        for (size_t i = 0; i < 52; i++)
        {
            procedure->local_vars[i].is_defined = false;
            procedure->local_vars[i].value = 0;
        }
    }
}

void
modify_stack_top(VMState* vm, const short change)
{
    ShortStack* stack = vm->stack; short* data = stack->data;
    data[stack->length - 1] = (short) (data[stack->length - 1] + change);
}

void
reverse_stack(VMState* vm, const size_t length)
{
    ShortStack* reversed = ShortStack_reversed_size(vm->stack, length);
    ShortStack_free(vm->stack);
    vm->stack = reversed;
}

void
define_procedure(VMState* vm, Instruction instruction)
{
    char index = get_index(instruction.argument);

    vm->procedures[index].is_defined = true;
    vm->procedures[index].value = instruction.optional.procedure;
}

Procedure*
get_procedure(VMState* vm, Instruction instruction)
{
    char index = get_index(instruction.argument);

    if (vm->procedures[index].is_defined)
    {
        return vm->procedures[index].value;
    }

    fprintf(
        stderr,
        "ERROR! Instruction %s tried to access undefined procedure '%c'.\n",
        repr_instruction_type(instruction.type), instruction.argument
    );
    exit(EXIT_FAILURE);
}

void
recursive_call(VMState* vm, Procedure* procedure)
{
    if (is_global_scope(vm))
    {
        fprintf(stderr, "ERROR! Recursive call cannot be done outside of a procedure.\n");
        exit(EXIT_FAILURE);
    }

    ProcedureStack_append(vm->call_stack, Procedure_copy(procedure));
}

void
start_repeat(VMState* vm, Procedure* procedure, Instruction* instruction, bool is_null)
{
    short argument;
    if (instruction->optional.counter == 0)
    {
        if (is_null)
        {
            assert_stack_length(vm, 1, *instruction);
            argument = vm->stack->data[vm->stack->length - 1];
        }
        else
        {
            argument = get_variable_value(vm, procedure, *instruction);
        }
        if (argument <= 0)
        {
            procedure->instruction_pointer = instruction->optional.jump_address;
        }
        else
        {
            instruction->optional.counter = (size_t) argument;
        }
    }
    else
    {
        instruction->optional.counter--;
        if (instruction->optional.counter == 0)
        {
            procedure->instruction_pointer = instruction->optional.jump_address;
        }
    }
}

void
handle_end(VMState* vm, Procedure* procedure, Instruction instruction)
{
    Instruction* data;
    size_t jump_address;
    switch (instruction.argument)
    {
    case iNone:
        exit(EXIT_SUCCESS);
        break;
    case iDefineProcedure:
        Procedure* removed = ProcedureStack_pop(vm->call_stack);
        Procedure_free(removed);
        break;
    case iRepeatStart:
        data = procedure->instruction_array->data;
        jump_address = instruction.optional.jump_address;
        data[jump_address].optional.counter = 0;
        procedure->instruction_pointer = data[jump_address].optional.jump_address;
        break;
    case iInfiniteStart:
        data = procedure->instruction_array->data;
        jump_address = instruction.optional.jump_address;
        procedure->instruction_pointer = data[jump_address].optional.jump_address;
        break;
    default:
        fprintf(
            stderr,
            "ERROR! iEnd has unknown context %d.\n",
            instruction.argument
        );
        exit(EXIT_FAILURE);
        break;
    }
}

void
handle_continue(VMState* vm, Procedure* procedure, Instruction instruction)
{
    switch (instruction.argument)
    {
    case iNone:
    case iDefineProcedure:
        fprintf(stderr, "ERROR! iContinue outside of a loop.\n");
        exit(EXIT_FAILURE);
        break;
    case iRepeatStart:
    case iInfiniteStart:
        procedure->instruction_pointer = instruction.optional.jump_address - 1;
        break;
    default:
        fprintf(
            stderr,
            "ERROR! iContinue has unknown context %d.\n",
            instruction.argument
        );
        exit(EXIT_FAILURE);
        break;
    }
}

void
execute_instruction(VMState* vm, Procedure* procedure, Instruction* instruction)
{
    short argument;
    Procedure* procedure_argument;
    switch (instruction->type)
    {
    case iNone:
        break;
    case iPushZero:
        ShortStack_append(vm->stack, 0);
        break;
    case iIncrement:
        assert_stack_length(vm, 1, *instruction);
        modify_stack_top(vm, 1);
        break;
    case iDecrement:
        assert_stack_length(vm, 1, *instruction);
        modify_stack_top(vm, -1);
        break;
    case iAdd:
        assert_stack_length(vm, 2, *instruction);
        modify_stack_top(vm, ShortStack_pop(vm->stack));
        break;
    case iSubtract:
        assert_stack_length(vm, 2, *instruction);
        modify_stack_top(vm, -ShortStack_pop(vm->stack));
        break;
    case iReverse:
        argument = get_variable_value(vm, procedure, *instruction);
        assert_stack_length(vm, argument, *instruction);
        reverse_stack(vm, (size_t) argument);
        break;
    case iReverseNull:
        reverse_stack(vm, vm->stack->length);
        break;
    case iAssign:
        assert_stack_length(vm, 1, *instruction);
        set_variable(vm, procedure, *instruction, ShortStack_pop(vm->stack));
        break;
    case iAssignNull:
        assert_stack_length(vm, 1, *instruction);
        ShortStack_pop(vm->stack);
        break;
    case iDelete:
        delete_variable(vm, procedure, *instruction);
        break;
    case iDeleteNull:
        clear_variables_in_scope(vm, procedure);
        break;
    case iPush:
        argument = get_variable_value(vm, procedure, *instruction);
        ShortStack_append(vm->stack, argument);
        break;
    case iPushNull:
        assert_stack_length(vm, 1, *instruction);
        ShortStack_append(vm->stack, vm->stack->data[vm->stack->length - 1]);
        break;
    case iAssignLocal:
        create_local_variable(vm, procedure, *instruction);
        break;
    case iAssignLocalNull:
        global_variables_to_local(vm, procedure);
        break;
    case iOutput:
        argument = get_variable_value(vm, procedure, *instruction);
        putc(argument, stdout);
        break;
    case iOutputNull:
        assert_stack_length(vm, 1, *instruction);
        putc(vm->stack->data[vm->stack->length - 1], stdout);
        break;
    case iInput:
        get_variable_value(vm, procedure, *instruction);
        set_variable(vm, procedure, *instruction, (short) getc(stdin));
        break;
    case iInputNull:
        ShortStack_append(vm->stack, (short) getc(stdin));
        break;
    case iDefineProcedure:
        define_procedure(vm, *instruction);
        break;
    case iDefineProcedureNull:
        ProcedureStack_append(vm->call_stack, Procedure_copy(instruction->optional.procedure));
        break;
    case iCall:
        procedure_argument = get_procedure(vm, *instruction);
        ProcedureStack_append(vm->call_stack, Procedure_copy(procedure_argument));
        break;
    case iCallNull:
        recursive_call(vm, procedure);
        break;
    case iInfiniteStart:
        if (get_variable_value(vm, procedure, *instruction) <= 0)
        {
            procedure->instruction_pointer = instruction->optional.jump_address;
        }
        break;
    case iInfiniteStartNull:
        break;
    case iInfiniteEnd:
        procedure->instruction_pointer = instruction->optional.jump_address - 1;
        break;
    case iRepeatStart:
        start_repeat(vm, procedure, instruction, false);
        break;
    case iRepeatStartNull:
        start_repeat(vm, procedure, instruction, true);
        break;
    case iRepeatEnd:
        procedure->instruction_pointer = instruction->optional.jump_address - 1;
        break;
    case iEnd:
        handle_end(vm, procedure, *instruction);
        break;
    case iContinue:
        handle_continue(vm, procedure, *instruction);
        break;
    case iConditionalStart:
        argument = get_variable_value(vm, procedure, *instruction);
        assert_stack_length(vm, 1, *instruction);
        if (argument != vm->stack->data[vm->stack->length - 1])
        {
            procedure->instruction_pointer = instruction->optional.jump_address;
        }
        break;
    case iConditionalStartNull:
        assert_stack_length(vm, 2, *instruction);
        if (vm->stack->data[vm->stack->length - 1] != vm->stack->data[vm->stack->length - 2])
        {
            procedure->instruction_pointer = instruction->optional.jump_address;
        }
        break;
    case iConditionalEnd:
        break;
    default:
        fprintf(
            stderr,
            "ERROR! Attempt to execute unknown instruction type %d.\n",
            instruction->type
        );
        exit(EXIT_FAILURE);
        break;
    }
}

void
execute_next_instruction(VMState* vm, Procedure* procedure)
{
    if (procedure->instruction_pointer >= (long long) procedure->instruction_array->length)
    {
        Procedure* removed = ProcedureStack_pop(vm->call_stack);
        Procedure_free(removed);
        return;
    }
    execute_instruction(
        vm, procedure, &(procedure->instruction_array->data[procedure->instruction_pointer])
    );
    procedure->instruction_pointer++;
}

void
execute(FILE* file, bool debug_info)
{
    VMState* vm = parse_file(file);
    Procedure* current_procedure;

    ProcedureStack_append(vm->call_stack, Procedure_copy(vm->all_procedures->data[0]));

    while (vm->call_stack->length > 0)
    {
        current_procedure = get_current_procedure(vm);
        execute_next_instruction(vm, current_procedure);
    }

    if (debug_info)
    {
        printf("Stack:\n");
        for (long long i = vm->stack->length - 1; i >= 0; i--)
        {
            printf("  %d\n", vm->stack->data[i]);
        }

        printf("Global variables:\n");
        for (char i = 0; i < 52; i++)
        {
            if (vm->global_vars[i].is_defined)
            {
                printf("  %c = %d\n", get_name(i), vm->global_vars[i].value);
            }
        }
        printf("Defined procedures:\n");
        for (char i = 0; i < 52; i++)
        {
            if (vm->procedures[i].is_defined)
            {
                printf("  %c (address: %p)", get_name(i), vm->procedures[i].value);
            }
        }
    }

    VMState_free(vm);
}
