cmake_minimum_required(VERSION 3.29)
set(CMAKE_C_COMPILER gcc)
project(Project_2025 C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -ansi -pedantic")

add_executable(Project_2025 assembler.c
        first_pass.c
        second_pass.c
        pre_assembler.c
        errors.c
        auxiliary_functions.c
        general_header.h
        pre_assembler.h
        auxiliary_functions.h
        first_pass.h
        errors.h
        convertor.c
        convertor.h
        table.c
        table.h
        second_pass.h
        parser.c
        parser.h)
