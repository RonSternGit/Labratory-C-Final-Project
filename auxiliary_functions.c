#include "general_header.h"
#include "auxiliary_functions.h"
#include "errors.h"


/* This class contains auxiliary functions that are used in the assembler program. */


char* get_file_name(int num_of_args, char *first_arg, ...)
{
    char *temp_name, *file_name;
    int i, file_name_length = strlen(first_arg);  /* Initialize to length of 'first_arg' */

    /* Create a variable argument list */
    va_list args;

    /* Initialize the argument list */
    va_start(args, first_arg);

    /* Append the length of each argument in the list to 'file_name_length' (va_arg returns the next argument from the list). */
    for (i = 1; i < num_of_args; i++)
    {/* 'i' is initialized to 1 in order to skip the first argument */
        /* Get next argument */
        temp_name = va_arg(args, char *);
        /* Append length of argument to 'file_name_length' */
        file_name_length += strlen(temp_name);
    }

    /* Allocate memory for the concatenated file name */
    file_name = safe_malloc((file_name_length + 1) * sizeof(char));  /* +1 for \0 */

    /* Initialize file_name with 'first_arg' */
    strcpy(file_name, first_arg);

    /* Reset the argument list to point to the beginning */
    va_start(args, first_arg);

    /* Append each argument in the list to 'file_name' */
    for (i = 1; i < num_of_args; i++)
    {/* 'i' is initialized to 1 in order to skip the first argument */
        /* Get next argument */
        temp_name = va_arg(args, char *);
        /* Append argument to 'file_name' */
        strcat(file_name, temp_name);
    }

    /* Clean up the argument list */
    va_end(args);

    return file_name;
}

void trim_leading_whitespaces(char *original_file_name, char *trimmed_file_name)
{
    FILE *original_file = safe_fopen(original_file_name, "r");
    FILE *trimmed_file = safe_fopen(trimmed_file_name, "w");
    char line[MAX_LINE_LENGTH];     /* Buffer for each line */
    char *ptr = 0;                  /* Initialize */

    while (fgets(line, sizeof(line), original_file))
    {/* Read each line from the original file */
        ptr = line;
        /* Skip leading whitespaces */
        while (isspace(*ptr))
        {
            ptr++;
        }
        /* Copy the trimmed line to trimmed_file */
        fputs(ptr, trimmed_file);
    }

    /* Close opened files */
    fclose(original_file);
    fclose(trimmed_file);
}

int skip_whitespaces_and_commas(char **ptr)
{
    int commas_count = 0;
    while (isspace(**ptr) || **ptr == ',')
    {
        if (**ptr == ',')
            /* Increase commas count */
            commas_count++;
        /* Skip character */
        (*ptr)++;
    }
    return commas_count;
}

int is_valid_number(char *ptr)
{
    /* Skip '-' or '+' sign if exists */
    if (*ptr == '-' || *ptr == '+')
        ptr++;

    if (*ptr == '0')
    {/* Number cannot start with 0 */
        print_error(ERROR_22, AM_FILE_STAGE);
        return 0;
    }

    while (*ptr != '\0')
    {
        if (!isdigit(*ptr++))
        {/* A valid number consists of digits only */
            print_error(ERROR_23, AM_FILE_STAGE);
            return 0;
        }
    }
    /* Number is valid */
    return 1;
}

int is_valid_label_syntax(char *operand)
{
    if (!isalpha(*operand))
        /* Label must start with an alphabetic character */
        return 0;

    /* Start checking from next char */
    operand++;

    while (isalpha(*operand) || isdigit(*operand))
    {/* Label contains only alphabetic characters and digits */
        operand++;
    }

    if (*operand != '\0')
        /* Label contains char that is not: alphabetic nor digit */
        return 0;

    /* Valid label - starts with alphabetic char, and contains only alphabetic chars and digits */
    return 1;
}

void reverse_string(char *string)
{
    int i, j;
    char temp;
    for (i = 0, j = WORD_SIZE - 1; i < j; i++, j--)
    {/* Swap characters */
        temp = string[i];
        string[i] = string[j];
        string[j] = temp;
    }
}

int is_ascii(int c)
{
    return c >= ASCII_MIN && c <= ASCII_MAX;
}

void initialize_array_with_zeros(char *array, int array_length)
{
    int i;
    for (i = 0; i < array_length; i++)
    {
        array[i] = '0';
    }
    /* Add null terminator */
    array[array_length] = '\0';
}

void check_no_extra_chars(char **ptr)
{
    /* Skip whitespaces */
    while (isspace(**ptr))
        (*ptr)++;

    if (**ptr != '\n' && **ptr != '\0')
    {/* End of line has not been reached */
        print_error(ERROR_27, AM_FILE_STAGE);
    }
}

int contains_non_ascii_chars(char *name)
{
    while (*name != '\0')
    {
        if (!is_ascii(*name))
            return 1;
        /* Skip to next char */
        name++;
    }
    return 0;
}