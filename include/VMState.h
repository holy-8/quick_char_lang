#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "ProcedureStack.h"
#include "ShortStack.h"

typedef struct ProcedureVariable
{
    bool is_defined;
    Procedure* value;
} ProcedureVariable;

typedef struct VMState
{
    ShortStack* stack;
    Variable global_vars[52];
    ProcedureVariable procedures[52];
    ProcedureStack* call_stack;
    ProcedureStack* all_procedures;
} VMState;

VMState*
VMState_new(void);

void
VMState_free(VMState* this);
