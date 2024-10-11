#include <stdlib.h>
#include <conio.h>
#include "panic.h"

typedef struct KeyInfo {
    int primary;
    int secondary;
} KeyInfo;

KeyInfo* KeyInfoNew(void) {
    KeyInfo* this = panicMalloc(sizeof(this), "at file keyinfo.c, line 11");
    this->primary = _getch();
    this->secondary = 0;
    if (this->primary == 0 || this->primary == 224)
        this->secondary = _getch();
    return this;
}

void KeyInfoDestroy(KeyInfo* this) {
    free(this);
}
