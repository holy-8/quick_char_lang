#pragma once

#include <stdlib.h>

#include "DataTypes.h"

InstructionStack*
InstructionStack_new(const size_t capacity);


void
InstructionStack_free(InstructionStack* this);


void
InstructionStack_resize(InstructionStack* this, const size_t capacity);


void
InstructionStack_append(InstructionStack* this, Instruction item);

/* Generated from src/datatypes/InstructionStack.c */
