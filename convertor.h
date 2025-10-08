#ifndef CONVERTOR_H
#define CONVERTOR_H


#include "first_pass.h"

/**
 * Adds a specified number of zeros to a binary word representation.
 * This function appends the required number of '0' characters to the word_in_binary string
 * at the position indicated by binary_index. It then updates the binary_index pointer to
 * point to the position immediately after the last zero added.
 *
 * Input:
 *   - word_in_binary: The string representing the binary word being constructed
 *   - binary_index: Pointer to the current position in the binary word
 *   - required_zeros: Number of '0' characters to add
 *
 * Output:
 *   - No return value
 */
void add_zeros(char *word_in_binary, int *binary_index, int required_zeros);


/**
 * Converts an integer value to its binary representation,
 * stores it in the provided string,
 * and updated the position of the binary_index pointer.
 *
 * Input:
 *   - num: Integer value to convert to binary
 *   - word_in_binary: String where the binary representation will be stored
 *   - binary_index: Pointer to current position in the binary word
 *
 * Output:
 *   - No return value
 */
void integer_to_binary(int num, char *word_in_binary, int *binary_index);


/**
 * Converts an instruction to its binary representation and stores it in the code array.
 * This function processes an encoded instruction structure and generates the appropriate
 * binary *words* for the instruction.
 *
 * Input:
 *   - instruction_line: Pointer to the encoded instruction to be converted
 *   - code: Pointer to the code array where the binary representation will be stored
 *   - IC: Pointer to the Instruction Counter, which will be updated as words are added
 *
 * Output:
 *   - No return value
 */
void instruction_to_binary(encoded_instruction **instruction_line, code_data_array **code, int *IC);


/**
 * Converts the first word of an instruction to its binary representation.
 * This function takes an encoded instruction and generates the binary code for the first word.
 *
 * Input:
 *   - instruction_line: Pointer to the pointer of the encoded instruction structure
 *   - word_in_binary: Character array where the binary representation will be stored
 *
 * Output:
 *   - No return value
 */
void first_word_to_binary(encoded_instruction **instruction_line, char *word_in_binary);


/**
 * Converts an operand word to its binary representation and adds it to the code array (if needed).
 *
 * Input:
 *   - instruction_line: Pointer to the pointer of the encoded instruction structure
 *   - code: Pointer to the code array where the binary representation will be stored
 *   - IC: Pointer to the Instruction Counter, which will be updated as words are added
 *   - word_in_binary: Character array where the binary representation will be stored
 *   - is_destination_operand: Indicates if the operand is a destination (1) or source (0) operand
 *
 * Output:
 *   - No return value
 */
void operand_word_to_binary(encoded_instruction **instruction_line, code_data_array **code, int *IC, char *word_in_binary, int is_destination_operand);


/**
 * Converts an immediate operand to its binary representation.
 * This function handles the conversion of immediate values (operands starting with '#')
 * to their binary representation in the word_in_binary string.
 *
 * Input:
 *   - instruction_line: Pointer to the pointer of the encoded instruction structure
 *   - word_in_binary: Character array where the binary representation will be stored
 *   - is_destination_operand: Indicates if the operand is a destination (1) or source (0) operand
 *
 * Output:
 *   - No return value
 */
void immediate_operand_word_to_binary(encoded_instruction **instruction_line, char *word_in_binary, int is_destination_operand);


/**
 * Converts a data directive to its binary representation and adds it to the data array.
 * This function processes the directive operands and converts them
 * to binary format according to the directive type (.data, .string, .entry, .extern).
 *
 * Input:
 *   - ptr: Pointer to the pointer of the line being processed
 *   - directive_name: String containing the directive name
 *   - data: Pointer to the data array where the binary representation will be stored
 *   - label_table: Pointer to the label table
 *   - DC: Pointer to the Data Counter, which will be updated as data words are added
 *   - label_table_lines: Pointer to the number of label table lines added
 *
 * Output:
 *   - No return value
 */
void directive_to_binary(char **ptr, char *directive_name, code_data_array **data, label_table **label_table, int *DC, int *label_table_lines);


/**
 * Parse and get the next directive number from the input string.
 *
 * Input:
 *   - ptr: Pointer to the pointer of the current position in the line being processed
 *   - num_of_commas_expected: The expected number of commas before this number (for error checking)
 *
 * Output:
 *   - Returns a dynamically allocated string containing the extracted number on success
 *   - Returns NULL if an error occurs during parsing
 */
char *get_next_directive_number(char **ptr, int num_of_commas_expected);


/**
 * Converts a directive number (number in a .data directive) to its binary representation.
 *
 * Input:
 *   - directive_number: String representing the number to be converted
 *   - word_in_binary: Character array where the binary representation will be stored
 *
 * Output:
 *   - No return value
 */
void directive_number_to_binary(char *directive_number, char *word_in_binary);


/**
 * Extracts a string literal from a .string directive.
 * Validates that the string starts and ends with double quotes and contains only ASCII characters.
 * And updates ptr to point to the position after the closing quote.
 *
 * Input:
 *   - ptr: Pointer to the pointer of the current position in the line being processed
 *
 * Output:
 *   - Returns a dynamically allocated string containing the extracted directive string on success
 *   - Returns NULL if validation fails (missing quotes or non-ASCII characters)
 */
char *get_directive_string(char **ptr);


/**
 * Inserts a directive string character by character into the data table.
 * This function processes each character of the input string (including the null terminator),
 * converts each character to binary representation, and adds it to the data table.
 *
 * Input:
 *   - directive_string: The string to be inserted into the data table
 *   - word_in_binary: Character array where the binary representation will be stored
 *   - data: Pointer to the data array where the binary string will be stored
 *   - DC: Pointer to the Data Counter, which will be updated as data words are added
 *
 * Output:
 *   - No return value
 */
void insert_directive_string_into_data_table(char *directive_string, char *word_in_binary, code_data_array **data, int *DC);


/**
 * Converts a direct addressing operand to its binary representation.
 * This function converts the address value into binary format with appropriate ARE bits
 * (A=0, R=1, E=1 for external addresses or A=0, R=1, E=0 for internal addresses).
 * This function is used in second pass (because direct addressing mode can be encoded only after label table was built)
 *
 * Input:
 *   - address: The address value to be converted (0 for external addresses)
 *   - word_in_binary: Character array where the binary representation will be stored
 *
 * Output:
 *   - No return value
 */
void direct_operand_word_to_binary(int address, char *word_in_binary);


/**
 * Converts a relative addressing operand to its binary representation.
 * This function converts the jumping distance value into binary format with appropriate ARE bits
 * (A=1, R=0, E=0 for relative addressing). It handles negative distances using two's complement.
 * This function is used in second pass (because relative addressing mode can be encoded only after label table was built)
 *
 * Input:
 *   - jumping_distance: The distance to jump (can be positive or negative)
 *   - word_in_binary: Character array where the binary representation will be stored
 *
 * Output:
 *   - No return value
 */
void relative_operand_word_to_binary(int jumping_distance, char *word_in_binary);


#endif /* CONVERTOR_H */