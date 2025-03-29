#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "InstructionStack.h"
#include "VMState.h"

typedef struct Variable
{
    bool is_defined;
    short value;
} Variable;

typedef struct Procedure
{
    VMState* vm;
    Variable local_vars[52];
    InstructionStack* iarr;
    size_t iptr;
} Procedure;

Procedure*
Procedure_new(VMState* vm);

void
Procedure_free(Procedure* this);

Procedure*
Procedure_copy(Procedure* this);

typedef struct ProcedureStack
{
    Procedure** data;
    size_t cap;
    size_t size;
} ProcedureStack;

ProcedureStack*
ProcedureStack_new(const size_t cap);

void
ProcedureStack_free(ProcedureStack* this);

void
ProcedureStack_resize(ProcedureStack* this, const size_t cap);

void
ProcedureStack_append(ProcedureStack* this, const Procedure* item);
