#include "second_pass.h"
#include "auxiliary_functions.h"
#include "convertor.h"
#include "first_pass.h"
#include "table.h"
#include "errors.h"
#include "parser.h"


/* This class contains the functions for the second pass stage of the assembler program.
 * The second pass stage is responsible for generating the final object code and creating the output files.
 * It also handles the entries and externals tables, as well as updating the machine code with label addresses.
 */


void second_pass_stage(code_data_array **code, code_data_array **data, label_table **label_table, general_table **entries, general_table **externs, int ICF, int DCF, int label_table_lines, int *entries_lines, int *externs_lines, char *file_name, FILE *file)
{
    /* Go back to the beginning of the file (in order to pass over it a second time) */
    rewind(file);

    /* Add 'entry' type to label table (for labels that have been declared as entry in our file) */
    add_entry_type_to_label_table(label_table, label_table_lines, file);

    /* Build the entries table */
    build_entries_table(entries, label_table, entries_lines, label_table_lines);

    /*
     * Update the machine code of label operands (using the label table)
     * The function also adds lines to the 'externs' table (if necessary).
     * I decided to do it inside this function in order to save time complexity (of going over the code table twice [and searching for each ;ines representing an operand its corresponding label in the label table]).
     */
    update_machine_code_of_label_operands(code, label_table, externs, ICF, label_table_lines, externs_lines);

    if (current_error_number == ERROR_0)
    {/* Create the output files only if error has not been found */
        create_output_files(code, data, label_table, entries, externs, ICF, DCF, label_table_lines, *entries_lines, *externs_lines, file_name);
    }
}

void add_entry_type_to_label_table(label_table **label_table, int label_table_lines, FILE *file)
{
    int i, label_updated = 0;       /* 'label_updated' indicates if label was updated in label table (for error checking) */
    char line[MAX_LINE_LENGTH];     /* Buffer to store each line read from the file */
    char *ptr;                      /* Pointer to the current character */
    char *directive_name, *entry_label;

    while (fgets(line, sizeof(line), file))
    {/* Read line by line, and find out it's type */
        /* Update current line number */
        current_line_number++;

        /* Pointer to the beginning of the line */
        ptr = line;

        while (isspace(*ptr))
        {/* Skip all whitespaces (for 'Empty line' check) */
            ptr++;
        }
        if (*ptr == '\n' || *ptr == '\0')
        {/* Empty line => skip it */
            continue;
        }

        if (*ptr == ';')
        {/* Comment line => skip it */
            continue;
        }

        directive_name = get_directive_name(&ptr);
        if (directive_name != NULL && strcmp(directive_name, ".entry") == 0)
        {/* Entry directive found */
            /* Parse and get the label name */
            entry_label = get_label_name_and_advance(&ptr);

            /* Print error if extra characters are found at the end of the line */
            check_no_extra_chars(&ptr);

            if (entry_label == NULL)
                /* Invalid label name */
                continue;

            for (i = 0; i < label_table_lines; i++)
            {/* Go over each label */
                if (strcmp(entry_label, (*label_table + i)->label) == 0)
                {/* Matching label has been found => Update its type */
                    if ((*label_table + i)->secondary_type != NULL && strcmp((*label_table + i)->secondary_type, EXTERN_TYPE) == 0)
                    {/* Label type is already external (error checking is valid because external type is defined in the first pass stage) */
                        print_error(ERROR_10, AM_FILE_STAGE);
                    }
                    /* Update entry type in label table */
                    (*label_table + i)->secondary_type = ENTRY_TYPE;
                    label_updated = 1;
                    break;
                }
            }
            if (label_updated == 0)
                /* Label not found in label table */
                print_error(ERROR_36, AM_FILE_STAGE);
        }
    }
    /* Reset current line number */
    current_line_number = 0;
}

