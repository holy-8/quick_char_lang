cmake_minimum_required(VERSION 3.5)
project(quick_char_lang VERSION 1.0 LANGUAGES C)

add_executable(
    "QCL"
    src/interpreter/main.c
    src/internals/data_structures/chardict.c
    src/internals/data_structures/linkedlist.c
    src/internals/data_structures/stringio.c
    src/internals/heap/garbagecollector.c
    src/internals/heap/heapobjects.c
    src/internals/heap/panic.c
    src/internals/io/keyinfo.c
    src/internals/io/readchar.c
)

target_include_directories("QCL" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
