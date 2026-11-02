/**
 * @file parser.c
 * @brief Parses user input of assembly and determines what kind of instruction it is to be converted into
 * @author Cameron Hannay
 */

#include "include.h"

/**
 * @brief Takes the string and turns each character to uppercase
 * @param operand String to convert to uppercase
 */
static void uppercase(char *operand);

/**
 * @brief Takes the string compares it with each register type to find a match
 * @param operand String to lookup
 */
static uint32_t reg_lookup(char *str, char** error);

/**
 * @brief Removes whitespace from a string
 * @param str String to remove whitespace from
 */
static void remove_space(char* str);

/**
 * @brief Removes pound symbol from string
 * @param str String to remove pound symbol from
 */
static void remove_pound(char* str);

// TODO: Add in documentation/comments since Old Jim forgot

uint32_t parse_assembly(char *line, char **error)
{
    // Initialize the result structure to store the parsed assembly instruction and other variables for the function
    // This includes setting the error pointer to NULL and allocating memory for the opcode name
    // The opcode name will be used to store the parsed instruction name from the input line
    struct assm_parse_result result;
    *error = NULL;
    size_t length;
    char* opp_name;

    // Determine the length of the opcode name by finding the first space character
    length = strcspn(line, " ");

    // Allocate memory for the opcode name based on the determined length
    opp_name = (char*)malloc(sizeof(char) * (length + 1));

    // Check if the memory allocation was successful
    if (opp_name == NULL)
    {
        *error = "Memory not allocated.";
        return UNDEFINED;
    }

    // Copy the opcode name from the input line into the allocated memory
    strncpy(opp_name, line, length);

    // Add null termination to the end of the opcode
    opp_name[length] = '\0';
    
    // Convert opcode name to uppercase for consistent comparisons
    uppercase(opp_name);

    // Store the opcode name in the result structure
    result.op_name = opp_name;

    // Move the input line pointer past the opcode name
    line += length;

    // Break the input line up using commas
    char* token = strtok(line, ",");
    size_t arg;

    // Loop through each token until it is NULL
    for (arg = 0; token != NULL; arg++)
    {
        // Remove any leading and trailing whitespace from the token
        remove_space(token);

        // Check if the token represents an immediate value
        if (token[0] == '#')
        {
            // Mark the argument type as immediate if it passes if statement
            result.types[arg] = IMMEDIATE;

            // Remove the leading pound from the immediate
            remove_pound(token);
            char* end_ptr;
        
        // Convert the immediate value to a signed 16-bit integer using the union trick
	    union signedness_switch_trick signedness;
            signedness.i = (int16_t) strtol(token, &end_ptr, 10);
            result.vals[arg] = signedness.u;

            // Check if the immediate was formatted properly
            if(end_ptr == token || *end_ptr != '\0')
            {
                // If the immediate was not formatted properly, set an error and clean up allocated memory
                *error = "Improperly formatted immediate.";
                free(opp_name);
                opp_name = NULL;
                return UNDEFINED;
            }
        }

        // Check if the token represents a register
        else if (token[0] == '$')
        {
            // Mark the argument type as register
            result.types[arg] = REGISTER;

            // Look up the register value
            result.vals[arg] = reg_lookup(token, error);

            // Check if the register lookup was successful
            if (*error != NULL) {
                free(opp_name);
                opp_name = NULL;
                return UNDEFINED;
            }
        }

        // Check if the token is neither a register nor an immediate value
        else
        {
            // If the token is neither a register nor an immediate value, send error message and clean up allocated memory
            *error = "Argument isn't register or immediate (targets not yet supported).";
            free(opp_name);
            opp_name = NULL;
            return UNDEFINED; 
        }
        token = strtok(NULL, ",");
    }

    // Fill in any remaining argument slots with NONE type and UNDEFINED values
    for (;arg < 4; arg++) {
        result.types[arg] = NONE;
        result.vals[arg] = UNDEFINED;
    }

    // Convert the parsed arguments to machine code
    uint32_t new_val = convert_to_machine_code(result, error);
    
    // Free the allocated memory for the operand name and return the new machine code
    free(opp_name);
    opp_name = NULL;

    return new_val;
}

static void uppercase(char *operand)
{   
    // Convert the operand string to uppercase
    size_t length = strlen(operand);
    for (size_t i = 0; i < length; i++)
    {
        operand[i] = toupper((unsigned char) operand[i]);
    }
}

static void remove_pound(char* str)
{
    // Initialize count for new string size and remove the pound sign from the immediate
    int count = 0;

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] != '#')
        {
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}

static void remove_space(char* str)
{
    // Initialize count for new string size and remove spaces from the string
    int count = 0;

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] != ' ')
        {
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}

static uint32_t reg_lookup(char *str, char** error)
{
    // Look up the register in the registers array and return its index if found
    for (uint32_t i = 0; i < sizeof(registers) / sizeof(char *); i++)
    {
        if (strcmp(registers[i],str) == 0)
        {
            return i;
        }
    }

    // If the register is not found, set an error message and return UNDEFINED
    *error = "Register not found.";
    return UNDEFINED;
}
