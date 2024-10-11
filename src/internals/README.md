This directory contains implementation of certain internal objects and functions.

panic.c
=======
    Implements panic alloc functions.
    These end the program and write an error message upon failure.

garbagecollector.c
==================
    Implements garbage collection of objects allocated on heap.
    Provides functions to allocate some objects on the heap.

chardict.c
==========
    Implements a simple dictionary, where each key is a char.
    Value can by of any type (`void*`).

linkedlist.c
============
    Implements a linked list, each value can be of any type (`void*`).
    Also used as a stack.

stringio.c
==========
    Implements a string buffer, which changes its size when needed.

keyinfo.c
=========
    Implements a simple KeyInfo object.
    Since it uses `_getch()`, it only works on Windows.

readchar.c
==========
    Implements `readKey()` and `readChar()` functions.
    `readKey()` depends upon `_kbhit()`, so both functions will only work on Windows.

README.md
=========
    You're reading this now.
