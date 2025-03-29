What's this
===========
I've made an interpreted language ([CCL](https://github.com/holy-8/cool_char_lang)).
This is a work-in-progress C implementation of the interpreter.

To be more exact, this is a second attempt (You can find initial code at `old` branch).
And i feel like it's much worse than the first one... I mean, yes, it is.
But i just want to finish this thing.

Now it even builds.

How to build
============
You need to have `cmake` installed.
Open terminal in the root directory, and execute the following commands:
```
cmake -B build
cmake --build build
```
This will build the binary, which ends up here: `build/debug/QCL.exe`.
That's it.
