#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "DataTypes.h"
#include "Parser.h"
#include "ShowStack.h"
#include "ProcedureStack.h"
#include "ShortStack.h"

char
get_index(const char name);


char
get_name(const char index);


bool
is_global_scope(VMState* vm);


Procedure*
get_current_procedure(VMState* vm);


void
assert_stack_length(VMState* vm, const int length, Instruction instruction);


short
get_variable_value(VMState* vm, Procedure* procedure, Instruction instruction);


void
set_variable(VMState* vm, Procedure* procedure, Instruction instruction, short value);


void
delete_variable(VMState* vm, Procedure* procedure, Instruction instruction);


void
create_local_variable(VMState* vm, Procedure* procedure, Instruction instruction);


void
global_variables_to_local(VMState* vm, Procedure* procedure);


void
clear_variables_in_scope(VMState* vm, Procedure* procedure);


void
modify_stack_top(VMState* vm, const short change);


void
reverse_stack(VMState* vm, const size_t length);


void
define_procedure(VMState* vm, Instruction instruction);


Procedure*
get_procedure(VMState* vm, Instruction instruction);


void
recursive_call(VMState* vm, Procedure* procedure);


void
start_repeat(VMState* vm, Procedure* procedure, Instruction* instruction, bool is_null);


void
handle_end(VMState* vm, Procedure* procedure, Instruction instruction);


void
handle_continue(VMState* vm, Procedure* procedure, Instruction instruction);


void
execute_instruction(VMState* vm, Procedure* procedure, Instruction* instruction);


void
execute_next_instruction(VMState* vm, Procedure* procedure);


void
execute(FILE* file, bool debug_info);

/* Generated from src/interpreter/Runtime.c */
