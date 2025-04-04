#include <stdlib.h>
#include <stdbool.h>

#include "DataTypes.h"
#include "ShortStack.h"
#include "ProcedureStack.h"

VMState*
VMState_new(void)
{
    VMState* this = calloc(1, sizeof(VMState));
    this->stack = ShortStack_new(8);
    for (size_t i = 0; i < 52; i++)
    {
        this->global_vars[i] = (Variable) { .is_defined = false };
        this->procedures[i] = (ProcedureVariable) { .is_defined = false };
    }
    this->call_stack = ProcedureStack_new(8);
    this->all_procedures = ProcedureStack_new(8);
    return this;
}

void
VMState_free(VMState* this)
{
    ShortStack_free(this->stack);
    ProcedureStack_free(this->call_stack);
    ProcedureStack_free(this->all_procedures);
    free(this);
}
