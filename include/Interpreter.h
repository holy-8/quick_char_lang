#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "DataTypes.h"
#include "Parser.h"

char
get_index(const char name);


void
repr_instruction(const size_t index, Instruction instruction);


void
repr_procedure(const size_t index, Procedure* procedure);


int
main(int argc, char** argv);

/* Generated from src/interpreter/Interpreter.c */
