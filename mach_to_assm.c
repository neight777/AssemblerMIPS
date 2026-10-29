/**
 * @file mach_to_assm.c
 * @brief File containing the logic to convert a machine instruction to assembly.
 * instruction
 * @author Cameron Hannay
 */

#include "include.h"

/**
 * @brief Finds the index in instructon_definitions of the instruction corresponding to the
 * funct_code if it is R_TYPE or the op_code otherwise.
 * @param op_code The provided op_code as an integer.
 * @param funct_code The provided funct_code as an integer (nonsense if op_code isn't for
 * R_TYPE instruction).
 * @param error Pointer to string to hold error message if an error occurs.
 * @return The index of the instruction in instruction_definitions if error is not set.
 * Undefined if error is set.
 */
static size_t find_instruction_by_code(uint32_t op_code, uint32_t funct_code, char **error)
{
/**
	this is meant to iterate throught the instruction code given, and checking if this the input itself is found with the list of possible opcodes, the next conditional
	is meant to check if it belongs to a register function type. If so it will perform that check and grab that specific function that the op code belongs to. 
	If not, it will simply return the iterator and pass through the error pointer, telling us the instruction code does not exist.
*/ 
	size_t i;
	for (i = 0; i < NUM_INSTRUCTIONS; i++) // Going through each instruction definition to find a matching opcode
	{
		if (instruction_definitions[i].op_code == op_code)// Then check if that instruction matches the input
		{
			if (op_code == R_TYPE) // If the opcode represents an R_TYPE instruction
			{
				if (instruction_definitions[i].funct_code == funct_code) // Now checking if the function code also mathces the R_TYPE instruction code
				{
					return i; // returning index if both match.
				}
			}
			else
			{
				return i; // This is for other types of instructions and will still return index as it only needs op code to match
			}

	}
	}
	*error = "undefined"; // Error Message
	return 0;
}
// TODO: Add in documentation/comments since Old Jim forgot

struct assm_parse_result convert_to_assembly(uint32_t inst, char **error)
{
	*error = NULL;

	uint32_t rd = (inst >> 11) & MAX_5_BIT;
	uint32_t rs = (inst >> 21) & MAX_5_BIT;
	uint32_t rt = (inst >> 16) & MAX_5_BIT;
	uint32_t sa = (inst >> 6) & MAX_5_BIT;
	uint32_t imm = inst & MAX_16_BIT;
	uint32_t tar = inst & MAX_26_BIT;

	uint32_t op_code = (inst >> 26) & MAX_6_BIT;
	uint32_t funct_code = inst & MAX_6_BIT;

	struct assm_parse_result assembly;

	size_t ins = find_instruction_by_code(op_code, funct_code, error);
	if (*error != NULL)
		return assembly;

	assembly.op_name = instruction_definitions[ins].op_name;
	for (size_t i = 0; i < 4; i++) {
		switch (instruction_definitions[ins].parts[i]) {
		case RD:
			assembly.types[i] = REGISTER;
			assembly.vals[i] = rd;
			break;
		case RS:
			assembly.types[i] = REGISTER;
			assembly.vals[i] = rs;
			break;
		case RT:
			assembly.types[i] = REGISTER;
			assembly.vals[i] = rt;
			break;
		case SA:
			assembly.types[i] = REGISTER;
			assembly.vals[i] = sa;
			break;
		case IMM:
			assembly.types[i] = IMMEDIATE;
			assembly.vals[i] = imm;
			break;
		case TAR:
			assembly.types[i] = TARGET;
			assembly.vals[i] = tar;
			break;
		case EMPTY:
			assembly.types[i] = NONE;
			assembly.vals[i] = UNDEFINED;
			break;
		}
	}

	return assembly;
}

static size_t find_instruction_by_code(uint32_t op_code, uint32_t funct_code, char **error)
{
	size_t ins;
	bool found = false;

	for (ins = 0; ins < sizeof(instruction_definitions)
			/ sizeof(struct instruction_definition); ins++) {
		if (bin_to_num(instruction_definitions[ins].op_code) != op_code)
			continue;

		if (instruction_definitions[ins].type == R_TYPE) {
			if (bin_to_num(instruction_definitions[ins].funct_code) != funct_code)
				continue;
		}

		found = true;
		break;
	}

	if (found == false)
		*error = "No instruction of given op code or funct code.";

	return ins;
}
