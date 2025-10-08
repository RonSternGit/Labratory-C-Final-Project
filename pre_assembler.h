#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H


/** A structure to store macro names and their content (for the macro list) */
typedef struct macro_node {
    char *name;
    char *content;
    struct macro_node *next;
} Macro;


/**
 * Adds a new macro to the linked list of macros.
 *
 * Input:
 *   - head: Pointer to the pointer of the head of the macro list
 *   - name: String containing the name of the macro to add
 *   - content: String containing the content/body of the macro
 *
 * Output:
 *   - No return value
 */
void add_macro_to_list(Macro **head, char *name, char *content);


/**
 * Saves the macro definitions from file in the macro list.
 *
 * Input:
 *   - file: Pointer to the file where macros will be written
 *   - head: Pointer to the pointer of the head of the macro list
 *
 * Output:
 *   - No return value
 */
void save_macros(FILE *file, Macro **head);


/**
 * Replaces macro calls in the source file with their content and writes to the expanded file.
 *
 * Input:
 *   - file1: Pointer to the source file containing potential macro calls
 *   - file2: Pointer to the destination file where expanded code will be written
 *   - head: Pointer to the head of the macro list
 *
 * Output:
 *   - No return value
 */
void replace_macros(FILE *file1, FILE *file2, Macro *head);


/**
 * Searches for a macro by name in the linked list of macros.
 *
 * Input:
 *   - head: Pointer to the head of the macro list
 *   - name: String containing the name of the macro to search for
 *
 * Output:
 *   - Returns a pointer to the macro's content if found, NULL otherwise
 */
char *find_macro_in_list(Macro *head, char *name);


/**
 * Handles the pre-assembly stage by processing macros in the assembly file.
 *
 * Input:
 *   - file_name: String containing the name of the assembly file to process
 *
 * Output:
 *   - Returns an integer representing the error status (0 if no error found)
 */
int pre_assembler_stage(char *file_name);


/**
 * Frees all memory allocated for the linked list of macros.
 *
 * Input:
 *   - head: Pointer to the head of the macro list
 *
 * Output:
 *   - No return value
 */
void free_macro_list(Macro *head);


#endif /* PRE_ASSEMBLER_H */