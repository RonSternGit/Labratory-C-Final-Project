#include "general_header.h"
#include "errors.h"


/* This class defines the error code numbers and the corresponding error message. */


/* The Following array specifies the error code numbers and the corresponding error message. */
Error ERRORS[NUM_OF_ERRORS] = {
    {ERROR_0, "No Error"},
    {ERROR_1, "Memory allocation failed"},
    {ERROR_2, "File opening failed"},
    {ERROR_3, "The number of words in the object file exceeds the maximum allowed"},
    {ERROR_4, "Line is too long"},
    {ERROR_5, "Macro name is invalid (equal to instruction/directive/register name)"},
    {ERROR_6, "Macro name is invalid (equal to an existing macro name)"},
    {ERROR_7, "Macro definition contains extra characters"},
    {ERROR_8, "Label address cannot be external in relative addressing mode"},
    {ERROR_9, "Label before entry/extern directive"},
    {ERROR_10, "Label cannot be defined as both entry and external"},
    {ERROR_11, "Label cannot be defined more than once"},
    {ERROR_12, "Label name is invalid (equal to instruction/directive/register name)"},
    {ERROR_13, "Label name is invalid (equal to macro name)"},
    {ERROR_14, "Invalid directive name"},
    {ERROR_15, "Invalid instruction name"},
    {ERROR_16, "Invalid number of instruction arguments"},
    {ERROR_17, "Label name must start with an alphabetic symbol"},
    {ERROR_18, "Label name must contain only alphabetic symbols and digits"},
    {ERROR_19, "Label name exceeds maximum length"},
    {ERROR_20, "Invalid character before directive/instruction sentence"},
    {ERROR_21, "Invalid number of commas between parameters/names"},
    {ERROR_22, "Number cannot start with 0"},
    {ERROR_23, "Number contains an illegal character"},
    {ERROR_24, "A valid directive string must start with a \" char"},
    {ERROR_25, "A valid directive string must contain only ASCII characters"},
    {ERROR_26, "A valid directive string must end with a \" char"},
    {ERROR_27, "Unexpected characters after valid input"},
    {ERROR_28, "Invalid source operand type"},
    {ERROR_29, "Invalid destination operand type"},
    {ERROR_30, "Missing source operand"},
    {ERROR_31, "Missing destination operand"},
    {ERROR_32, ".data directive must contain at least one number"},
    {ERROR_33, "Missing label"},
    {ERROR_34, "Missing instruction name"},
    {ERROR_35, "Macro name must contain only ASCII characters"},
    {ERROR_36, "Label not found in label table"},
    {ERROR_37, "Label was never declared"},
    {ERROR_38, "NO ERROR - source operand is referenced to as a label (and not as a register)"},
    {ERROR_39, "NO ERROR - destination operand is referenced to as a label (and not as a register)"},
    {ERROR_40, "Macro ending contains extra characters"},
};

/* Initialization of global variables */
int current_error_number = ERROR_0;
int current_line_number = 0;


void print_error(int error_number, int stage)
{
    /* Update current error number */
    current_error_number = error_number;

    /* Print error message
     * AS_FILE_STAGE - error in .as file
     * AM_FILE_STAGE - error in .am file
     * INTERNAL_ERROR_STAGE - internal error (not in .as or .am file)
     */
    if (stage == AS_FILE_STAGE)
        printf("Error [%d] at line %d in the .as file: %s\n", error_number, current_line_number, ERRORS[error_number].error_message);
    else if (stage == AM_FILE_STAGE)
        printf("Error [%d] at line %d in the .am file: %s\n", error_number, current_line_number, ERRORS[error_number].error_message);
    else
        /* stage == INTERNAL_ERROR_STAGE */
        printf("Error [%d]: %s\n", error_number, ERRORS[error_number].error_message);
}

void* safe_realloc(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    if (ptr == NULL)
    {/* Memory allocation failed */
        print_error(ERROR_1,INTERNAL_ERROR_STAGE);
    }
    return ptr;
}

void* safe_malloc(size_t size)
{
    void* ptr = malloc(size);
    if (ptr == NULL)
    {/* Memory allocation failed */
        print_error(ERROR_1,INTERNAL_ERROR_STAGE);
    }
    return ptr;
}

FILE* safe_fopen(char *filename, char *mode)
{
    FILE *file = fopen(filename, mode);
    if (file == NULL)
    {/* File opening failed */
        print_error(ERROR_2,INTERNAL_ERROR_STAGE);
    }
    return file;
}