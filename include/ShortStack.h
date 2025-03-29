#pragma once

#include <stdlib.h>

typedef struct ShortStack
{
    short* data;
    size_t cap;
    size_t size;
} ShortStack;

ShortStack*
ShortStack_new(const size_t cap);

void
ShortStack_free(ShortStack* this);

void
ShortStack_resize(ShortStack* this, const size_t cap);

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
