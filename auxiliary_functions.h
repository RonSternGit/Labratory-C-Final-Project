#ifndef AUXILIARY_FUNCTIONS_H
#define AUXILIARY_FUNCTIONS_H


#include <stdarg.h>

/**
 * Creates a file name with a specific extension based on input arguments.
 * This function uses variable argument lists (va_list) to concatenate multiple string components
 * into a single dynamically allocated string.
 * I used a variable argument list in order to create names based on any number of arguments I want.
 *
 * Input:
 *   - num_of_args: Number of arguments to be processed
 *   - first_arg: The first argument in the output name
 *   - ...: The other arguments
 *
 * Output:
 *   - Returns a pointer to a dynamically allocated string containing the constructed file name
 */
char* get_file_name(int num_of_args, char *first_arg, ...);

/**
 * Removes leading whitespaces from each line in the source file and writes to a new file.
 *
 * Input:
 *   - original_file_name: Path to the source file to be processed
 *   - trimmed_file_name: Path where the processed file will be saved
 *
 * Output:
 *   - No return value.
 */
void trim_leading_whitespaces(char *original_file_name, char *updated_file_name);


/**
 * Skips whitespace characters and commas in a string.
 * And updates ptr to point after whitespaces and commas.
 *
 * Input:
 *   - ptr: Pointer to the pointer of the current position in a string
 *
 * Output:
 *   - Returns the number of commas encountered
 */
int skip_whitespaces_and_commas(char **ptr);

/**
 * Checks if a string is a valid number representation (according to the definition in the course booklet).
 * Valid numbers must not start with leading zeros, must only contain digit characters (after an optional sign).
 *
 * Input:
 *   - ptr: String to validate as a number
 *
 * Output:
 *   - Returns 1 if the string is a valid number, 0 otherwise
 */
int is_valid_number(char *ptr);

/**
 * Validates if a string has correct label syntax (according to the definition in the course booklet).
 * A valid label must start with an alphabetic character and can only contain
 * alphabetic characters and digits after that. Labels have length restrictions and cannot
 * be identical to reserved words (such as register names, instructions, or directives).
 *
 * Input:
 *   - operand: String to check for valid label syntax
 *
 * Output:
 *   - Returns 1 if the operand has valid label syntax, 0 otherwise
 */
int is_valid_label_syntax(char *operand);

/**
 * Reverses a string in place swapping characters from opposite ends..
 *
 * Input:
 *   - string: String to be reversed
 *
 * Output:
 *   - No return value
 */
void reverse_string(char *string);

/**
 * Checks if a character is within ASCII range (between ASCII_MIN and ASCII_MAX constants).
 *
 * Input:
 *   - c: Character to check
 *
 * Output:
 *   - Returns 1 if character is in ASCII range, 0 otherwise
 */
int is_ascii(int c);

/**
 * Initializes a character array with '0' characters.
 *
 * Input:
 *   - array: Character array to initialize
 *   - array_length: Length of the array to initialize
 *
 * Output:
 *   - No return value
 */
void initialize_array_with_zeros(char *array, int array_length);

/**
 * Checks if there are any non-whitespace characters after the current position in the string.
 * If non-whitespace characters are found, it reports an error (ERROR_27) indicating extraneous text.
 * This function is used to ensure that no unexpected content exists after valid input.
 *
 * Input:
 *   - ptr: Pointer to the pointer of the current position in a string
 *
 * Output:
 *   - No return value
 */
void check_no_extra_chars(char **ptr);

/**
 * Checks if a string contains non-ASCII characters.
 *
 * Input:
 *   - name: String to check for non-ASCII characters
 *
 * Output:
 *   - Returns 1 if non-ASCII characters are found, 0 otherwise
 */
int contains_non_ascii_chars(char *name);


#endif /* AUXILIARY_FUNCTIONS_H */