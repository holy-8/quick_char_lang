#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "DataTypes.h"
#include "InstructionStack.h"

Procedure*
Procedure_new(VMState* vm);


void
Procedure_free(Procedure* this);


Procedure*
Procedure_copy(Procedure* this);


ProcedureStack*
ProcedureStack_new(const size_t capacity);


void
ProcedureStack_free(ProcedureStack* this);


void
ProcedureStack_resize(ProcedureStack* this, const size_t capacity);


void
ProcedureStack_append(ProcedureStack* this, Procedure* item);


Procedure*
ProcedureStack_pop(ProcedureStack* this);

/* Generated from src/datatypes/ProcedureStack.c */
