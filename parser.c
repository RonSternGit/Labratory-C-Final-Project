#include "first_pass.h"
#include "parser.h"
#include "auxiliary_functions.h"
#include "errors.h"


/* This class provides functions for parsing assembly instruction names, operands, directives, and addressing modes */


/** A table that contains the instruction name, opcode, funct, valid addressing modes for source and destination operands, and the number of arguments for each instruction.
 * Each instruction is represented by a structure of type 'instruction_info'.
 * END_OF_ARRAY is used to mark the end of an array. */
instruction_info INSTRUCTIONS[NUM_OF_INSTRUCTIONS] = {
    {"mov", 0, 0, {0,1,3}, {1,3, END_OF_ARRAY}, 2},
    {"cmp", 1, 0, {0,1,3}, {0,1,3}, 2},
    {"add", 2, 1, {0,1,3}, {1,3, END_OF_ARRAY}, 2},
    {"sub", 2, 2, {0,1,3}, {1,3, END_OF_ARRAY}, 2},
    {"lea", 4, 0, {1, END_OF_ARRAY}, {1,3, END_OF_ARRAY}, 2},
    {"clr", 5, 1, {END_OF_ARRAY}, {1,3, END_OF_ARRAY}, 1},
    {"not", 5, 2, {END_OF_ARRAY}, {1,3, END_OF_ARRAY}, 1},
    {"inc", 5, 3, {END_OF_ARRAY}, {1,3, END_OF_ARRAY}, 1},
    {"dec", 5, 4, {END_OF_ARRAY}, {1,3, END_OF_ARRAY}, 1},
    {"jmp", 9, 1, {END_OF_ARRAY}, {1,2, END_OF_ARRAY}, 1},
    {"bne", 9, 2, {END_OF_ARRAY}, {1,2, END_OF_ARRAY}, 1},
    {"jsr", 9, 3, {END_OF_ARRAY}, {1,2, END_OF_ARRAY}, 1},
    {"red", 12, 0, {END_OF_ARRAY}, {1,3, END_OF_ARRAY}, 1},
    {"prn", 13, 0, {END_OF_ARRAY}, {0,1,3}, 1},
    {"rts", 14, 0, {END_OF_ARRAY}, {END_OF_ARRAY}, 0},
    {"stop", 15, 0, {END_OF_ARRAY}, {END_OF_ARRAY}, 0}
};

/** A table that contains the directive names */
char* DIRECTIVES[NUM_OF_DIRECTIVES] = {".data", ".string", ".entry", ".extern"};

/** A table that contains the register names */
char* REGISTERS[NUM_OF_REGISTERS] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};


char *get_instruction_name(char **ptr)
{
    int i, instr_name_length;

    /* Search for the instruction name in the INSTRUCTIONS table */
    for (i = 0; i < NUM_OF_INSTRUCTIONS; i++)
    {
        instr_name_length = strlen(INSTRUCTIONS[i].name);
        if (strncmp(*ptr, INSTRUCTIONS[i].name, instr_name_length) == 0)
        {/* Our pointer *starts* with a certain instruction name */
            if (isspace(*(*ptr + instr_name_length)) || *(*ptr + instr_name_length) == '\0')
            {/* If it *ends* right after that - it is a valid instruction name */
                /* Update pointer location and return instruction name */
                *ptr += instr_name_length;
                return INSTRUCTIONS[i].name;
            }
        }
    }
    /* No valid instruction name has been found */
    return NULL;
}

char *get_instruction_operand(char **ptr, int num_of_commas_expected)
{
    char *instr_operand;
    int operand_length = 0;

    /* Skip whitespaces and commas, and print an error if there is an invalid number of commas */
    if (skip_whitespaces_and_commas(ptr) != num_of_commas_expected)
    {/* Invalid number of commas between parameters/names */
        print_error(ERROR_21, AM_FILE_STAGE);
    }

    /* Find the length of the operand */
    while (*(*ptr + operand_length) != ' ' && *(*ptr + operand_length) != ',' && *(*ptr + operand_length) != '\n' && *(*ptr + operand_length) != '\0')
    {/* Operand can end with: whitespace, comma, newline, null terminator */
        operand_length++;
    }

    /* Allocate memory for instr_operand */
    instr_operand = safe_malloc((operand_length + 1) * sizeof(char));  /* +1 for \0 */

    /* Copy operand into instr_operand */
    strncpy(instr_operand, *ptr, operand_length);
    *(instr_operand + operand_length) = '\0';  /* Null termination */

    /* Update pointer location */
    *ptr += operand_length;

    return instr_operand;
}

