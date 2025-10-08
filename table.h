#ifndef TABLE_H
#define TABLE_H


#include "first_pass.h"

/**
 * Adds a new line to the code table with the provided machine code and label.
 *
 * Input:
 *   - code: Double pointer to the code table array
 *   - IC: Pointer to the instruction counter
 *   - word_in_binary: String containing the binary representation of the machine code
 *   - label: String containing the label associated with this code (can be NULL)
 *
 * Output:
 *   - No return value
 */
void add_to_code_table(code_data_array **code, int *IC, char *word_in_binary, char *label);


/**
 * Adds a new line to the data table with the provided binary representation.
 *
 * Input:
 *   - data: Double pointer to the data table array
 *   - DC: Pointer to the data counter
 *   - word_in_binary: String containing the binary representation of the data
 *
 * Output:
 *   - No return value
 */
void add_to_data_table(code_data_array **data, int *DC, char *word_in_binary);


/**
 * Adds a new line to the label table with the provided information.
 *
 * Input:
 *   - table: Double pointer to the label table
 *   - address: Memory address associated with the label
 *   - label_name: String containing the name of the label
 *   - type: String indicating the label type (code/data)
 *   - secondary_type: String containing additional type information (entry/extern)
 *   - lines: Pointer to the counter of lines in the label table
 *
 * Output:
 *   - No return value
 */
void add_to_label_table(label_table **table, int address, char *label_name, char *type, char *secondary_type, int *lines);


/**
 * Updates the address values of all data-type labels by adding the final instruction count (ICF).
 *
 * Input:
 *   - table: Double pointer to the label table
 *   - ICF: Final instruction counter value from first pass
 *   - lines: Number of lines in the label table
 *
 * Output:
 *   - No return value
 */
void update_label_table_cells_of_type_data(label_table **table, int ICF, int lines);


/**
 * Prints all lines in the label table with their details.
 *
 * Input:
 *   - table: Pointer to the label table
 *   - lines: Number of lines in the label table
 *
 * Output:
 *   - No return value
 */
void print_label_table_cells(label_table *table, int lines);


/**
 * Prints all lines in the code/data table with their machine code.
 *
 * Input:
 *   - table: Pointer to the code/data table
 *   - counter: Number of lines in the table
 *
 * Output:
 *   - No return value
 */
void print_code_data_table_cells(code_data_array *table, int counter);


/**
 * Adds a new line to the entries/externals table with the provided information.
 *
 * Input:
 *   - table: Double pointer to the entries/xternals table
 *   - address: Memory address associated with the label
 *   - label_name: String containing the name of the label
 *   - lines: Pointer to the counter of lines in the table
 *
 * Output:
 *   - No return value
 */
void add_to_entries_externs_table(general_table **table, int address, char *label_name, int *lines);


/**
 * Prints all lines in the entries table with their labels and addresses.
 *
 * Input:
 *   - table: Pointer to the entries table
 *   - lines: Number of lines in the entries table
 *
 * Output:
 *   - No return value
 */
void print_entries_table_cells(general_table *table, int lines);


/**
 * Prints all lines in the externs table with their labels and addresses.
 *
 * Input:
 *   - table: Pointer to the externs table
 *   - lines: Number of lines in the externs table
 *
 * Output:
 *   - No return value
 */
void print_externs_table_cells(general_table *table, int lines);


#endif /* TABLE_H */