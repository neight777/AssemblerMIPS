/**
 * @file gen_assm.c
 * @brief Code to generate assembly from a parse result
 * @author Cameron Hannay

 */

#include "include.h"

// TODO: Add in documentation/comments since Old Jim forgot

void generate_assembly(char *buf, size_t size, struct assm_parse_result parsed, char **error)
{
	// Initialize the error pointer to NULL
	*error = NULL;

	// Initialize variables for array storage of arguments
	char args[4][8]; 
	size_t arg_count;
	union signedness_switch_trick signedness;

	// Loop through the parsed arguments and store them in the arguments array
	for (arg_count = 0; arg_count < 4 && parsed.types[arg_count] != NONE; arg_count++) {
		switch (parsed.types[arg_count]) {

		// Handle each type of argument (REGISTER, IMMEDIATE, TARGET, NONE)
		case REGISTER:
		    // Copy the register name from the registers array into the arguments array
			strcpy(args[arg_count], registers[parsed.vals[arg_count]]);
			break;
		case IMMEDIATE:
			// Convert the immediate value to a signed 16-bit integer and store it in the arguments array
			signedness.u = (uint16_t) parsed.vals[arg_count];
			sprintf(args[arg_count], "#%d", signedness.i);
			break;
		case TARGET:
		    // Handle the target argument type, which is currently unsupported
			*error = "Generating assembly with targets is unsupported.";
			return;
		case NONE:
		    // Break the loop once the end of the argument is reached
			break;
		}
	}

	// Format the assembly instruction using the appropriate format string based on the number of arguments
	char *formats[5] = {
		"%s",
		"%s %s",
		"%s %s, %s",
		"%s %s, %s, %s",
		"%s %s, %s, %s, %s"
	};
	// Generate the assembly instruction string using the selected format
	int needed = snprintf(buf, size, formats[arg_count], parsed.op_name,
			args[0], args[1], args[2], args[3]);
	// Check if the generated assembly instruction fits within the buffer size
	if (needed >= size)
		*error = "Buffer too small for assembly instruction.";
}
