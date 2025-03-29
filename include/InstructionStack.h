#pragma once

#include <stdlib.h>

#include "DataTypes.h"

InstructionStack*
InstructionStack_new(const size_t cap);

void
InstructionStack_free(InstructionStack* this);

void
InstructionStack_resize(InstructionStack* this, const size_t cap);

void
InstructionStack_append(InstructionStack* this, Instruction item);
