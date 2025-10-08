#include "first_pass.h"
#include "general_header.h"
#include "auxiliary_functions.h"
#include "convertor.h"
#include "table.h"
#include "second_pass.h"
#include "parser.h"
#include "errors.h"


/* This class contains the functions that are responsible for the first pass stage of the assembler
 * The first pass stage is responsible for parsing the assembly code and converting it to machine code
 * It also creates the label table and the externs table
 * The second pass stage is responsible for resolving the labels and externs
 */


int first_pass_stage(char *file_name)
{
    /* 'instruction_line' is used to store the necessary data of the *current* instruction line */
    encoded_instruction *instruction_line = safe_malloc(sizeof(encoded_instruction));

    /* 'code' is the code array, and 'data' is the data array
     * they are used to store the machine code and data respectively
     */
    code_data_array *code = NULL, *data = NULL;      /* Initialize to NULL in order to be able to 'free' at the end (uninitialized variables can't be freed) */

    /* 'label_table' is used to store the label table
     * it is used to store the labels and their addresses
     */
    label_table *label_table = NULL;                 /* Initialize to NULL in order to prevent future errors */

    /* 'externs' is used to store the externs table
     * 'entries' is used to store the entries table
     */
    general_table *externs = NULL, *entries = NULL;  /* Initialize to NULL in order to be able to 'free' at the end (uninitialized variables can't be freed) */

    /* 'IC' is the instruction counter, and 'DC' is the data counter
     * 'ICF' is the final instruction counter, and 'DCF' is the final data counter
     */
    int IC = INITIAL_IC_VALUE, DC = INITIAL_DC_VALUE, ICF, DCF;

    /* 'label_table_lines' is the *current* number of lines in the label table
     * 'externs_lines' is the *current* number of lines in the externs table
     * 'entries_lines' is the *current* number of lines in the entries table
     */
    int label_table_lines = 0, externs_lines = 0, entries_lines = 0;

    /* 'complete_file_name' is used to store the name of the file with its ending
     * 'am_file' is used to open the file for reading
     */
    char *complete_file_name;
    FILE *am_file;


    /* 'complete' because it also contains the file's ending.
     * '.am' because we are taking the file after the macro expansion.
     */
    complete_file_name = get_file_name(2, file_name, ".am");  /* Get the file name with the extension */
    am_file = safe_fopen(complete_file_name, "r");           /* Open the file for reading */

    /* This function is responsible for most part in the first pass stage */
    encode_all_assembly_lines(am_file, &instruction_line, &code, &data, &label_table, &IC, &DC, &label_table_lines);

    /* Save final IC and DC values */
    ICF = IC;
    DCF = DC;

    /* Update label table cells of type data, by adding ICF to their address */
    update_label_table_cells_of_type_data(&label_table, ICF, label_table_lines);

    /* Start second pass */
    second_pass_stage(&code, &data, &label_table, &entries, &externs, ICF, DCF, label_table_lines, &entries_lines, &externs_lines, file_name, am_file);

    /* Here are some printing functions if someone desires */
    /*print_label_table_cells(label_table, label_table_lines);
      print_code_data_table_cells(code, IC-INITIAL_IC_VALUE);
      print_code_data_table_cells(data, DC);
      print_entries_table_cells(entries, entries_lines);
      print_externs_table_cells(externs, externs_lines);*/

    /* Free allocated space, and close open files */
    free(instruction_line);
    free(label_table);
    free(code);
    free(data);
    free(externs);
    free(entries);
    fclose(am_file);
    free(complete_file_name);

    /* Return the error number */
    return current_error_number;
}

void encode_all_assembly_lines(FILE *file, encoded_instruction **instruction_line, code_data_array **code, code_data_array **data, label_table **label_table, int *IC, int *DC, int *label_table_lines)
{
    char line[MAX_LINE_LENGTH];         /* Buffer to store the current line */
    char *ptr;                          /* Pointer to the current character in the line */
    int invalid_chars_error_found = 0;  /* Indicates if invalid character error was printed (in order to print it once) */

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

        if (strchr(ptr, ':') != NULL)
        {/* Label declaration in current line (contains ':') => check if label is valid */
            /* Check if label is valid and print error messages if errors found */
            check_validity_of_label_name(&ptr);
        }

        while (*ptr != '.' && !islower(*ptr) && *ptr != '\0')
        {/* Skip all characters while neither Directive nor Instruction nor end of line has been reached */
            if (!isspace(*ptr) && !invalid_chars_error_found)
            {/* Invalid character before directive/instruction sentence */
                print_error(ERROR_20, AM_FILE_STAGE);
                invalid_chars_error_found = 1;  /* Print error only once */
            }
            /* Skip character */
            ptr++;
        }

        if (*ptr == '.')
        {/* Directive => encode it (if valid) */
            encode_directive(&ptr, line, data, label_table, DC, label_table_lines);
        }

        else
        {/* Instruction => encode it (if valid) */
            encode_instruction(&ptr, line, instruction_line, code, label_table, IC, label_table_lines);
        }
    }
    /* Reset current line number */
    current_line_number = 0;
}

