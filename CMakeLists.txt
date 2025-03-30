cmake_minimum_required(VERSION 3.5)
project(quick_char_lang VERSION 1.0 LANGUAGES C)

add_executable(
    "QCL"
    src/interpreter/Interpreter.c
    src/interpreter/Parser.c
    src/interpreter/VMState.c
    src/datatypes/BracketStack.c
    src/datatypes/DataTypes.c
    src/datatypes/InstructionStack.c
    src/datatypes/ProcedureStack.c
    src/datatypes/ReadBuffer.c
    src/datatypes/ShortStack.c
)

target_include_directories("QCL" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
