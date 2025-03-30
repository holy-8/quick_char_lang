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
try_comment(bool* is_comment, ReadBuffer* buffer);


void
append_instruction(VMState* vm, BracketStack* bracket_stack, Instruction instruction);


InstructionType
get_context(BracketStack* bracket_stack);


size_t
get_context_jump_address(BracketStack* bracket_stack);


bool
try_simple_instruction(VMState* vm, BracketStack* bracket_stack, ReadBuffer* buffer);

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
try_unknown_symbol(ReadBuffer* buffer);


bool
try_name_error(ReadBuffer* buffer);


VMState*
parse_file(FILE* file);

/* Generated from src/interpreter/Parser.c */
