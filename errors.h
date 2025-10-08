#ifndef ERRORS_H
#define ERRORS_H


#include "general_header.h"

/** Enum including all error numbers with their value */
typedef enum ERROR_NUMBERS {
    ERROR_0 = 0,
    /* No value is provided => value will be one more than the previous enum value */
    ERROR_1,
    ERROR_2,
    ERROR_3,
    ERROR_4,
    ERROR_5,
    ERROR_6,
    ERROR_7,
    ERROR_8,
    ERROR_9,
    ERROR_10,
    ERROR_11,
    ERROR_12,
    ERROR_13,
    ERROR_14,
    ERROR_15,
    ERROR_16,
    ERROR_17,
    ERROR_18,
    ERROR_19,
    ERROR_20,
    ERROR_21,
    ERROR_22,
    ERROR_23,
    ERROR_24,
    ERROR_25,
    ERROR_26,
    ERROR_27,
    ERROR_28,
    ERROR_29,
    ERROR_30,
    ERROR_31,
    ERROR_32,
    ERROR_33,
    ERROR_34,
    ERROR_35,
    ERROR_36,
    ERROR_37,
    ERROR_38,
    ERROR_39,
    ERROR_40
} ERROR_NUMBERS;

/** Error structure that contains an error with its message */
typedef struct Error {
    int error_number;
    char *error_message;
} Error;

/** A global variable to store the current error number */
extern int current_error_number;
/** A global variable to store the current line number (in the ".am" file [after the macro deployment]) */
extern int current_line_number;


/**
 * Prints an error message based on the error number, line number, file name.
 *
 * Input:
 *   - error_number: Integer representing the specific error that occurred
 *   - stage: Integer indicating the assembly stage when the error occurred in order to print the file name where the error occurred
 *
 * Output:
 *   - No return value
 */
void print_error(int error_number, int stage);



/**
 * Safely reallocates memory, handling any allocation failures.
 *
 * Input:
 *   - ptr: Pointer to the memory block to be reallocated
 *   - size: New size in bytes for the memory block
 *
 * Output:
 *   - Returns a pointer to the reallocated memory block on success
 *   - Terminates the program with an error message if allocation fails
 */
void* safe_realloc(void *ptr, size_t size);


/**
 * Safely allocates memory, handling any allocation failures.
 *
 * Input:
 *   - size: Size in bytes for the memory block to allocate
 *
 * Output:
 *   - Returns a pointer to the allocated memory block on success
 *   - Terminates the program with an error message if allocation fails
 */
void* safe_malloc(size_t size);


/**
 * Safely opens a file, handling any opening failures.
 *
 * Input:
 *   - filename: Path to the file to be opened
 *   - mode: String specifying the mode for file access ("r" for read, "w" for write)
 *
 * Output:
 *   - Returns a FILE pointer to the opened file on success
 *   - Terminates the program with an error message if opening fails
 */
FILE* safe_fopen(char *filename, char *mode);


#endif /* ERRORS_H */