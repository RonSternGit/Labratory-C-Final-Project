#ifndef SECOND_PASS_H
#define SECOND_PASS_H


#include "first_pass.h"


/**
 * Handles the second pass of the assembly process to complete machine code generation.
 *
 * Input:
 *   - code: Double pointer to the array containing machine code instructions
 *   - data: Double pointer to the array containing data segment
 *   - label_table: Double pointer to the table containing all labels
 *   - entries: Double pointer to the table that will store entry labels
 *   - externs: Double pointer to the table that will store external labels
 *   - ICF: Final instruction counter value from first pass
 *   - DCF: Final data counter value from first pass
 *   - label_table_lines: Number of lines in the label table
 *   - entries_lines: Pointer to store the count of entry labels
 *   - externs_lines: Pointer to store the count of external labels
 *   - file_name: Name of the source file being assembled
 *   - file: Pointer to the source file
 *
 * Output:
 *   - No return value
 */
void second_pass_stage(code_data_array **code, code_data_array **data, label_table **label_table, general_table **entries, general_table **externs, int ICF, int DCF, int label_table_lines, int *entries_lines, int *externs_lines, char *file_name, FILE *file);


/**
 * Marks labels declared as entry types in the label table.
 *
 * Input:
 *   - label_table: Double pointer to the table containing all labels
 *   - label_table_lines: Number of lines in the label table
 *   - file: Pointer to the source file being processed
 *
 * Output:
 *   - No return value
 */
void add_entry_type_to_label_table(label_table **label_table, int label_table_lines, FILE *file);


/**
 * Builds a table of all entry labels from the label table.
 *
 * Input:
 *   - entries: Double pointer to the table that will store entry labels
 *   - label_table: Double pointer to the table containing all labels
 *   - entries_lines: Pointer to store the count of entry labels
 *   - label_table_lines: Number of lines in the label table
 *
 * Output:
 *   - No return value
 */
void build_entries_table(general_table **entries, label_table **label_table, int *entries_lines, int label_table_lines);

/**
 * Updates machine code for instructions that have label operands with their correct addresses.
 *
 * Input:
 *   - code: Double pointer to the array containing machine code instructions
 *   - label_table: Double pointer to the table containing all labels
 *   - externs: Double pointer to the table that will store external labels
 *   - ICF: Final instruction counter value from first pass
 *   - label_table_lines: Number of lines in the label table
 *   - externs_lines: Pointer to store the count of external labels
 *
 * Output:
 *   - No return value
 */
void update_machine_code_of_label_operands(code_data_array **code, label_table **label_table, general_table **externs, int ICF, int label_table_lines, int *externs_lines);


/**
 * Creates all output files for the assembler (object, entries, externals).
 *
 * Input:
 *   - code: Double pointer to the array containing machine code instructions
 *   - data: Double pointer to the array containing data segment
 *   - label_table: Double pointer to the table containing all labels
 *   - entries: Double pointer to the table containing entry labels
 *   - externs: Double pointer to the table containing external labels
 *   - ICF: Final instruction counter value from first pass
 *   - DCF: Final data counter value from first pass
 *   - label_table_lines: Number of lines in the label table
 *   - entries_lines: Number of entry labels
 *   - externs_lines: Number of external labels
 *   - file_name: Name of the source file being assembled
 *
 * Output:
 *   - No return value
 */
void create_output_files(code_data_array **code, code_data_array **data, label_table **label_table, general_table **entries, general_table **externs, int ICF, int DCF, int label_table_lines, int entries_lines, int externs_lines, char *file_name);


/**
 * Creates the object output file containing machine code in hexadecimal format.
 *
 * Input:
 *   - file: Pointer to the file where the object code will be written
 *   - code: Double pointer to the array containing machine code instructions
 *   - data: Double pointer to the array containing data segment
 *   - ICF: Final instruction counter value from first pass
 *   - DCF: Final data counter value from first pass
 *
 * Output:
 *   - No return value
 */
void create_object_file(FILE *file, code_data_array **code, code_data_array **data, int ICF, int DCF);


/**
 * Creates the entries output file listing all entry labels and their addresses.
 * File is created only if entries exist.
 *
 * Input:
 *   - file: Pointer to the file where entry label information will be written
 *   - entries: Double pointer to the table containing all entry labels
 *   - entries_lines: Number of entry labels in the table
 *
 * Output:
 *   - No return value
 */
void create_entries_file(FILE *file, general_table **entries, int entries_lines);


/**
 * Creates the externals output file listing all external labels and their references.
 * File is created only if externals exist.
 *
 * Input:
 *   - file: Pointer to the file where external label information will be written
 *   - externs: Double pointer to the table containing all external labels and references
 *   - externs_lines: Number of external labels in the table
 *
 * Output:
 *   - No return value
 */
void create_externals_file(FILE *file, general_table **externs, int externs_lines);


/**
 * Converts an integer machine code value to its hexadecimal string representation.
 *
 * Input:
 *   - machine_code: Integer value of the machine code to convert
 *
 * Output:
 *   - Returns a pointer to a string containing the hexadecimal representation
 */
char *convert_to_hexadecimal_base(int machine_code);


#endif /* SECOND_PASS_H */