int get_instruction_num_of_args(char *name)
{
    int i;
    for (i = 0; i < NUM_OF_INSTRUCTIONS; i++)
    {/* Search for the instruction name in the INSTRUCTIONS table */
        if (strcmp(name, INSTRUCTIONS[i].name) == 0)
            return INSTRUCTIONS[i].num_of_args;
    }
    /* Invalid instruction */
    return GARBAGE_VALUE;
}

char *get_label_name(char *ptr)
{
    char *label_name;
    int label_length = 0;

    /* Skip whitespaces */
    while (isspace(*ptr))
        ptr++;

    if (!isalpha(*ptr))
        /* Label must start with an alphabetic symbol */
        return NULL;

    /* Find the length of the label */
    while (isalpha(*(ptr + label_length)) || isdigit(*(ptr + label_length)))
    {/* Label contains only alphabetic symbols and digits */
        label_length++;
    }

    if (*(ptr + label_length) != ':')
        /* Label must end with ':'  */
        return NULL;

    if (label_length > MAX_LABEL_LENGTH)
        /* Label exceeds maximum length */
        return NULL;

    /* Allocate memory for label_name */
    label_name = safe_malloc((label_length + 1) * sizeof(char));  /* +1 for \0 */

    /* Copy label into label_name */
    strncpy(label_name, ptr, label_length);
    *(label_name + label_length) = '\0';  /* Null termination */

    return label_name;
}

char *get_label_name_and_advance(char **ptr)
{
    char *label_name;
    int label_length = 0, invalid_chars_error = 0;

    /* Skip whitespaces */
    while (isspace(**ptr))
        (*ptr)++;

    /* Find the length of the label */
    while (!isspace(*(*ptr + label_length)) && *(*ptr + label_length) != '\n' && *(*ptr + label_length) != '\0')
    {/* While label end is not reached */
        /* Check for invalid characters */
        if (!isalpha(*(*ptr + label_length)) && !isdigit(*(*ptr + label_length)) && !invalid_chars_error) /* [!invalid_chars_error] to avoid printing the error message multiple times */
        {/* Error - Label name must contain only alphabetic symbols and digits */
            print_error(ERROR_18, AM_FILE_STAGE);
            invalid_chars_error = 1;
        }
        /* Increment label_length */
        label_length++;
    }

    /* Check for errors, and print error messages if needed */
    if (!isalpha(**ptr))
    {
        if (**ptr == '\0')
        {/* Missing label */
            print_error(ERROR_33, AM_FILE_STAGE);
        }
        else
        {/* Label must start with an alphabetic symbol => advance pointer, print error and return */
            print_error(ERROR_17, AM_FILE_STAGE);
        }
        /* Advance pointer to the character after the label name, and return */
        *ptr += label_length;
        return NULL;
    }

    /* Check for errors, and print error messages if needed */
    if (label_length > MAX_LABEL_LENGTH)
    {/* Label exceeds maximum length => advance pointer, print error and return */
        print_error(ERROR_19, AM_FILE_STAGE);
        /* Advance pointer to the character after the label name, and return */
        *ptr += label_length;
        return NULL;
    }

    /* Allocate memory for label_name */
    label_name = safe_malloc((label_length + 1) * sizeof(char));  /* +1 for \0 */

    /* Copy label into label_name */
    strncpy(label_name, *ptr, label_length);
    *(label_name + label_length) = '\0';  /* Null termination */

    /* Check if label name matches any reserved name */
    if (is_reserved_name(label_name))
    {
        print_error(ERROR_12, AM_FILE_STAGE);
    }

    /* Advance pointer to the character after the label name  */
    *ptr += label_length;

    return label_name;
}

