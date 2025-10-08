#ifndef PARSER_H
#define PARSER_H


/**
 * Finds the instruction name from the current position in the line.
 * Advances the pointer to the position after the instruction name.
 *
 * Input:
 *   - ptr: Pointer to the pointer of the current position in the line being processed
 *
 * Output:
 *   - Returns a dynamically allocated string containing the instruction name
 *   - NULL if no valid instruction name is found
 */
char *get_instruction_name(char **ptr);


/**
 * Extracts an instruction operand from the current position in the line.
 * Advances the pointer to the position after the operand.
 *
 * Input:
 *   - ptr: Pointer to the pointer of the current position in the line being processed
 *   - num_of_commas_expected: Number of commas expected before or after the operand
 *
 * Output:
 *   - Returns a dynamically allocated string containing the operand
 *   - NULL if no valid operand is found
 */
char *get_instruction_operand(char **ptr, int num_of_commas_expected);


/**
 * Gets the number of operands an instruction requires.
 *
 * Input:
 *   - name: The name of the instruction to check
 *
 * Output:
 *   - Returns the number of operands required by the instruction
 *   - Returns GARBAGE_VALUE if the instruction is invalid
 */
int get_instruction_num_of_args(char *name);


/**
 * Gets the label name from a string without advancing the pointer.
 *
 * Input:
 *   - ptr: Pointer to a string containing a potential label
 *
 * Output:
 *   - Returns a dynamically allocated string containing the label name
 *   - Returns NULL if no valid label is found
 */
char *get_label_name(char *ptr);


/**
 * Gets the label name from the current position in the line and advances the pointer.
 * Advances the pointer to the position after the label.
 * This function includes error checking unlike 'get_label_name'.
 *
 * Input:
 *   - ptr: Pointer to the pointer of the current position in the line being processed
 *
 * Output:
 *   - Returns a dynamically allocated string containing the label name
 *   - Returns NULL if no valid label is found
 */
char *get_label_name_and_advance(char **ptr);


/**
 * Gets the opcode value for a given instruction name.
 *
 * Input:
 *   - name: The name of the instruction to look up
 *
 * Output:
 *   - Returns the opcode value associated with the instruction
 *   - Returns GARBAGE_VALUE if the instruction name is invalid
 */
int get_instruction_opcode(char *name);


/**
 * Gets the function value for a given instruction name.
 *
 * Input:
 *   - name: The name of the instruction to look up
 *
 * Output:
 *   - Returns the function value associated with the instruction
 *   - Returns GARBAGE_VALUE if the instruction name is invalid
 */
int get_instruction_funct(char *name);


/**
 * Gets the register number from a register operand.
 *
 * Input:
 *   - operand: String representing a potential register operand
 *   - is_source_operand: Indicates if operand is a source or destination operand
 *
 * Output:
 *   - Returns the register number if operand is a valid register
 *   - Returns GARBAGE_VALUE if operand is not a valid register
 */
int get_register_number(char *operand, int is_source_operand);


/**
 * Determines the addressing mode of an operand for a given instruction.
 *
 * Input:
 *   - instr_name: The name of the instruction
 *   - operand: String representing the operand to analyze
 *
 * Output:
 *   - Returns the addressing mode code (0-3) for the operand
 *   - Returns 0 if operand is NULL (no addressing mode)
 *   - Returns GARBAGE_VALUE if the addressing mode is invalid
 */
int get_addressing_mode(char *instr_name, char *operand);


/**
 * Checks if the given operand uses immediate addressing mode.
 *
 * Input:
 *   - operand: String representing the operand to check
 *
 * Output:
 *   - Returns 1 (true) if the operand is in immediate addressing mode (starts with '#' followed by a valid number)
 *   - Returns 0 (false) otherwise
 */
int is_immediate_addressing_mode(char *operand);


/**
 * Checks if the given operand uses direct addressing mode.
 *
 * Input:
 *   - operand: String representing the operand to check
 *
 * Output:
 *   - Returns 1 (true) if the operand is in direct addressing mode (valid label syntax)
 *   - Returns 0 (false) otherwise
 */
int is_direct_addressing_mode(char *operand);


/**
 * Checks if the given operand uses relative addressing mode.
 *
 * Input:
 *   - instr_name: The name of the instruction being analyzed
 *   - operand: String representing the operand to check
 *
 * Output:
 *   - Returns 1 (true) if the operand is in relative addressing mode
 *     (operand begins with '&' followed by valid label and instruction is jmp, bne, or jsr)
 *   - Returns 0 (false) otherwise
 */
int is_relative_addressing_mode(char *instr_name, char *operand);


/**
 * Checks if the given operand is a register.
 *
 * Input:
 *   - operand: String representing the operand to check
 *
 * Output:
 *   - Returns 1 (true) if the operand is a valid register name
 *   - Returns 0 (false) if the operand is not a register or is NULL
 */
int is_register(char *operand);


/**
 * Finds the directive name from the current position in the line.
 * Advances the pointer to the position after the directive name.
 *
 * Input:
 *   - ptr: Pointer to the pointer of the current position in the line being processed
 *
 * Output:
 *   - Returns a pointer to the directive name from the DIRECTIVES array
 *   - Returns NULL if no valid directive name is found
 */
char *get_directive_name(char **ptr);


/**
 * Checks if a given name is a reserved word in the assembly language.
 * Reserved words include: instruction names, directive names, and register names.
 *
 * Input:
 *   - name: String containing the name to check
 *
 * Output:
 *   - Returns 1 (true) if the name is reserved
 *   - Returns 0 (false) if the name is not reserved
 */
int is_reserved_name(char *name);


/**
 * Validates a label name and checks it against language rules.
 * Advances the pointer to the character after the label (after the colon).
 * Prints relevant errors if the label is invalid.
 * This function is responsible for detecting an invalid declaration of a label.
 *
 * Input:
 *   - ptr: Pointer to the pointer of the current position in the line being processed
 *
 * Output:
 *   - No return value
 */
void check_validity_of_label_name(char **ptr);


/**
 * Checks if an operand type is valid for a given instruction (in terms of addressing mode).
 *
 * Input:
 *   - instr_name: The name of the instruction
 *   - operand_type: The addressing mode type to check
 *   - is_source_operand: 1 if checking source operand, 0 if destination operand
 *
 * Output:
 *   - Returns 1 (true) if the operand type is invalid for the instruction
 *   - Returns 0 (false) if the operand type is valid or instruction name is invalid
 */
int invalid_instruction_operand_type(char *instr_name, int operand_type, int is_source_operand);


#endif /* PARSER_H */