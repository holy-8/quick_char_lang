This directory contains implementation of certain internal objects and functions.

`heap/panic.c`
=======
Implements panic alloc functions.
These end the program and write an error message upon failure.

`heap/garbagecollector.c`
==================
Implements garbage collection of objects allocated on heap.

`heap/heapobjects.c`
==================
Provides functions to allocate some objects on the heap.

`data_structures/chardict.c`
==========
Implements a simple dictionary, where each key is a char.
Value can by of any type (`void*`).

`data_structures/linkedlist.c`
============
Implements a linked list, each value can be of any type (`void*`).
Also used as a stack.

`data_structures/stringio.c`
==========
Implements a string buffer, which changes its size when needed.

`io/keyinfo.c`
=========
Implements a simple KeyInfo object.
Since it uses `_getch()`, it only works on Windows.

`io/readchar.c`
==========
Implements `readKey()` and `readChar()` functions.
`readKey()` depends upon `_kbhit()`, so both functions will only work on Windows.

README.md
=========
You're reading this now.
