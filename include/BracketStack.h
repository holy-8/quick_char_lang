#pragma once

#include <stdlib.h>

#include "DataTypes.h"

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
