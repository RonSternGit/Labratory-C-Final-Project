#include "general_header.h"
#include "first_pass.h"
#include "table.h"
#include "auxiliary_functions.h"
#include "convertor.h"
#include "errors.h"


/* This class contains functions that are responsible for managing the tables used in the assembler program.
 * The tables include:
 * 1. Code table
 * 2. Data table
 * 3. Label table
 * 4. Entries table
 * 5. Externs table
 */


void add_to_code_table(code_data_array **code, int *IC, char *word_in_binary, char *label)
{
    /* Allocate memory */
    *code = safe_realloc(*code, (*IC-INITIAL_IC_VALUE + 1) * sizeof(code_data_array));  /* +1 for the new line in the code table */
    /* Store values in table */
    (*code + (*IC-INITIAL_IC_VALUE))->machine_code = strtol(word_in_binary, NULL, BINARY_BASE);
    (*code + (*IC-INITIAL_IC_VALUE))->address = *IC;
    (*code + (*IC-INITIAL_IC_VALUE))->label = label;
    (*code + (*IC-INITIAL_IC_VALUE))->line_number = current_line_number;
    /* Increase counter */
    (*IC)++;
}

void add_to_data_table(code_data_array **data, int *DC, char *word_in_binary)
{
    /* Allocate memory */
    *data = safe_realloc(*data, (*DC + 1) * sizeof(code_data_array));  /* +1 for the new line in the code table */
    /* Store values in table */
    (*data + *DC)->machine_code = strtol(word_in_binary, NULL, BINARY_BASE);
    (*data + *DC)->address = *DC;
    (*data + *DC)->label = NULL;
    /* Increase counter */
    (*DC)++;
}

void add_to_label_table(label_table **table, int address, char *label_name, char *type, char *secondary_type, int *lines)
{
    /* Check if label is already defined in the table */
    int i;
    for (i = 0; i < *lines; i++)
    {
        if (strcmp((*table + i)->label, label_name) == 0)
        {/* Label already exists in the table */
            print_error(ERROR_11, AM_FILE_STAGE);
            return;
        }
    }

    /* 'lines' - Points to the next empty line */
    *table = safe_realloc(*table, (*lines + 1) * sizeof(label_table));  /* +1 for the new line */
    /* Store values in table */
    (*table + *lines)->address = address;
    (*table + *lines)->label = label_name;
    (*table + *lines)->type = type;
    (*table + *lines)->secondary_type = secondary_type;
    /* Increase the number of lines in the table (for the next insertion) */
    (*lines)++;
}

/* A single function is sufficient as both operations are identical */
void add_to_entries_externs_table(general_table **table, int address, char *label_name, int *lines)
{
    /* 'lines' - Points to the next empty line */
    *table = safe_realloc(*table, (*lines + 1) * sizeof(general_table));  /* +1 for the new line */
    /* Store values in table */
    (*table + *lines)->address = address;
    (*table + *lines)->label = label_name;
    /* Increase counter */
    (*lines)++;
}

void update_label_table_cells_of_type_data(label_table **table, int ICF, int lines)
{
    int i;
    for (i = 0; i < lines; i++)
    {/* Go over label table and add ICF to the address of data type lines */
        /* [(*table + i)->type != NULL] - Is checked in order to deal with an external (because it's 'type' is NULL) */
        if ((*table + i)->type != NULL && strcmp((*table + i)->type, DATA_TYPE) == 0)
        {
            (*table + i)->address += ICF;
        }
    }
}

void print_label_table_cells(label_table *table, int lines)
{
    int i;
    for (i = 0; i < lines; i++)
    {/* Go over label table and print its lines */
        printf("Label:       %3d | %5s", (table + i)->address, (table + i)->label);
        printf(" | %7s  | %s\n", (table + i)->type, (table + i)->secondary_type);
    }
}

void print_code_data_table_cells(code_data_array *table, int counter)
{
    int i, bin_num;
    char word_in_binary[WORD_SIZE];

    for (i = 0; i < counter; i++)
    {/* Go over code/data table and print its lines (manually convert to its machine code) */
        bin_num = 0;
        integer_to_binary((table + i)->machine_code, word_in_binary, &bin_num);
        add_zeros(word_in_binary, &bin_num, WORD_SIZE - bin_num);
        reverse_string(word_in_binary);
        printf("Address: %3d | Machine Code: %s\n", (table + i)->address, word_in_binary);
    }
}

void print_entries_table_cells(general_table *table, int lines)
{
    int i;
    for (i = 0; i < lines; i++)
    {/* Go over entries table and print its lines */
        printf("Entries: %5s | %3d\n", (table + i)->label, (table + i)->address);
    }
}

void print_externs_table_cells(general_table *table, int lines)
{
    int i;
    for (i = 0; i < lines; i++)
    {/* Go over externs table and print its lines */
        printf("Externs: %5s | %3d\n", (table + i)->label, (table + i)->address);
    }
}