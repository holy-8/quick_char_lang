#include <stdio.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "stringio.h"
#include "chardict.h"
#include "readchar.h"
#include "garbagecollector.h"


void main(void) {
    GarbageCollector* gc = GarbageCollectorNew(32, 32);

    short* num = shortNew(gc, 69);
    LinkedList* list = LinkedListNew(gc);
    CharDict* dict = CharDictNew(gc);
    StringIO* str = StringIONew(gc, 32, 32);

    GarbageCollectorDestroy(gc);
}
