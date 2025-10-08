#ifndef FIRST_PASS_H
#define FIRST_PASS_H


#include "general_header.h"

/** Structure to hold machine code for code/data tables */
typedef struct code_data_array {
    int address;
    int machine_code;  /* Is equal to the decimal representation of the binary representation of a 'word' */
    char *label;       /* Is stored if current line represents an operand that is a label */
    int line_number;   /* Is used for error checking in 'update_machine_code_of_label_operands' (therefore it is saved only for the code table) */
} code_data_array;

/** Structure to hold all required data for a specific instruction line in order to convert and add it to the code table */
typedef struct encoded_instruction {/* Values are in decimal base */
    int opcode;
    int source_addressing_mode;
    int source_register;
    int destination_addressing_mode;
    int destination_register;
    int funct;
    int A;
    int R;
    int E;
    char *label;              /* Saved if exists */
    int num_of_args;
    char *source_operand;
    char *destination_operand;
} encoded_instruction;

/** Structure to hold all allowed values for an instruction */
typedef struct instruction_info {
    char *name;
    int opcode;
    int funct;
    int src_valid_addressing_modes[MAX_NUM_OF_VALID_ADDRESSING_MODES_FOR_PARAMETER];
    int dest_valid_addressing_modes[MAX_NUM_OF_VALID_ADDRESSING_MODES_FOR_PARAMETER];
    int num_of_args;
} instruction_info;

/** Structure to hold the label table */
typedef struct label_table {
    int address;
    char *label;
    char *type;            /* For code and data */
    char *secondary_type;  /* For entries and externs */
} label_table;

/** Structure to hold entries/externals table */
typedef struct general_table {
    int address;
    char *label;
} general_table;


/**
 * Executes the first pass of the assembly process.
 * This function processes the input assembly file, building the label table,
 * encoding instructions and directives, and preparing for the second pass.
 *
 * Input:
 *   - file_name: Name of the assembly source file to process
 *
 * Output:
 *   - Returns 0 if the first pass completed successfully
 *   - Returns an error code if errors were encountered
 */
int first_pass_stage(char *file_name);


/**
 * Processes all lines in the assembly source file, encoding instructions and directives.
 * For each line, it determines if it contains a label, instruction, or directive, and
 * processes it accordingly by calling appropriate encoding functions.
 *
 * Input:
 *   - file: Pointer to the open assembly source file being processed
 *   - instruction_line: Pointer to the pointer of the instruction structure to store encoded instructions
 *   - code: Pointer to the code array where encoded instructions will be stored
 *   - data: Pointer to the data array where encoded directives will be stored
 *   - label_table: Pointer to the label table to store defined labels
 *   - IC: Pointer to the Instruction Counter, which will be updated as instructions are encoded
 *   - DC: Pointer to the Data Counter, which will be updated as directives are encoded
 *   - label_table_lines: Pointer to the count of lines in the label table
 *
 * Output:
 *   - No return value
 */
void encode_all_assembly_lines(FILE *file, encoded_instruction **instruction_line, code_data_array **code, code_data_array **data, label_table **label_table, int *IC, int *DC, int *label_table_lines);


/**
 * Encodes an assembly instruction line into its binary representation.
 * This function identifies the instruction type, validates operands, and creates
 * the encoded representation for further processing.
 *
 * Input:
 *   - ptr: Pointer to the pointer of the current position in the line being processed
 *   - line: The complete line text being processed
 *   - instruction_line: Pointer to the pointer of the instruction structure to store encoded instruction
 *   - code: Pointer to the code array where the encoded instruction will be stored
 *   - label_table: Pointer to the label table for referencing labels
 *   - IC: Pointer to the Instruction Counter, which will be updated as words are added
 *   - label_table_lines: Pointer to the count of lines in the label table
 *
 * Output:
 *   - No return value
 */
void encode_instruction(char **ptr, char *line, encoded_instruction **instruction_line, code_data_array **code, label_table **label_table, int *IC, int *label_table_lines);


/**
 * Creates and initializes an encoded instruction structure based on the provided instruction details.
 *
 * Input:
 *   - instr_name: Name of the instruction
 *   - instr_source_operand: String representing the source operand (NULL if not exists)
 *   - instr_destination_operand: String representing the destination operand (NULL if not exists)
 *   - instr_num_of_args: Number of arguments the instruction has (0, 1, 2)
 *   - line: The complete assembly line being processed
 *   - instruction_line: Pointer to the pointer of the instruction structure to be stored in
 *
 * Output:
 *   - No return value
 */
void create_encoded_instruction(char *instr_name, char *instr_source_operand, char *instr_destination_operand, int instr_num_of_args, char *line, encoded_instruction **instruction_line);


/**
 * Encodes an assembly directive line into its binary representation.
 * This function identifies the directive type, validates parameters, and processes
 * any associated label, adding it to the label table if needed.
 *
 * Input:
 *   - ptr: Pointer to the pointer of the current position in the line being processed
 *   - line: The complete line text being processed
 *   - data: Pointer to the data array where the encoded directive will be stored
 *   - label_table: Pointer to the label table for adding labels defined with directives
 *   - DC: Pointer to the Data Counter, which will be updated as data words are added
 *   - label_table_lines: Pointer to the count of lines in the label table
 *
 * Output:
 *   - No return value
 */
void encode_directive(char **ptr, char *line, code_data_array **data, label_table **label_table, int *DC, int *label_table_lines);


#endif /* FIRST_PASS_H */