#pragma once

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
is_a_letter(const char name);


bool
try_comment(ReadBuffer* buffer);


size_t
get_current_address(VMState* vm, BracketStack* bracket_stack);


void
append_instruction(VMState* vm, BracketStack* bracket_stack, Instruction instruction);


InstructionType
get_context(BracketStack* bracket_stack);


size_t
get_context_jump_address(BracketStack* bracket_stack, const InstructionType lookup_type);


Instruction
get_instruction(VMState* vm, BracketStack* bracket_stack, const size_t address);


void
overwrite_instruction(VMState* vm, BracketStack* bracket_stack, const size_t address, Instruction instruction);


bool
try_simple_instruction(VMState* vm, BracketStack* bracket_stack, ReadBuffer* buffer);


bool
try_name_instruction(VMState* vm, BracketStack* bracket_stack, ReadBuffer* buffer);


bool
try_argument_instruction(VMState* vm, BracketStack* bracket_stack, ReadBuffer* buffer);


bool
try_end_instruction(VMState* vm, BracketStack* bracket_stack, ReadBuffer* buffer);


bool
try_unknown_symbol(ReadBuffer* buffer);


bool
try_unexpected_end(BracketStack* bracket_stack, ReadBuffer* buffer);


bool
try_invalid_end(BracketStack* bracket_stack, ReadBuffer* buffer);


bool
try_name_error(ReadBuffer* buffer);


bool
try_argument_error(ReadBuffer* buffer);


VMState*
parse_file(FILE* file);

/* Generated from src/interpreter/Parser.c */
