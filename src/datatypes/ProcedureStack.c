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
Procedure_new(VMState* vm)
{
    Procedure* this = calloc(1, sizeof(Procedure));
    this->vm = vm;
    for (size_t i = 0; i < 52; i++)
    {
        this->local_vars[i] = (Variable) { .is_defined = false };
    }
    this->iarr = InstructionStack_new(8);
    this->iptr = 0;
    return this;
}

void
Procedure_free(Procedure* this)
{
    InstructionStack_free(this->iarr);
    free(this);
}

Procedure*
Procedure_copy(Procedure* this)
{
    Procedure* copy = Procedure_new(this->vm);
    for (size_t i = 0; i < this->iarr->size; i++)
    {
        InstructionStack_append(copy->iarr, this->iarr->data[i]);
    }
    return copy;
}

typedef struct ProcedureStack
{
    Procedure** data;
    size_t cap;
    size_t size;
} ProcedureStack;

ProcedureStack*
ProcedureStack_new(const size_t cap)
{
    ProcedureStack* this = calloc(1, sizeof(ProcedureStack));
    this->data = calloc(cap, sizeof(Procedure*));
    this->cap = cap;
    this->size = 0;
    return this;
}

void
ProcedureStack_free(ProcedureStack* this)
{
    for (size_t i = 0; i < this->size; i++)
    {
        Procedure_free(this->data[i]);
    }
    free(this->data);
    free(this);
}

void
ProcedureStack_resize(ProcedureStack* this, const size_t cap)
{
    this->data = realloc(this->data, cap);
    this->cap = cap;
}

void
ProcedureStack_append(ProcedureStack* this, const Procedure* item)
{
    if (this->size >= this->cap)
    {
        ProcedureStack_resize(this, (size_t) this->cap * 1.5);
    }
    this->data[this->size] = item;
    this->size++;
}
