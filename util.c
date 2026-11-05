/**
 * @file util.c
 * @brief Utility functions
 * @author Cameron Hannay
 */

#include "include.h"

uint32_t bin_to_num(char *binary)
{
	// Initialize the number to 0
	uint32_t num = 0;

	// Loop through each character in the binary string
	while (*binary != '\0') {
		num <<= 1;

		// If the character is '1', set the least significant bit of num to 1
		if (*binary == '1')
			num |= 1;
		binary++;
	}

	// Return the decimal equivalent of the binary string
	return num;
}


const uint32_t ARG_AUTO 	= 1;
const uint32_t ARG_REVERSE	= 2;

const uint32_t LINE_BUFF_SIZE = 4096;

// TODO: Add in documentation/comments since Old Jim forgot

uint32_t command_line_args(int argc, char *argv[], char** in_filename, char** out_filename)
{
	// Initialize the result variable to 0
	uint32_t result = 0;

	// Loop through each command line argument starting from index 1
	for (int i = 1; i < argc; i++) {

		// Check if the argument is "-a" or "-r" and set the corresponding bit in the result variable
		if (!strcmp(argv[i], "-a"))
			result = result | ARG_AUTO;

		// Check if the argument is "-r" and set the corresponding bit in the result variable
		else if (!strcmp(argv[i], "-r"))
			result = result | ARG_REVERSE;

		// If the argument is not "-a" or "-r", check if the input filename is NULL and set it to the current argument	
		else if (*in_filename == NULL)
			*in_filename = argv[i];

		// If the input filename is not NULL, check if the output filename is NULL and set it to the current argument
		else if (*out_filename == NULL)
			*out_filename = argv[i];
	}

	// Return the result variable containing the flags for the command line arguments
	return result;
}