void encode_directive(char **ptr, char *line, code_data_array **data, label_table **label_table, int *DC, int *label_table_lines)
{
    char *directive_name = NULL, *label_name;  /* Label name (if exists) */

    directive_name = get_directive_name(ptr);  /* No need to free directive_name_name because it points to a value in DIRECTIVES [that is available through the whole program] */

    if (directive_name == NULL)
    {
        /* Invalid directive name */
        print_error(ERROR_14, AM_FILE_STAGE);
        return; /* Return here to prevent dereferencing NULL pointer */
    }

    label_name = get_label_name(line);
    if (label_name != NULL)
    {/* Label found => add it to label table */
        /* Add label only if it is a data or string directive (labels before entry and extern directives are ignored) */
        if (strcmp(directive_name, ".data") == 0 || strcmp(directive_name, ".string") == 0)
            add_to_label_table(label_table, *DC, label_name, DATA_TYPE, NULL, label_table_lines);
        else
            print_error(ERROR_9, AM_FILE_STAGE);
    }

    /* Convert the directive to its binary machine code and save it in the data array */
    directive_to_binary(ptr ,directive_name, data, label_table, DC, label_table_lines);
}

void encode_instruction(char **ptr, char *line, encoded_instruction **instruction_line, code_data_array **code, label_table **label_table, int *IC, int *label_table_lines)
{
    int instr_num_of_args;
    char *instr_name, *instr_source_operand = NULL, *instr_destination_operand = NULL;

    instr_name = get_instruction_name(ptr); /* No need to free instr_name because it points to a value in INSTRUCTIONS [that is available through the whole program] */

    /* Print error for invalid instruction name */
    if (instr_name == NULL)
    {
        if (**ptr == '\0')
            /* Missing instruction name */
            print_error(ERROR_34, AM_FILE_STAGE);
        else
            /* Invalid instruction name */
            print_error(ERROR_15, AM_FILE_STAGE);
        return; /* Return here to prevent dereferencing NULL pointer */
    }

    instr_num_of_args = get_instruction_num_of_args(instr_name); /* instr_num_of_args is defined as written in INSTRUCTIONS (and not manually parsing and counting the number of operands) */

    /* Get the instruction operands (according to the number of arguments) */
    if (instr_num_of_args == 0)
    {
        instr_source_operand = NULL;
        instr_destination_operand = NULL;
    }
    else if (instr_num_of_args == 1)
    {
        instr_source_operand = NULL;
        instr_destination_operand = get_instruction_operand(ptr, 0);
    }
    else if (instr_num_of_args == 2)
    {
        instr_source_operand = get_instruction_operand(ptr, 0);
        instr_destination_operand = get_instruction_operand(ptr, 1);
    }
    else
        /* Invalid number of instruction arguments */
        print_error(ERROR_16, AM_FILE_STAGE);

    /* Print error if extra characters are found at the end of the line */
    check_no_extra_chars(ptr);

    /* Save the instruction line's data in the instruction_line structure for later use (conversion to binary) */
    create_encoded_instruction(instr_name, instr_source_operand, instr_destination_operand, instr_num_of_args, line, instruction_line);

    /* Print errors for invalid instruction operands */
    if (instr_source_operand != NULL)
    {
        if (*instr_source_operand == '\0')
        {/* Missing source operand */
            print_error(ERROR_30, AM_FILE_STAGE);
        }
        else if (invalid_instruction_operand_type(instr_name, (*instruction_line)->source_addressing_mode, 1))
        {/* Invalid source operand type */
            print_error(ERROR_28, AM_FILE_STAGE);
        }
    }
    if (instr_destination_operand != NULL)
    {
        if (*instr_destination_operand == '\0')
        {/* Missing destination operand */
            print_error(ERROR_31, AM_FILE_STAGE);
        }
        else if (invalid_instruction_operand_type(instr_name, (*instruction_line)->destination_addressing_mode, 0))
        {/* Invalid destination operand type */
            print_error(ERROR_29, AM_FILE_STAGE);
        }
    }

    if ((*instruction_line)->label != NULL)
    {/* Label found => add it to label table */
        add_to_label_table(label_table, *IC, (*instruction_line)->label, CODE_TYPE, NULL, label_table_lines);
    }

    /* Reset the source and destination addressing modes to 0 if they are GARBAGE_VALUE (in order to avoid a future error) */
    if ((*instruction_line)->source_addressing_mode == GARBAGE_VALUE)
        (*instruction_line)->source_addressing_mode = 0;
    if ((*instruction_line)->destination_addressing_mode == GARBAGE_VALUE)
        (*instruction_line)->destination_addressing_mode = 0;

    /* Convert the instruction to its binary machine code and save it in the code array */
    instruction_to_binary(instruction_line, code, IC);
}

void create_encoded_instruction(char *instr_name, char *instr_source_operand, char *instr_destination_operand, int instr_num_of_args, char *line, encoded_instruction **instruction_line)
{
    /* Insert data of current instruction line into encoded_instruction structure */
    (*instruction_line)->opcode = get_instruction_opcode(instr_name);
    (*instruction_line)->source_addressing_mode = get_addressing_mode(instr_name, instr_source_operand); /* Result according to parsing */
    (*instruction_line)->source_register = get_register_number(instr_source_operand, 1);
    (*instruction_line)->destination_addressing_mode = get_addressing_mode(instr_name, instr_destination_operand);
    (*instruction_line)->destination_register = get_register_number(instr_destination_operand, 0);
    (*instruction_line)->funct = get_instruction_funct(instr_name);
    (*instruction_line)->A = 1;
    (*instruction_line)->R = 0;
    (*instruction_line)->E = 0;
    (*instruction_line)->label = get_label_name(line);
    (*instruction_line)->num_of_args = instr_num_of_args;

    /* Save operands for later use */
    (*instruction_line)->source_operand = instr_source_operand;
    (*instruction_line)->destination_operand = instr_destination_operand;
}