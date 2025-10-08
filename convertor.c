#include "convertor.h"
#include "general_header.h"
#include "auxiliary_functions.h"
#include "first_pass.h"
#include "table.h"
#include "errors.h"
#include "parser.h"


/* This class contains functions that convert the data from the first pass to binary format.
 * The functions are used to convert the instructions and directives to binary format.
 * The functions are also used to add the binary data to the code and data tables.
 */


void add_zeros(char *word_in_binary, int *binary_index, int required_zeros)
{
    int i;
    for (i = 0; i < required_zeros; i++)
    {
        word_in_binary[(*binary_index)++] = '0';
    }
    /* Null termination */
    word_in_binary[*binary_index] = '\0';
}

void integer_to_binary(int num, char *word_in_binary, int *binary_index)
{
    /* Save the binary representation of 'num' in the array: word_in_binary */
    word_in_binary[(*binary_index)++] = (num % BINARY_BASE) ? '1' : '0';
    if (num > 1)
    {/* If num is greater than 1, continue dividing by 2 */
        integer_to_binary(num / BINARY_BASE, word_in_binary, binary_index);
    }
}

void instruction_to_binary(encoded_instruction **instruction_line, code_data_array **code, int *IC)
{
    char word_in_binary[WORD_SIZE]; /* Array to store the binary representation of a word */

    /* Get the binary representation of the first word */
    first_word_to_binary(instruction_line, word_in_binary);

    /* Add the first word to the code array */
    add_to_code_table(code, IC, word_in_binary, NULL);

    /*
     * Continue and add the words representing the operands (if necessary)
     * (Memory is being allocated for words representing the operands)
     */
    if ((*instruction_line)->num_of_args == 1)
    {
        operand_word_to_binary(instruction_line, code, IC, word_in_binary, 1);
    }
    else if ((*instruction_line)->num_of_args == 2)
    {
        operand_word_to_binary(instruction_line, code, IC, word_in_binary, 0);
        operand_word_to_binary(instruction_line, code, IC, word_in_binary, 1);
    }
}

void first_word_to_binary(encoded_instruction **instruction_line, char *word_in_binary)
{
    int binary_index = 0, finished_bits = 0;

    /*
     * Convert each field of the encoded instruction to binary, and append it to the array: word_in_binary
     * After converting each field, we add zeros to the array to fill out the rest of the field.
     * At the end, we reverse the array to get the correct binary representation of the word.
     *
     * Notes:
     * 1. We are starting with 'E' which is the first field of a "word", and ending with 'opcode' which is the last field of a "word"
     * 2. The expression: [finished_bits + "field"_NUM_OF_BITS - binary_index] is true because:
     *   - finished_bits: the number of bits that have been added to the array in the previous fields.
     *   - "field"_NUM_OF_BITS: the number of bits that the current field should have.
     *   - binary_index: the current index in the array.
     */
    integer_to_binary((*instruction_line)->E, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + E_NUM_OF_BITS - binary_index);
    finished_bits += E_NUM_OF_BITS;

    integer_to_binary((*instruction_line)->R, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + R_NUM_OF_BITS - binary_index);
    finished_bits += R_NUM_OF_BITS;

    integer_to_binary((*instruction_line)->A, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + A_NUM_OF_BITS - binary_index);
    finished_bits += A_NUM_OF_BITS;

    integer_to_binary((*instruction_line)->funct, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + FUNCT_NUM_OF_BITS - binary_index);
    finished_bits += FUNCT_NUM_OF_BITS;

    integer_to_binary((*instruction_line)->destination_register, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + DESTINATION_REGISTER_NUM_OF_BITS - binary_index);
    finished_bits += DESTINATION_REGISTER_NUM_OF_BITS;

    integer_to_binary((*instruction_line)->destination_addressing_mode, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + DESTINATION_ADDRESSING_MODE_NUM_OF_BITS - binary_index);
    finished_bits += DESTINATION_ADDRESSING_MODE_NUM_OF_BITS;

    integer_to_binary((*instruction_line)->source_register, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + SOURCE_REGISTER_NUM_OF_BITS - binary_index);
    finished_bits += SOURCE_REGISTER_NUM_OF_BITS;

    integer_to_binary((*instruction_line)->source_addressing_mode, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + SOURCE_ADDRESSING_MODE_NUM_OF_BITS - binary_index);
    finished_bits += SOURCE_ADDRESSING_MODE_NUM_OF_BITS;

    integer_to_binary((*instruction_line)->opcode, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + OPCODE_NUM_OF_BITS - binary_index);
    finished_bits += OPCODE_NUM_OF_BITS;

    /* Add null terminator */
    word_in_binary[binary_index] = '\0';

    /* Reverse the string to get the correct binary representation */
    reverse_string(word_in_binary);
}

