#include <stdlib.h>
#include "panic.h"

typedef void (*ReferenceDestroyer)(void*);

typedef struct GarbageCollector {
    size_t size;
    size_t allocatedSize;
    size_t overflowIncrement;
    void** references;
    ReferenceDestroyer* referenceDestroyers;
} GarbageCollector;

GarbageCollector* GarbageCollectorNew(size_t initialSize, size_t overflowIncrement) {
    GarbageCollector* this = panicMalloc(sizeof(GarbageCollector), "at file garbagecollector.c, line 15");
    this->size = 0;
    this->allocatedSize = initialSize;
    this->overflowIncrement = overflowIncrement;
    this->references = panicMalloc(sizeof(void*) * this->allocatedSize, "at file garbagecollector.c, line 19");
    this->referenceDestroyers = panicMalloc(sizeof(ReferenceDestroyer) * this->allocatedSize, "at file garbagecollector.c, line 20");
    return this;
}

void GarbageCollectorAddReference(GarbageCollector* this, void* reference, ReferenceDestroyer referenceDestroyer) {
    if (this->size >= this->allocatedSize) {
        this->allocatedSize += this->overflowIncrement;
        this->references = panicRealloc(this->references, sizeof(void*) * this->allocatedSize, "at file garbagecollector.c, line 27");
        this->referenceDestroyers = panicRealloc(this->referenceDestroyers, sizeof(ReferenceDestroyer) * this->allocatedSize, "at file garbagecollector.c, line 28");
    }
    this->references[this->size] = reference;
    this->referenceDestroyers[this->size] = referenceDestroyer;
    this->size++;
}

void GarbageCollectorClear(GarbageCollector* this) {
    if (this->size == 0)
        return;
    for (size_t i = 0; i < this->size; i++)
        this->referenceDestroyers[i](this->references[i]);
    this->size = 0;
}

void GarbageCollectorDestroy(GarbageCollector* this) {
    GarbageCollectorClear(this);
    free(this->references);
    free(this->referenceDestroyers);
    free(this);
}

void* shortNew(GarbageCollector* gc, short value) {
    short* obj = panicMalloc(sizeof(short), "at file garbagecollector.c, line 51");
    *obj = value;
    GarbageCollectorAddReference(gc, obj, free);
    return obj;
}
