#pragma once

#include <stdlib.h>

#include "ProcedureStack.h"

typedef struct BracketPos
{
    char symbol;
    size_t address;
    Procedure* procedure;
} BracketPos;

typedef struct BracketStack
{
    BracketPos* data;
    size_t cap;
    size_t size;
} BracketStack;

BracketStack*
BracketStack_new(const size_t cap);

void
BracketStack_free(BracketStack* this);

void
BracketStack_resize(BracketStack* this, const size_t cap);

void
BracketStack_append(BracketStack* this, const BracketPos item);

BracketPos
BracketStack_pop(BracketStack* this);
