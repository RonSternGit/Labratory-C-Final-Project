#include "general_header.h"
#include "auxiliary_functions.h"
#include "pre_assembler.h"
#include "errors.h"
#include "parser.h"


/* This class is used to store the macro name and its content */


int pre_assembler_stage(char *file_name)
{
    /* File names */
    char *original_file_name, *trimmed_file_name, *expanded_file_name;
    /* File pointers */
    FILE *first_assembly_file, *second_assembly_file;
    /* Macro list */
    Macro *head = NULL;

    /* Get original file name (ends with - ".as") */
    original_file_name = get_file_name(2, file_name, ".as");

    /* Get trimmed file name (ends with - ".as") */
    trimmed_file_name = get_file_name(3, "trimmed_", file_name, ".as");

    /* Get expanded file name (ends with - ".am") - the macro expanded file is ending with '.am' */
    expanded_file_name = get_file_name(2, file_name, ".am");

    /* Remove white spaces at beginning of each line, and put the result in trimmed_file_name */
    trim_leading_whitespaces(original_file_name, trimmed_file_name);

    /* Open first_assembly_file (the trimmed one is opened) */
    first_assembly_file = safe_fopen(trimmed_file_name, "r");

    /* Save macros */
    save_macros(first_assembly_file, &head);

    /* Open expanded file (it is empty at the beginning) */
    second_assembly_file = safe_fopen(expanded_file_name, "w");

    /* Rewind first_assembly_file to beginning in order to scan it again */
    rewind(first_assembly_file);

    /* Replace macros (in expanded file) */
    replace_macros(first_assembly_file, second_assembly_file, head);

    /* Free macro list */
    free_macro_list(head);

    /* Close opened files */
    fclose(first_assembly_file);
    fclose(second_assembly_file);

    /* Remove the trimmed file - which is not needed anymore */
    remove(trimmed_file_name);

    /* Free file names */
    free(original_file_name);
    free(trimmed_file_name);
    free(expanded_file_name);

    /* Return error number (0 if no error found) */
    return current_error_number;
}

void save_macros(FILE *file, Macro **head)
{
    char *macro_name = NULL;
    char *macro_content = NULL;
    char line[MAX_LINE_LENGTH];              /* Buffer to store the current line */
    char extra_characters[MAX_LINE_LENGTH];  /* Array to store extra characters found in the macro definition/ending line (if exist) */
    int temp_char;                           /* Temporary variable for 'line is too long' error check */

    while (fgets(line, sizeof(line), file))
    {
        /* Update current line number */
        current_line_number++;

        /* If buffer is full and EOF is not reached => line is too long (and was truncated) */
        if (strlen(line) >= MAX_LINE_LENGTH - 1 && !feof(file))
        {
            print_error(ERROR_4, AS_FILE_STAGE);
            /* Skip the rest of the line */
            while ((temp_char = fgetc(file)) != EOF && temp_char != '\n')
                ;
        }
        if (strncmp(line, MACRO_START, strlen(MACRO_START)) == 0)
        {/* Macro start has been reached */
            /* Save name */
            macro_name = safe_malloc(strlen(line) * sizeof(char));
            /* Check for "Macro definition/ending contains extra characters" error */
            *extra_characters = '\0';  /* Reset 'extra_characters' array in order not to save content for iteration to iteration */
            sscanf(line, "mcro %s %s", macro_name, extra_characters);

            /* Check for errors and print them if exist */
            if (strlen(extra_characters) > 0)
            {/* Extra characters were found in the macro definition line */
                print_error(ERROR_7, AS_FILE_STAGE);
            }
            if (is_reserved_name(macro_name))
            {/* Macro name is equal to instruction/directive/register name */
                print_error(ERROR_5, AS_FILE_STAGE);
            }
            if (find_macro_in_list(*head, macro_name) != NULL)
            {/* Macro name already exists in the list */
                print_error(ERROR_6, AS_FILE_STAGE);
            }
            if (contains_non_ascii_chars(macro_name))
            {/* Macro name must contain only ASCII characters */
                print_error(ERROR_35, AS_FILE_STAGE);
            }

            /* Initialize macro_content to a valid pointer in order to prevent error using realloc */
            macro_content = safe_malloc(1);
            macro_content[0] = '\0';

            /* While end of macro is not reached */
            while (fgets(line, sizeof(line), file) && strncmp(line, MACRO_END, strlen(MACRO_END)) != 0)
            {
                /* Update current line number */
                current_line_number++;
                /* Reallocate memory for macro_content (define its memory to: current memory + next line memory) */
                macro_content = safe_realloc(macro_content, (strlen(macro_content) + strlen(line) + 1) * sizeof(char)); /* +1 for \0 */
                /* Read line and add it to macro_content */
                strcat(macro_content, line);
            }

            /* Update current line number (for the 'mcroend' line) */
            current_line_number++;

            /* Check for "Macro definition/ending contains extra characters" error */
            *extra_characters = '\0';  /* Reset 'extra_characters' array in order not to save content for iteration to iteration */
            sscanf(line, "mcroend %s", extra_characters);
            if (strlen(extra_characters) > 0)
            {/* Extra characters were found in the macro ending line */
                print_error(ERROR_40, AS_FILE_STAGE);
            }
            /* Macro is valid => add it to macro list */
            add_macro_to_list(head, macro_name, macro_content);
        }
    }
    /* Reset current line number */
    current_line_number = 0;
}