void operand_word_to_binary(encoded_instruction **instruction_line, code_data_array **code, int *IC, char *word_in_binary, int is_destination_operand)
{
    int addressing_mode;

    /* Get the addressing mode of the operand */
    if (is_destination_operand)
        addressing_mode = (*instruction_line)->destination_addressing_mode;
    else
        /* Source operand */
        addressing_mode = (*instruction_line)->source_addressing_mode;

    if (addressing_mode == IMMEDIATE_ADDRESSING_MODE || addressing_mode == DIRECT_ADDRESSING_MODE || addressing_mode == RELATIVE_ADDRESSING_MODE)
    {/* Addressing mode is not Direct register => a word needs to be added */
        if (addressing_mode == IMMEDIATE_ADDRESSING_MODE)
        {
            /* Create the word representing the immediate addressing mode */
            immediate_operand_word_to_binary(instruction_line, word_in_binary, is_destination_operand);
            /* Add word to code table */
            add_to_code_table(code, IC, word_in_binary, NULL);
        }
        else
        {
            /*
             * Create an empty word (Because Direct and Relative addressing modes can be implemented only in second pass)
             * Save the label name for the second pass, and add the word to the code table
             */
            initialize_array_with_zeros(word_in_binary, WORD_SIZE);
            if (is_destination_operand)
                add_to_code_table(code, IC, word_in_binary, (*instruction_line)->destination_operand);
            else
                add_to_code_table(code, IC, word_in_binary, (*instruction_line)->source_operand);
        }
    }
}

void immediate_operand_word_to_binary(encoded_instruction **instruction_line, char *word_in_binary, int is_destination_operand)
{
    int operand, binary_index = 0, finished_bits = 0;

    /* Get the decimal value of the operand */
    if (is_destination_operand)
    {
        operand = strtol((*instruction_line)->destination_operand + 1, NULL, DECIMAL_BASE); /* +1 in order to skip the '#' character */
    }
    else
    {/* Source operand */
        operand = strtol((*instruction_line)->source_operand + 1, NULL, DECIMAL_BASE); /* +1 in order to skip the '#' character */
    }
    /*
     * Use two's complement representation (if necessary)
     *     A. [~(-operand) + 1] => Is the two's complement representation of 'operand'
     *     B. [& ((1 << 21) - 1)] => Ensures 21-bit representation (by performing a bitwise AND operation with a binary number of 21 ones [(1 << 21) - 1])
     */
    if (operand < 0)
        operand = (~(-operand) + 1) & ((1 << ADDITIONAL_WORD_LENGTH_IN_BITS) - 1);

    /*
     * Similar to first_word_to_binary function
     */
    integer_to_binary((*instruction_line)->E, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + E_NUM_OF_BITS - binary_index);
    finished_bits += E_NUM_OF_BITS;

    integer_to_binary((*instruction_line)->R, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + R_NUM_OF_BITS - binary_index);
    finished_bits += R_NUM_OF_BITS;

    integer_to_binary((*instruction_line)->A, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + A_NUM_OF_BITS - binary_index);
    finished_bits += A_NUM_OF_BITS;

    /* Convert 'operand' to binary, and save it */
    /* Add zeros to fill out the rest of the field (21 bits) */
    integer_to_binary(operand, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + ADDITIONAL_WORD_LENGTH_IN_BITS - binary_index);
    finished_bits += ADDITIONAL_WORD_LENGTH_IN_BITS;

    /* Add null terminator */
    word_in_binary[binary_index] = '\0';

    /* Reverse the string to get the correct binary representation */
    reverse_string(word_in_binary);
}

void directive_to_binary(char **ptr, char *directive_name, code_data_array **data, label_table **label_table, int *DC, int *label_table_lines)
{
    char word_in_binary[WORD_SIZE]; /* Array to store the binary representation of a word */
    char *directive_number = NULL, *directive_string = NULL, *label_name = NULL;  /* Initialize to NULL in order to 'free' an initialized variable */
    int num_of_data_parameters = 0;

    if (strcmp(directive_name, ".data") == 0)
    {/* .data directive */
        directive_number = get_next_directive_number(ptr, 0);
        while (directive_number != NULL)
        {/* While there are numbers in the line */
            num_of_data_parameters++;
            /* Convert the directive number to binary and add it to the data table */
            directive_number_to_binary(directive_number, word_in_binary);
            add_to_data_table(data, DC, word_in_binary);
            /* Get the next number */
            directive_number = get_next_directive_number(ptr, 1);
        }

        if (num_of_data_parameters == 0)
        {/* .data directive must contain at least one number */
            print_error(ERROR_32, AM_FILE_STAGE);
        }
        /* Print error if extra characters are found at the end of the line */
        check_no_extra_chars(ptr);
    }
    else if (strcmp(directive_name, ".string") == 0)
    {/* .string directive */
        directive_string = get_directive_string(ptr);
        if (directive_string != NULL)
            /* Convert the directive string to binary and add it to the data table */
            insert_directive_string_into_data_table(directive_string, word_in_binary, data, DC);

        /* Print error if extra characters are found at the end of the line */
        check_no_extra_chars(ptr);
    }
    else if (strcmp(directive_name, ".entry") == 0)
    {/* .entry directive */
        /* .entry directive is handled in the second pass => Do nothing */
    }
    else
    {/* strcmp(directive_name, ".extern") == 0   => .extern directive */
        label_name = get_label_name_and_advance(ptr);
        if (label_name != NULL)
        {/* Label found => add it to label table */
            add_to_label_table(label_table, 0, label_name, NULL, EXTERN_TYPE, label_table_lines);
        }
        /* Print error if extra characters are found at the end of the line */
        check_no_extra_chars(ptr);
    }

    /* Free allocated memory */
    free(directive_number);
    free(directive_string);
}

