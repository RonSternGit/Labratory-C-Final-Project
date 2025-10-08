#ifndef GENERAL_HEADER_H
#define GENERAL_HEADER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MACRO_START "mcro"                          /** Valid syntax of the start of a macro */
#define MACRO_END "mcroend"                         /** Valid syntax of the end of a macro */
#define BIG_INTEGER 1000                            /** Declared in order to check validity of line length */
#define MAX_LINE_LENGTH 81                          /** 80 allowed chars +1 for \n */
#define MAX_LABEL_LENGTH 31                         /** Maximum length of a label in the assembly language */
#define OPCODE_NUM_OF_BITS 6                        /** Number of bits for the 'opcode' */
#define SOURCE_ADDRESSING_MODE_NUM_OF_BITS 2        /** Number of bits for the 'source addressing mode' */
#define SOURCE_REGISTER_NUM_OF_BITS 3               /** Number of bits for the 'source register' */
#define DESTINATION_ADDRESSING_MODE_NUM_OF_BITS 2   /** Number of bits for the 'destination addressing mode' */
#define DESTINATION_REGISTER_NUM_OF_BITS 3          /** Number of bits for the 'destination register' */
#define FUNCT_NUM_OF_BITS 5                         /** Number of bits for the 'funct' */
#define A_NUM_OF_BITS 1                             /** Number of bits for the 'A' */
#define R_NUM_OF_BITS 1                             /** Number of bits for the 'R' */
#define E_NUM_OF_BITS 1                             /** Number of bits for the 'E' */
#define ADDITIONAL_WORD_LENGTH_IN_BITS 21           /** Number of bits in the main field in an additional word (in the code/data table) */
#define NUM_OF_ERRORS 41                            /** Number of possible errors in the assembly language */
#define WORD_SIZE 24                                /** The name "word" is defined as a memory cell */
#define NUM_OF_INSTRUCTIONS 16                      /** Number of instructions in the assembly language */
#define NUM_OF_DIRECTIVES 4                         /** Number of directives in the assembly language */
#define NUM_OF_REGISTERS 8                          /** Number of registers in the assembly language */
#define MAX_NUM_OF_VALID_ADDRESSING_MODES_FOR_PARAMETER 3  /** Maximum number of valid addressing modes for a parameter */
#define CODE_TYPE "code"                            /** Attribute name for the code table */
#define DATA_TYPE "data"                            /** Attribute name for the data table */
#define ENTRY_TYPE "entry"                          /** Attribute name for the entry table */
#define EXTERN_TYPE "external"                      /** Attribute name for the external table */
#define HEX_STR_SIZE 7                              /** 6 digits + '\0' */
#define MAX_NUM_OF_WORDS 2097152                    /** 2^21 */
#define GARBAGE_VALUE 2389325                       /** A garbage value for return purposes */
#define ASCII_MIN 0                                 /** Minimum ASCII value */
#define ASCII_MAX 127                               /** Maximum ASCII value */
#define INITIAL_IC_VALUE 100                        /** Initial value for the Instruction Counter */
#define INITIAL_DC_VALUE 0                          /** Initial value for the Data Counter */
#define END_OF_ARRAY 4329419                        /** A garbage value to mark the end of an array (like '\0' does to a string) */
#define BINARY_BASE 2                               /** Base for binary numbers */
#define DECIMAL_BASE 10                             /** Base for decimal numbers */

/* Error stages for print_error function */
#define AS_FILE_STAGE 0
#define AM_FILE_STAGE 1
#define INTERNAL_ERROR_STAGE 2

/* Addressing mode constants */
#define IMMEDIATE_ADDRESSING_MODE 0
#define DIRECT_ADDRESSING_MODE 1
#define RELATIVE_ADDRESSING_MODE 2
#define DIRECT_REGISTER_ADDRESSING_MODE 3


#endif /* GENERAL_HEADER_H */