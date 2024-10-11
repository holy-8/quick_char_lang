cmake_minimum_required(VERSION 3.5)
project(quick_char_lang VERSION 1.0 LANGUAGES C)

add_executable(
    "QCL"
    src/interpreter/main.c
    src/internals/stringio.c
    src/internals/readchar.c
    src/internals/keyinfo.c
    src/internals/chardict.c
    src/internals/linkedlist.c
    src/internals/garbagecollector.c
    src/internals/panic.c
)

target_include_directories("QCL" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
