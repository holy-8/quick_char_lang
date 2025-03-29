#pragma once

#include <stdlib.h>

#include "ProcedureStack.h"

typedef enum InstructionType
{
    iPushZero,
    iIncrement,
    iDecrement,
    iAdd,
    iSubtract,
    iReverse,
    iAssign,
    iDelete,
    iPush,
    iAssignLocal,
    iOutput,
    iInput,
    iDefineProcedure,
    iCall,
    iInfiniteStart,
    iInfiniteEnd,
    iRepeatStart,
    iRepeatEnd,
    iEnd,
    iContinue,
    iConditionalStart,
    iConditionalEnd
} InstructionType;

typedef struct Instruction
{
    InstructionType type;
    char arg;
    union
    {
    Procedure* procedure;
    size_t jumpptr;
    size_t counter;
    } optional;
} Instruction;

typedef struct InstructionStack
{
    Instruction* data;
    size_t cap;
    size_t size;
} InstructionStack;

InstructionStack*
InstructionStack_new(const size_t cap);

void
InstructionStack_free(InstructionStack* this);

void
InstructionStack_resize(InstructionStack* this, const size_t cap);

void
InstructionStack_append(InstructionStack* this, const Instruction item);
