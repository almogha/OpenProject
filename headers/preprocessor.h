/* Name: Almog Hakak, ID: 211825229 */

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "main.h"
#include "errors.h"
#include "helpers.h"


/**
 * @brief Substitutes macro references in the given file with their defined values.
 *
 * This function iterates through the specified file, identifying and skipping macro definitions.
 * For all other lines, it performs a substitution of macro references with their corresponding definitions from the macro list.
 * The updated lines are written to a temporary file, which then replaces the original file.
 * @param input_file The path to the file that will be processed.
 * @param head The head of the linked list that holds the macro definitions and their replacements.
 */
void replaceMacroReferences(char *input_file, MacroNode *head);

/**
 * @brief Performs macro substitution on the specified file.
 *
 * This function parses the input file, substitutes macro invocations with their respective
 * definitions, and generates a new .am file containing the modified content.
 * @param file_name The name of the file to be processed.
 * @return Returns 1 upon successful macro substitution, or 0 if an error occurs.
 */
int processMacros(char *file_name);

/**
 * @brief Substitutes a placeholder with its defined content in a given string.
 *
 * This function looks for a specific placeholder within the input string and substitutes it with the associated content.
 * @param str The string that may contain the placeholder.
 * @param macr Pointer to the MacroNode representing the placeholder and its content.
 * @return A new string with the placeholder substituted, or NULL if the placeholder was not found.
 */
char *substitutePlaceholder(char *str, MacroNode *macr);

/**
 * @brief Extracts and stores macro content from a file.
 *
 * This function reads macro content from a file starting at the specified position,
 * updates the line count for each line read, and determines the total length of the macro.
 * @param fp Pointer to the file to read from.
 * @param pos Position in the file where reading begins.
 * @param line_count Pointer to the variable tracking the number of lines read.
 * @return Pointer to the allocated memory containing the macro content.
 */
char *extractMacroData(FILE *fp, fpos_t *pos, int *line_count);

/**
 * @brief Analyzes and processes a macro definition.
 *
 * This function examines the provided string to determine if it conforms to a proper macro definition format.
 * It extracts the macro identifier and allocates memory for it accordingly.
 * @param str The string that potentially contains a macro definition.
 * @param name Pointer to hold the extracted macro identifier.
 * @param line_count The number of the line being processed.
 * @param file_name The name of the file from which the string was read.
 * @return 1 if the macro definition is correctly formatted, 0 otherwise.
 */
int analyzeMacroDefinition(char *str, char **name, int line_count, char *file_name);

/**
 * @brief Reserves memory and handles allocation errors.
 *
 * This function reserves a block of memory of the given size using malloc.
 * If the memory allocation fails, it outputs an error message and terminates the program.
 * @param size The amount of memory to reserve, in bytes.
 * @return A pointer to the allocated memory block.
 */
char *allocateMemory(size_t size);

/**
 * @brief Incorporates macro definitions from a specified file into a list.
 *
 * This function processes a file to extract macro definitions and appends them to the given list.
 * It ensures that each macro definition is properly formatted and stores the associated data.
 * @param file_name The path to the file to be processed.
 * @param head A pointer to the start of the list.
 * @return 1 if the operation was successful, 0 otherwise.
 */
int importMacros(char *file_name, MacroNode **head);

#endif