void build_entries_table(general_table **entries, label_table **label_table, int *entries_lines, int label_table_lines)
{
    int i;
    for (i = 0; i < label_table_lines; i++)
    {/* Go over each label */
        if ((*label_table + i)->secondary_type != NULL && strcmp((*label_table + i)->secondary_type, ENTRY_TYPE) == 0)
        {/* Label type is entry */
            add_to_entries_externs_table(entries, (*label_table + i)->address, (*label_table + i)->label, entries_lines);
        }
    }
}

void update_machine_code_of_label_operands(code_data_array **code, label_table **label_table, general_table **externs, int ICF, int label_table_lines, int *externs_lines)
{
    int i, j;                        /* Indexes for loops */
    char word_in_binary[WORD_SIZE];  /* Array to store the binary representation of a word (used if necessary) */
    char *current_label;             /* Pointer to the current label in the code table */
    int jumping_distance;            /* Distance between the label address to the current code line address (used if necessary) */
    int operand_was_encoded = 0;     /* Indicates if the operand was encoded (used for error checking) */

    for (i = 0; i < ICF-INITIAL_IC_VALUE; i++)
    {/* Go over each code line */
        if ((*code + i)->machine_code == 0)
        {/* Empty word has been reached */
            /* Reset the flag */
            operand_was_encoded = 0;
            current_label = (*code + i)->label;
            if (is_direct_addressing_mode(current_label))
            {/* Direct addressing mode */
                for (j = 0; j < label_table_lines; j++)
                {/* Find it's matching label */
                    if (strcmp(current_label, (*label_table + j)->label) == 0)
                    {/* Matching label has been found => Encode its address (according to direct addressing mode) */
                        if ((*label_table + j)->secondary_type != NULL && strcmp((*label_table + j)->secondary_type, EXTERN_TYPE) == 0)
                        {/* Label type is external */
                            add_to_entries_externs_table(externs, i+INITIAL_IC_VALUE, (*label_table + j)->label, externs_lines);
                        }
                        /* Create the word representing the direct addressing mode operand */
                        direct_operand_word_to_binary((*label_table + j)->address, word_in_binary);
                        /* Replace it in code table */
                        (*code + i)->machine_code = strtol(word_in_binary, NULL, BINARY_BASE);
                        /* Update flag */
                        operand_was_encoded = 1;
                    }
                }
            }
            else
            {/* An empty word can appear only if operand is a label => If addressing mode is not direct, it is relative */
                current_label += 1;  /* Skip the '&' character */
                for (j = 0; j < label_table_lines; j++)
                {/* Find it's matching label */
                    if (strcmp(current_label, (*label_table + j)->label) == 0)
                    {/* Matching label has been found => Encode its jumping distance (according to relative addressing mode) */
                        if ((*label_table + j)->address == 0)
                        {/* Label address is external */
                            print_error(ERROR_8, AM_FILE_STAGE);
                        }
                        /* Calculate the jumping distance (-1 in order to take the address of the 'first word' of the current instruction line) */
                        jumping_distance = (*label_table + j)->address - (*code + i - 1)->address;
                        /* Create the word representing the relative addressing mode operand */
                        relative_operand_word_to_binary(jumping_distance, word_in_binary);
                        /* Replace it in code table */
                        (*code + i)->machine_code = strtol(word_in_binary, NULL, BINARY_BASE);
                        /* Update flag */
                        operand_was_encoded = 1;
                    }
                }
            }
            if (operand_was_encoded == 0)
            {/* Label was never declared */
                current_line_number = (*code + i)->line_number;  /* Update current line number for error checking */
                print_error(ERROR_37, AM_FILE_STAGE);
            }
        }
    }
}