char *get_next_directive_number(char **ptr, int num_of_commas_expected)
{
    char *directive_number;
    int num_of_digits = 0; /* num_of_digits includes '+' and '-' signs */
    int commas_count;

    commas_count = skip_whitespaces_and_commas(ptr);
    if (**ptr != '\0' && **ptr != '\n')
    {/* We are not at the end of the line */
        if (commas_count != num_of_commas_expected)
        {/* Invalid number of commas between parameters/names */
            print_error(ERROR_21, AM_FILE_STAGE);
        }
    }
    else
        /* We have reached the end of the line => No more numbers */
        return NULL;

    /* Find the length of the next directive number */
    while (!isspace(*(*ptr + num_of_digits)) && *(*ptr + num_of_digits) != ',' && *(*ptr + num_of_digits) != '\n' && *(*ptr + num_of_digits) != '\0')
    {/* While integer end is not reached */
        /* Increment label_length */
        num_of_digits++;
    }

    /* Allocate memory for directive_number */
    directive_number = malloc((num_of_digits + 1) * sizeof(char));  /* +1 for \0 */

    /* Copy integer into directive_number */
    strncpy(directive_number, *ptr, num_of_digits);
    *(directive_number + num_of_digits) = '\0';  /* Null termination */

    /* Update pointer location */
    *ptr += num_of_digits;

    if (!is_valid_number(directive_number))
    {/* directive_number is not a valid number */
        free(directive_number);
        return NULL;
    }
    return directive_number;
}

void directive_number_to_binary(char *directive_number, char *word_in_binary)
{
    int integer, binary_index = 0;
    if (isalpha(directive_number[0]))
        /* First character is a letter => get its ascii value */
        integer = (int)directive_number[0];
    else
        /* String represents a number => get the number */
        integer = strtol(directive_number, NULL, DECIMAL_BASE);

    /*
     * Use two's complement representation (if necessary)
     *     A. [~(-integer) + 1] => Is the two's complement representation of 'integer'
     *     B. [& ((1 << 24) - 1)] => Ensures 24-bit representation (by performing a bitwise AND operation with a binary number of 24 ones [(1 << 24) - 1])
     */
    if (integer < 0)
        integer = (~(-integer) + 1) & ((1 << WORD_SIZE) - 1);

    /*
     * Similar to first_word_to_binary function.
     *
     * Convert 'integer' to binary, and save it
     * Add zeros to fill out the rest of the field (24 bits)
     */
    integer_to_binary(integer, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, WORD_SIZE - binary_index);

    /* Add null terminator */
    word_in_binary[binary_index] = '\0';

    /* Reverse the string to get the correct binary representation */
    reverse_string(word_in_binary);
}

