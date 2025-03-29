#include <stdlib.h>

typedef struct ShortStack
{
    short* data;
    size_t cap;
    size_t size;
} ShortStack;

ShortStack*
ShortStack_new(const size_t cap)
{
    ShortStack* this = calloc(1, sizeof(ShortStack));
    this->data = calloc(cap, sizeof(short));
    this->cap = cap;
    this->size = 0;
    return this;
}

void
ShortStack_free(ShortStack* this)
{
    free(this->data);
    free(this);
}

void
ShortStack_resize(ShortStack* this, const size_t cap)
{
    this->data = realloc(this->data, cap);
    this->cap = cap;
}

void
ShortStack_append(ShortStack* this, const short item)
{
    if (this->size >= this->cap)
    {
        ShortStack_resize(this, (size_t) this->cap * 1.5);
    }
    this->data[this->size] = item;
    this->size++;
}

short
ShortStack_pop(ShortStack* this)
{
    short value = this->data[this->size - 1];
    this->data[this->size - 1] = 0;
    this->size--;
    return value;
}

void
ShortStack_extend(ShortStack* this, ShortStack* other)
{
    for (size_t i = 0; i < other->size; i++)
    {
        ShortStack_append(this, other->data[i]);
    }
}

ShortStack*
ShortStack_slice(ShortStack* this, const size_t size)
{
    ShortStack* slice = ShortStack_new(this->cap);
    for (size_t i = 0; i < size; i++)
    {
        ShortStack_append(slice, this->data[this->size - size + i]);
    }
    return slice;
}

ShortStack*
ShortStack_reversed(ShortStack* this)
{
    ShortStack* reversed = ShortStack_new(this->cap);
    for (size_t i = this->size - 1; i >= 0; i++)
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
        return ShortStack_slice(this, this->size);
    }
    if (size == this->size)
    {
        return ShortStack_reversed(this);
    }
    ShortStack* copy = ShortStack_slice(this, this->size);

    ShortStack* slice = ShortStack_slice(copy, size);
    copy->size -= size;

    ShortStack* reversed_slice = ShortStack_reversed(slice);
    ShortStack_extend(copy, reversed_slice);

    ShortStack_free(slice);
    ShortStack_free(reversed_slice);

    return copy;
}
