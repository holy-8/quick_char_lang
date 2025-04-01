#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DataTypes.h"
#include "Parser.h"

char*
repr_instruction_type(InstructionType type);


void
repr_instruction(const size_t index, Instruction instruction);


void
repr_procedure(const size_t index, Procedure* procedure);


void
showstack(FILE* file);

/* Generated from src/interpreter/ShowStack.c */
