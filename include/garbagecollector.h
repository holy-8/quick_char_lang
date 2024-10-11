#include <stdlib.h>

#ifndef __GARBAGECOLLECTOR_H_
#define __GARBAGECOLLECTOR_H_

typedef void (*ReferenceDestroyer)(void*);

typedef struct GarbageCollector {
    size_t size;
    size_t allocatedSize;
    size_t overflowIncrement;
    void** references;
    ReferenceDestroyer* referenceDestroyers;
} GarbageCollector;

GarbageCollector* GarbageCollectorNew(size_t initialSize, size_t overflowIncrement);

void GarbageCollectorAddReference(GarbageCollector* this, void* reference, ReferenceDestroyer referenceDestroyer);

void GarbageCollectorClear(GarbageCollector* this);

void GarbageCollectorDestroy(GarbageCollector* this);

void* shortNew(GarbageCollector* gc, short value);

#endif /* __GARBAGECOLLECTOR_H_ */