char *get_directive_string(char **ptr)
{
    char *directive_string;
    int directive_string_length = 0;
    int error_found = 0;  /* Error found in directive string: 0 - No error, 1 - Missing " at the beginning, 2 - Non-ASCII char, 3 - Missing " at the end */

    /* Skip whitespaces */
    while (isspace(**ptr))
        (*ptr)++;

    if (**ptr != '"')
    {/* A valid directive string must start with a " char */
        print_error(ERROR_24, AM_FILE_STAGE);
        error_found = 1;
    }
    else
        (*ptr)++;  /* Skip the " char */

    /* Find the number of characters in the directive string */
    while (*(*ptr + directive_string_length) != '"' && *(*ptr + directive_string_length) != '\n' && *(*ptr + directive_string_length) != '\0')
    {/* While neither " nor string end is reached */
        if (!is_ascii(*(*ptr + directive_string_length)) && error_found != 2) /* [&& error_found != 2] in order to print the error message only once */
        {/* A valid directive string must contain only ASCII characters */
            print_error(ERROR_25, AM_FILE_STAGE);
            error_found = 2;
        }
        /* Increment directive_string_length */
        directive_string_length++;
    }

    if (*(*ptr + directive_string_length) != '"')
    {/* A valid directive string must end with a " char */
        print_error(ERROR_26, AM_FILE_STAGE);
        error_found = 3;
    }

    /* Allocate memory for directive_string */
    directive_string = malloc((directive_string_length + 1) * sizeof(char));  /* +1 for the char \0 */

    /* Copy string into directive_string */
    strncpy(directive_string, *ptr, directive_string_length);
    *(directive_string + directive_string_length) = '\0';  /* Null termination */

    /* Update pointer location */
    *ptr += directive_string_length + 1;  /* +1 to skip the last " char */

    if (error_found)
    {/* directive string is not valid */
        free(directive_string);
        return NULL;
    }
    return directive_string;
}

void insert_directive_string_into_data_table(char *directive_string, char *word_in_binary, code_data_array **data, int *DC)
{
    int i = 0;
    char string_char[2]; /* Array that contains one character and a null terminator */

    /* Insert each character of the directive string into the data table */
    while (directive_string[i] != '\0')
    {
        string_char[0] = directive_string[i++];     /* Get the next character */
        string_char[1] = '\0';                      /* Null termination */
        /* Convert the character to binary and add it to the data table */
        directive_number_to_binary(string_char, word_in_binary);
        add_to_data_table(data, DC, word_in_binary);
    }

    /* Add '\0' into data table */
    string_char[0] = '\0';                          /* Add null terminator (as the value) */
    string_char[1] = '\0';                          /* Null termination */
    directive_number_to_binary(string_char, word_in_binary);
    add_to_data_table(data, DC, word_in_binary);
}

void direct_operand_word_to_binary(int address, char *word_in_binary)
{
    int A, R, E, binary_index = 0, finished_bits = 0;

    if (address == 0)
    {/* External address */
        A = 0;
        R = 0;
        E = 1;
    }
    else
    {/* Internal address */
        A = 0;
        R = 1;
        E = 0;
    }

    /*
     * Similar to first_word_to_binary function
     */
    integer_to_binary(E, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + E_NUM_OF_BITS - binary_index);
    finished_bits += E_NUM_OF_BITS;

    integer_to_binary(R, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + R_NUM_OF_BITS - binary_index);
    finished_bits += R_NUM_OF_BITS;

    integer_to_binary(A, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + A_NUM_OF_BITS - binary_index);
    finished_bits += A_NUM_OF_BITS;

    /* Convert 'address' to binary, and save it */
    /* Add zeros to fill out the rest of the field (21 bits) */
    integer_to_binary(address, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + ADDITIONAL_WORD_LENGTH_IN_BITS - binary_index);
    finished_bits += ADDITIONAL_WORD_LENGTH_IN_BITS;

    /* Add null terminator */
    word_in_binary[binary_index] = '\0';

    /* Reverse the string to get the correct binary representation */
    reverse_string(word_in_binary);
}

void relative_operand_word_to_binary(int jumping_distance, char *word_in_binary)
{
    int A, R, E, binary_index = 0, finished_bits = 0;

    /* In relative addressing mode: A,R,E are always 1,0,0 */
    A = 1;
    R = 0;
    E = 0;

    /* Use two's complement representation (if necessary) - As explained in the 'immediate_operand_word_to_binary' function */
    if (jumping_distance < 0)
        jumping_distance = (~(-jumping_distance) + 1) & ((1 << ADDITIONAL_WORD_LENGTH_IN_BITS) - 1);

    /*
     * Similar to first_word_to_binary function
     */
    integer_to_binary(E, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + E_NUM_OF_BITS - binary_index);
    finished_bits += E_NUM_OF_BITS;

    integer_to_binary(R, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + R_NUM_OF_BITS - binary_index);
    finished_bits += R_NUM_OF_BITS;

    integer_to_binary(A, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + A_NUM_OF_BITS - binary_index);
    finished_bits += A_NUM_OF_BITS;

    /* Convert 'jumping_distance' to binary, and save it */
    /* Add zeros to fill out the rest of the field (21 bits) */
    integer_to_binary(jumping_distance, word_in_binary, &binary_index);
    add_zeros(word_in_binary, &binary_index, finished_bits + ADDITIONAL_WORD_LENGTH_IN_BITS - binary_index);
    finished_bits += ADDITIONAL_WORD_LENGTH_IN_BITS;

    /* Add null terminator */
    word_in_binary[binary_index] = '\0';

    /* Reverse the string to get the correct binary representation */
    reverse_string(word_in_binary);
}