void replace_macros(FILE *file1, FILE *file2, Macro *head)
{
    char line[MAX_LINE_LENGTH];        /* Buffer to store the current line */
    char *macro_content = NULL;
    char *label_name = NULL;

    while (fgets(line, sizeof(line), file1))
    {
        /* Update current line number */
        current_line_number++;

        if ((label_name = get_label_name(line)) != NULL)
        {/* Label has been found */
            /* Check if label name matches any macro name */
            if (find_macro_in_list(head, label_name))
            {/* Label name is equal to a macro name */
                print_error(ERROR_13, AS_FILE_STAGE);
            }
        }

        if (strncmp(line, MACRO_START, strlen(MACRO_START)) == 0)
        {/* Macro definition has been reached */
            /* Skip macro definition (because it is not part of expanded file) */
            while (fgets(line, sizeof(line), file1) && strncmp(line, MACRO_END, strlen(MACRO_END)) != 0)
                ;
        }
        else if ((macro_content = find_macro_in_list(head, line)) != NULL)
        {/* Call for macro has been reached */
            /* Write macro's content in the expanded file */
            fputs(macro_content, file2);
        }
        else
        {/* Text not related to macro */
            /* Write it in the expanded file */
            fputs(line, file2);
        }
    }
    /* Reset current line number */
    current_line_number = 0;
}

void add_macro_to_list(Macro **head, char *name, char *content)
{
    Macro *new_macro = safe_malloc(sizeof(Macro)), *temp_macro;

    if (find_macro_in_list(*head, name) != NULL)
    {/* Macro name already exists in the list => don't add it to list (return) */
        return;
    }

    /* Save macro's name and content */
    new_macro->name = name;
    new_macro->content = content;
    /* NULL because it will be stored at the end of the list */
    new_macro->next = NULL;

    /* If it is the first macro => store new_macro at the beginning */
    if (*head == NULL)
        *head = new_macro;
    else
    {/* Go to the last node and set it's next to new_macro */
        temp_macro = *head;
        while (temp_macro->next != NULL)
            temp_macro = temp_macro->next;
        temp_macro->next = new_macro;
    }
}

char *find_macro_in_list(Macro *head, char *name)
{
    while (head)
    {
        /* strncmp is used in order to compare the names without \n (if exists) */
        if (strncmp(head->name, name, strlen(head->name)) == 0)
            return head->content;
        head = head->next;
    }
    /* Macro was not found */
    return NULL;
}

void free_macro_list(Macro *head)
{
    Macro *next;
    while (head != NULL)
    {/* Go over macros in macro list */
        /* Save next macro */
        next = head->next;
        /* Free current macro */
        free(head->name);
        free(head->content);
        free(head);
        /* Move to next macro */
        head = next;
    }
}