int get_instruction_opcode(char *name)
{
    int i;
    for (i = 0; i < NUM_OF_INSTRUCTIONS; i++)
    {/* Search for the instruction name in the INSTRUCTIONS table */
        if (strcmp(name, INSTRUCTIONS[i].name) == 0)
            return INSTRUCTIONS[i].opcode;
    }
    /* Invalid instruction */
    return 0;
}

int get_instruction_funct(char *name)
{
    int i;
    for (i = 0; i < NUM_OF_INSTRUCTIONS; i++)
    {/* Search for the instruction name in the INSTRUCTIONS table */
        if (strcmp(name, INSTRUCTIONS[i].name) == 0)
            return INSTRUCTIONS[i].funct;
    }
    /* Invalid instruction */
    return 0;
}

int get_register_number(char *operand, int is_source_operand)
{
    char *end_ptr;      /* Temporary pointer for error checking */

    if (is_register(operand))
    {/* If operand is a valid register => return it's number */
        return *(operand + 1) - '0';
    }

    /* else => No register was found */

    /* Check for error before returning */
    if (operand != NULL && *operand == 'r')
    {/* Operand starts with 'r' */
        /* Check if the rest of the operand is a valid number greater than 7 */
        if (strtol(operand + 1, &end_ptr, DECIMAL_BASE) >= NUM_OF_REGISTERS && *end_ptr == '\0')
        {/* NO ERROR - operand is referenced to as a label */
            if (is_source_operand)
                print_error(ERROR_38, AM_FILE_STAGE);
            else
                /* Destination operand */
                print_error(ERROR_39, AM_FILE_STAGE);
        }
    }
    /* Return */
    return 0;
}

int get_addressing_mode(char *instr_name, char *operand)
{
    if (operand == NULL)
        /* NULL operand has no addressing mode (returning 0 is convenient for later use) */
        return 0;

    /* Find the addressing mode of the operand */
    if (is_immediate_addressing_mode(operand))
        return IMMEDIATE_ADDRESSING_MODE;
    if (is_register(operand))
        /* Addressing mode is: direct register */
        return DIRECT_REGISTER_ADDRESSING_MODE;
    if (is_relative_addressing_mode(instr_name, operand))
        return RELATIVE_ADDRESSING_MODE;
    if (is_direct_addressing_mode(operand))
        return DIRECT_ADDRESSING_MODE;
    else
        /* Invalid addressing mode */
        return GARBAGE_VALUE;
}

int is_immediate_addressing_mode(char *operand)
{
    return *operand == '#' && is_valid_number(operand + 1);
}

int is_direct_addressing_mode(char *operand)
{
    return is_valid_label_syntax(operand);
}

int is_relative_addressing_mode(char *instr_name, char *operand)
{
    if (strcmp(instr_name, "jmp") == 0 || strcmp(instr_name, "bne") == 0 || strcmp(instr_name, "jsr") == 0)
    {/* Addressing mode can be relative only if the instruction name is: "jmp" or "bne" or "jsr" */
        if (*operand == '&' && is_valid_label_syntax(operand + 1))
            return 1;
    }
    /* Addressing mode is not relative */
    return 0;
}

int is_register(char *operand)
{
    int i;

    if (operand == NULL)
        /* NULL operand cannot be a register */
        return 0;

    for (i = 0; i < NUM_OF_REGISTERS; i++)
    {/* Search for the register name in the REGISTERS table */
        if (strcmp(operand, REGISTERS[i]) == 0)
            return 1;
    }

    /* No register found */
    return 0;
}

char *get_directive_name(char **ptr)
{
    int i, directive_name_length;

    for (i = 0; i < NUM_OF_DIRECTIVES; i++)
    {/* Search for the directive name in the DIRECTIVES table */
        directive_name_length = strlen(DIRECTIVES[i]);
        if (strncmp(*ptr, DIRECTIVES[i], directive_name_length) == 0)
        {/* Our pointer *starts* with a certain directive name */
            if (isspace(*(*ptr + directive_name_length)) || *(*ptr + directive_name_length) == '\0')
            {/* If it *ends* right after that - it is a valid directive name */
                /* Update pointer location and return directive name */
                *ptr += directive_name_length;
                return DIRECTIVES[i];
            }
        }
    }
    /* No valid directive name has been found */
    return NULL;
}