void get_next_input(char* line, enum InteractiveState* int_state, uint32_t flags, char* in_filename, FILE** file)
{
	// Determine if the auto mode and reverse mode flags are set
	uint32_t auto_mode = (flags & ARG_AUTO) > 0;
	uint32_t reverse_mode = (flags & ARG_REVERSE) > 0;
	
	// Declare a pointer to a character string
	char* str;

	// If auto mode is enabled and an input filename is provided, read the next line from the input file
	if (auto_mode && in_filename != NULL) {

		// If the file pointer is NULL, open the input file for reading
		if (*file == NULL)
			*file = fopen(in_filename, "r");
		str = fgets(line, LINE_BUFF_SIZE, *file);

		// If the end of the file is reached, close the file and exit the program
		if (str == NULL && feof(*file)) {
			if (*file != NULL)
				fclose(*file);
			exit(0);
		}

		// If an error occurs while reading the file, print an error message, close the file, and exit the program
		else if (str == NULL && ferror(*file)) {
			printf("Invalid line");
			if (*file != NULL)
				fclose(*file);
			exit(1);
		}

		// If the line read from the file is empty, recursively call the function to read the next line
		else if (*line == '\n') {
			get_next_input(line, int_state, flags, in_filename, file);
		}

		// Set the newline character at the end of the line to null terminator and return
		line[strcspn(line, "\n")] = 0;
		return;
	}

	// If auto mode is enabled and no input filename is provided, read the next line from standard input
	else if (auto_mode && in_filename == NULL) {
		str = fgets(line, LINE_BUFF_SIZE, stdin);

		// If the end of the file is reached, close standard input and exit the program
		if (str == NULL && feof(stdin)) {
			if (stdin != NULL)
				fclose(stdin);
			exit(0);
		}

		// If an error occurs while reading from standard input, print an error message, close standard input, and exit the program
		else if (str == NULL && ferror(stdin)) {
			printf("Invalid line");
			if (stdin != NULL)
				fclose(stdin);
			exit(1);
		}

		// If the line read from standard input is empty, recursively call the function to read the next line
		else if (*line == '\n') {
			get_next_input(line, int_state, flags, in_filename, file);
		}

		// If the line read from standard input is invalid, print an error message, close the input file if it is open, and exit the program
		else if (strchr(line, '\n') == NULL || str == NULL) {
			printf("Invalid line");
			if (*file != NULL)
				fclose(*file);
			exit(1);
		}

		// Set the newline character at the end of the line to null terminator and return
		line[strcspn(line, "\n")] = 0;
		return;
	}

	// If auto mode is not enabled, enter an interactive mode to get user input
	else if (!auto_mode) {
		while (1) {

			// Declare a character array to store the user's choice
			char choice[10];

			// Use a switch statement to determine the current interactive state and prompt the user for input accordingly
			switch (*int_state) {

				// If the current state is ROOT, prompt the user to select an option from the main menu
				case ROOT:
					printf("\nPlease enter an option:\n\t(1) Assembly to Machine Code\n\t(2) Machine Code to Assembly\n\t(3) Quit\n\t(4) Corrupted Code Inspector\n");
					printf("\n> ");
					choice[0] = 0;

					// Read the user's choice from standard input and check for errors
					str = fgets(choice, sizeof(choice), stdin);
					if (strchr(choice, '\n') == NULL || str == NULL) {
						printf("Invalid input");
						*int_state = ROOT;
						continue;
					}

					// Set the newline character at the end of the choice to null terminator
					choice[strcspn(choice, "\n")] = 0;

					// Check the user's choice and update the interactive state accordingly
					if (!strcmp(choice, "1")) {
						*int_state = ASM_TO_MACH;
					}
					else if (!strcmp(choice, "2")) {
						*int_state = MACH_TO_ASM;
					}
					else if (!strcmp(choice, "3")) {
						if (*file != NULL)
							fclose(*file);
						exit(0);
					}
					else if (!strcmp(choice, "4")) {
						*int_state = DEBUG;
					}

					// If the user's choice is invalid, print an error message and reset the interactive state to ROOT
					else {
						printf("Invalid input");
						*int_state = ROOT;
					}
					break;
				
				// If the current state is MACH_TO_ASM, prompt the user to select an option for converting machine code to assembly
				case MACH_TO_ASM:
					printf("\nPlease select an option:\n\t(1) Hexadecimal to Assembly\n\t(2) Binary to Assembly\n\t[3] Main Menu\n");
					printf("\n> ");
					choice[0] = 0;

					// Read the user's choice from standard input and check for errors
					str = fgets(choice, sizeof(choice), stdin);
					if (strchr(choice, '\n') == NULL || str == NULL) {
						printf("Invalid input");
						*int_state = MACH_TO_ASM;
						continue;
					}

					// Set the newline character at the end of the choice to null terminator
					choice[strcspn(choice, "\n")] = 0;

					// Check the user's choice and update the interactive state accordingly
					if (!strcmp(choice, "1")) {
						*int_state = HEX_TO_ASM;
					}
					else if (!strcmp(choice, "2")) {
						*int_state = BIN_TO_ASM;
					}
					else if (!strcmp(choice, "3")) {
						*int_state = ROOT;
					}
					else {
						printf("Invalid input");
						*int_state = MACH_TO_ASM;
					}
					break;
				
				// If the current state is ASM_TO_MACH, prompt the user to enter a line of assembly code for conversion to machine code	
				case ASM_TO_MACH:
					printf("\n Enter a line of assembly:\n> ");
					str = fgets(line, LINE_BUFF_SIZE, stdin);
					if (strchr(line, '\n') == NULL || str == NULL) {
						printf("Invalid input");
						continue;
					}

					// Set the newline character at the end of the line to null terminator
					line[strcspn(line, "\n")] = 0;

					// If the line is empty, reset the interactive state to ROOT
					if (line[0] == 0) { 
						*int_state = ROOT;
					}
					return;
				
				// If the current state is HEX_TO_ASM, prompt the user to enter a line of hexadecimal code for conversion to assembly	
				case HEX_TO_ASM:
					printf("\n Enter Hex:\n> ");
					str = fgets(line, LINE_BUFF_SIZE, stdin);
					if (strchr(line, '\n') == NULL || str == NULL) {
						printf("Invalid input");
						continue;
					}

					// Set the newline character at the end of the line to null terminator
					line[strcspn(line, "\n")] = 0;

					// If the line is empty, reset the interactive state to MACH_TO_ASM
					if (line[0] == 0) { 
						*int_state = MACH_TO_ASM;
					}
					return;
				
				// If the current state is BIN_TO_ASM, prompt the user to enter a line of binary code for conversion to assembly	
				case BIN_TO_ASM:
					printf("\n Enter Binary:\n> ");
					str = fgets(line, LINE_BUFF_SIZE, stdin);

					// Check for errors in the input and print an error message if necessary
					if (strchr(line, '\n') == NULL || str == NULL) {
						printf("Invalid input");
						continue;
					}

					// Set the newline character at the end of the line to null terminator
					line[strcspn(line, "\n")] = 0;

					// If the line is empty, reset the interactive state to MACH_TO_ASM
					if (line[0] == 0) { 
						*int_state = MACH_TO_ASM;
					}
					return;

				// If the current state is DEBUG, prompt the user to enter a line of broken binary code for inspection	
				case DEBUG:
					printf("\n Enter Broken Binary:\n> ");
					str = fgets(line, LINE_BUFF_SIZE, stdin);

					// Check for errors in the input and print an error message if necessary
					if (strchr(line, '\n') == NULL || str == NULL) {
						printf("Invalid input");
						continue;
					}

					// Set the newline character at the end of the line to null terminator
					line[strcspn(line, "\n")] = 0;

					// If the line is empty, reset the interactive state to ROOT
					if (line[0] == 0) { 
						*int_state = ROOT;
					}
					return;

			}
		}
	}
	else {
		return;
	}
}
