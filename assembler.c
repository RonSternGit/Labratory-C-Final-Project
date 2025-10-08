#include "errors.h"
#include "pre_assembler.h"
#include "first_pass.h"


/* This is the main function of the assembler program.
 * It processes each file passed as a command line argument.
 * For each file, it performs the pre-assembler stage, the first and second pass stages.
 */
int main(int argc, char *argv[])
{
    int i;

    for (i = 1; i < argc; i++)
    {
        /* Reset the error number */
        current_error_number = ERROR_0;

        /* Perform the pre-assembler stage */
        if (pre_assembler_stage(argv[i]) != ERROR_0)
        {
            /* If it failed, move to the next file */
            continue;
        }

        /* Perform the first pass stage (and second pass stage which is inside 'first_pass_stage') */
        if (first_pass_stage(argv[i]) != ERROR_0)
        {
            /* If it failed, skip the 'Program succeeded' message and move to the next file */
            continue;
        }

        printf("Program succeeded for file: %s\n", argv[i]);
    }

    /* End of the program */
    return 0;
}
