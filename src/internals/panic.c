#include <stdlib.h>
#include <stdio.h>

void* panicMalloc(size_t size, char* note) {
    void* memory = malloc(size);
    if (memory == NULL) {
        printf("\n[PANIC] malloc failed (returned NULL)\n");
        if (note != NULL) printf("%s\n", note);
        exit(EXIT_FAILURE);
    }
    return memory;
}

void* panicCalloc(size_t count, size_t size, char* note) {
    void* memory = calloc(count, size);
    if (memory == NULL) {
        printf("\n[PANIC] calloc failed (returned NULL)\n");
        if (note != NULL) printf("%s\n", note);
        exit(EXIT_FAILURE);
    }
    return memory;
}

void* panicRealloc(void *reference, size_t newSize, char* note) {
    void* memory = realloc(reference, newSize);
    if (memory == NULL) {
        printf("\n[PANIC] realloc failed (returned NULL)\n");
        if (note != NULL) printf("%s\n", note);
        exit(EXIT_FAILURE);
    }
    return memory;
}