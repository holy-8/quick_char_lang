#pragma once

#include <stdlib.h>

#include "DataTypes.h"

ShortStack*
ShortStack_new(const size_t capacity);


void
ShortStack_free(ShortStack* this);


void
ShortStack_resize(ShortStack* this, const size_t capacity);


void
ShortStack_append(ShortStack* this, const short item);


short
ShortStack_pop(ShortStack* this);


void
ShortStack_extend(ShortStack* this, ShortStack* other);


ShortStack*
ShortStack_slice(ShortStack* this, const size_t size);


ShortStack*
ShortStack_reversed(ShortStack* this);


ShortStack*
ShortStack_reversed_size(ShortStack* this, const size_t size);

/* Generated from src/datatypes/ShortStack.c */
