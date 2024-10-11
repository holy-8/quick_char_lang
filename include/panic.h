#include <stdlib.h>

#ifndef __PANIC_H_
#define __PANIC_H_

void* panicMalloc(size_t size, char* note);

void* panicCalloc(size_t count, size_t size, char* note);

void* panicRealloc(void *reference, size_t newSize, char* note);

#endif /* __PANIC_H_ */