int is_reserved_name(char *name)
{
    /* Create duplicate of name (because 'get_instruction_name' and 'get_directive_name' change the pointing of the first char of the pointer they get) */
    char *temp = safe_malloc((strlen(name) + 1) * sizeof(char)); /* +1 for \0 */
    strcpy(temp, name);

    /* Check if name is equal to an instruction name */
    if (get_instruction_name(&temp) != NULL)
    {/* 'name' is equal to instruction name */
        return 1;
    }

    /* Reset 'temp' because 'get_instruction_name' changed the first char pointed to */
    strcpy(temp, name);

    /* Check if name is equal to a directive name */
    if (get_directive_name(&temp) != NULL)
    {/* 'name' is equal to directive name */
        return 1;
    }

    /* Check if name is equal to a register name */
    if (is_register(name))
    {/* 'name' is equal to register name */
        return 1;
    }

    /* Free temporary variable */
    free(temp);

    /* Name is valid */
    return 0;
}

void check_validity_of_label_name(char **ptr)
{
    char *label_name;
    int label_length = 0, invalid_chars_error = 0;

    /* Print error if needed */
    if (!isalpha(**ptr))
    {
        if (**ptr == '\0')
        {/* Missing label */
            print_error(ERROR_33, AM_FILE_STAGE);
        }
        else
        {/* Label must start with an alphabetic symbol */
            print_error(ERROR_17, AM_FILE_STAGE);
        }
    }

    /* Find the length of the label */
    while (*(*ptr + label_length) != ':')
    {/* While label end is not reached */
        /* Check for invalid characters */
        if (!isalpha(*(*ptr + label_length)) && !isdigit(*(*ptr + label_length)) && !invalid_chars_error) /* [!invalid_chars_error] to avoid printing the error message multiple times */
        {/* Error - Label name must contain only alphabetic symbols and digits */
            print_error(ERROR_18, AM_FILE_STAGE);
            invalid_chars_error = 1;
        }
        /* Increment label_length */
        label_length++;
    }

    if (label_length > MAX_LABEL_LENGTH)
    {/* Label exceeds maximum length */
        print_error(ERROR_19, AM_FILE_STAGE);
    }

    /* Save label name */
    label_name = safe_malloc((label_length + 1) * sizeof(char));  /* +1 for \0 */
    strncpy(label_name, *ptr, label_length);
    *(label_name + label_length) = '\0';  /* Null termination */
    /* Check if label name matches any reserved name */
    if (is_reserved_name(label_name))
    {
        print_error(ERROR_12, AM_FILE_STAGE);
    }

    /* Move pointer to the character after ':'  */
    *ptr += label_length + 1;

    /* Free temporary variable */
    free(label_name);
}

int invalid_instruction_operand_type(char *instr_name, int operand_type, int is_source_operand)
{
    int i, j;
    int *valid_types_array;  /* Pointer to the array of valid addressing modes for the instruction */

    for (i = 0; i < NUM_OF_INSTRUCTIONS; i++)
    {
        if (strcmp(instr_name, INSTRUCTIONS[i].name) == 0)
        {/* Found the instruction name */
            /* Save the array of valid addressing modes for the instruction */
            valid_types_array = is_source_operand ? INSTRUCTIONS[i].src_valid_addressing_modes : INSTRUCTIONS[i].dest_valid_addressing_modes;
            for (j = 0; j < MAX_NUM_OF_VALID_ADDRESSING_MODES_FOR_PARAMETER && valid_types_array[j] != END_OF_ARRAY; j++)
            {/* Check if the operand type is valid */
                if (valid_types_array[j] == operand_type)
                    return 0; /* Valid operand type */
            }
            return 1; /* Invalid operand type */
        }
    }
    /* Invalid instruction name (error was already printed) */
    return 0;
}