void create_output_files(code_data_array **code, code_data_array **data, label_table **label_table, general_table **entries, general_table **externs, int ICF, int DCF, int label_table_lines, int entries_lines, int externs_lines, char *file_name)
{
    /* File names */
    char *object_file_name, *entries_file_name, *externals_file_name;
    /* File pointers */
    FILE *object_file, *entries_file, *externals_file;

    /* Create the names of the output files */
    object_file_name = get_file_name(2, file_name, ".ob");
    entries_file_name = get_file_name(2, file_name, ".ent");
    externals_file_name = get_file_name(2, file_name, ".ext");

    /* Create the object file */
    object_file = safe_fopen(object_file_name, "w");
    create_object_file(object_file, code, data, ICF, DCF);
    fclose(object_file);

    /* Create the entries file (if entries table is not empty) */
    if (*entries != NULL)
    {
        entries_file = safe_fopen(entries_file_name, "w");
        create_entries_file(entries_file, entries, entries_lines);
        fclose(entries_file);
    }

    /* Create the externals file (if externs table is not empty) */
    if (*externs != NULL)
    {
        externals_file = safe_fopen(externals_file_name, "w");
        create_externals_file(externals_file, externs, externs_lines);
        fclose(externals_file);
    }

    /* Free the memory of the file names */
    free(object_file_name);
    free(entries_file_name);
    free(externals_file_name);
}

void create_object_file(FILE *file, code_data_array **code, code_data_array **data, int ICF, int DCF)
{
    int i;
    char *hex_str = NULL;  /* Buffer for the hexadecimal representation of the machine code
                              Initialize to NULL in order to 'free' an initialized variable */

    if (ICF-INITIAL_IC_VALUE + DCF > MAX_NUM_OF_WORDS)
    {/* Check if the number of words in the object file exceeds the maximum number of words */
        print_error(ERROR_3, AM_FILE_STAGE);
    }

    /* Write ICF and DCF in the first line of the file */
    fprintf(file, "     %d %d\n", ICF-INITIAL_IC_VALUE, DCF); /* Subtract INITIAL_IC_VALUE from ICF in order to get the number of code lines */

    /*
     * Convert machine code to hexadecimal base and write it in the file
     * Address is written in 7 digits with leading zeros
     * And machine code is written in 6 digits with leading zeros
     */
    for (i = 0; i < ICF-INITIAL_IC_VALUE; i++)
    {
        hex_str = convert_to_hexadecimal_base((*code + i)->machine_code);
        fprintf(file, "%.7d %.6s\n", (*code + i)->address, hex_str);
    }
    for (i = 0; i < DCF; i++)
    {
        hex_str = convert_to_hexadecimal_base((*data + i)->machine_code);
        fprintf(file, "%.7d %.6s\n", (*data + i)->address + ICF, hex_str); /* Add ICF to the address so that the data address will be after the code address */
    }

    /* Free the memory of the hexadecimal string */
    free(hex_str);
}

void create_entries_file(FILE *file, general_table **entries, int entries_lines)
{
    int i;
    for (i = 0; i < entries_lines; i++)
    {/* Go over entries table and add lines to entries file */
        /* Address is written in 7 digits with leading zeros */
        fprintf(file, "%s %.7d\n", (*entries + i)->label, (*entries + i)->address);
    }
}

void create_externals_file(FILE *file, general_table **externs, int externs_lines)
{
    int i;
    for (i = 0; i < externs_lines; i++)
    {/* Go over externs table and add lines to externals file */
        /* Address is written in 7 digits with leading zeros */
        fprintf(file, "%s %.7d\n", (*externs + i)->label, (*externs + i)->address);
    }
}

char *convert_to_hexadecimal_base(int machine_code)
{
    /* Buffer for 6 hex digits + a null terminator */
    char *hex_str = safe_malloc(HEX_STR_SIZE * sizeof(char));
    /*
     * Convert the machine code to hexadecimal base:
     * Format the machine_code integer as a hexadecimal number
     * And pad it with zeros to a width of 6 characters
     * Mask in order to use only 24 bits of machine_code (in order to fit within 6 hexadecimal digits)
     * And write the result to the hex_str buffer
     */
    sprintf(hex_str, "%06x", machine_code & 0xFFFFFF);
    return hex_str;
}