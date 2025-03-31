#include <stdlib.h>

#include "DataTypes.h"

ShortStack*
ShortStack_new(const size_t capacity)
{
    ShortStack* this = calloc(1, sizeof(ShortStack));
    this->data = calloc(capacity, sizeof(short));
    this->capacity = capacity;
    this->length = 0;
    return this;
}

void
ShortStack_free(ShortStack* this)
{
    free(this->data);
    free(this);
}

void
ShortStack_resize(ShortStack* this, const size_t capacity)
{
    this->data = realloc(this->data, capacity * sizeof(short));
    this->capacity = capacity;
}

void
ShortStack_append(ShortStack* this, const short item)
{
    if (this->length >= this->capacity)
    {
        ShortStack_resize(this, (size_t) (this->capacity * 1.5));
    }
    this->data[this->length] = item;
    this->length++;
}

short
ShortStack_pop(ShortStack* this)
{
    short value = this->data[this->length - 1];
    this->length--;
    return value;
}

void
ShortStack_extend(ShortStack* this, ShortStack* other)
{
    for (size_t i = 0; i < other->length; i++)
    {
        ShortStack_append(this, other->data[i]);
    }
}

ShortStack*
ShortStack_slice(ShortStack* this, const size_t size)
{
    ShortStack* slice = ShortStack_new(this->capacity);
    for (size_t i = 0; i < size; i++)
    {
        ShortStack_append(slice, this->data[this->length - size + i]);
    }
    return slice;
}

ShortStack*
ShortStack_reversed(ShortStack* this)
{
    ShortStack* reversed = ShortStack_new(this->capacity);
    for (size_t i = this->length - 1; i >= 0; i++)
    {
        ShortStack_append(reversed, this->data[i]);
    }
    return reversed;
}

ShortStack*
ShortStack_reversed_size(ShortStack* this, const size_t size)
{
    if (size < 2)
    {
        return ShortStack_slice(this, this->length);
    }
    if (size == this->length)
    {
        return ShortStack_reversed(this);
    }
    ShortStack* copy = ShortStack_slice(this, this->length);

    ShortStack* slice = ShortStack_slice(copy, size);
    copy->length -= size;

    ShortStack* reversed_slice = ShortStack_reversed(slice);
    ShortStack_extend(copy, reversed_slice);

    ShortStack_free(slice);
    ShortStack_free(reversed_slice);

    return copy;
}
