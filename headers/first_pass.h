
/* Name: Almog Hakak, ID: 211825229
*
* First Pass Functions
*/

#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "main.h"
#include "errors.h"
#include "helpers.h"

 /**
 * @brief Conducts the initial phase of the assembler process.
 *
 * This function executes the preliminary phase of the assembler, where it reads and interprets each line from the source file.
 * It updates the instruction counter (IC), data counter (DC), and the array containing line information (linesArr).
 * @param file Pointer to the source file.
 * @param linesArr Array for storing parsed line details.
 * @param linesFound Pointer to the count of lines processed.
 * @param IC Pointer to the instructions counter.
 * @param DC Pointer to the data counter.
 * @return The count of errors encountered during the initial phase.
 */
int initialAssemblerPass(FILE *file, lineInfo *linesArr, int *linesFound, int *IC, int *DC);

/**
 * @brief Fetches a single line from a specified file.
 *
 * This function extracts a line from the given file, making sure the line length does not surpass the defined limit.
 * If the line extends beyond the allowed length, it will read only up to the end of the line or file.
 * @param file The file stream to read from.
 * @param line_data The character array to hold the extracted line.
 * @param maxLength The maximum permissible length of the line to be read.
 * @return Returns TRUE if a line is successfully fetched, otherwise FALSE.
 */
boolean fetchLine(FILE *file, char *line_data, size_t maxLength);

/**
 * @description This function attempts to insert a new label into an existing label array, provided the label meets the necessary criteria and is not a duplicate.
 *
 * @param label The label data intended for insertion.
 * @param line The associated line data which includes the label.
 * @return A pointer to the newly added label in the label array, or NULL if the label is invalid or already present.
 */
labelInfo *insertLabelIfValid(labelInfo label, lineInfo *line);

/**
 * @brief Processes .string directive and appends its content to the data buffer.
 *
 * @param line A structure holding the details of the .string directive.
 * @param IC Pointer to the instructions counter.
 * @param DC Pointer to the data counter.
 */

void handleStringDirective(lineInfo *line, int *IC, int *DC);

/**
 * @brief Inserts a value into the data array if space permits.
 *
 * @param num The value to insert.
 * @param IC Pointer to the instructions counter.
 * @param DC Pointer to the data counter.
 * @param lineNum The current line number used for error messages.
 * @return TRUE if the value was successfully inserted, FALSE if there is insufficient space.
 */
boolean insertValueIntoDataArray(int num, int *IC, int *DC, int lineNum);

/**
 * @brief Parses and validates operand information.
 *
 * @param operand The operand information to be parsed and validated.
 * @param lineNum The line number (used for error reporting).
 */
void parseOpInfo(operandInfo *operand, int lineNum);

/**
 * @brief Searches for and handles a label within a given assembly code line.
 *
 * @param line The structure holding details about the line, which may include a label.
 * @param IC The current value of the instruction counter.
 * @return A pointer to the character immediately following the label in the line, or NULL if a label is not present.
 */
char *locateAndProcessLabel(lineInfo *line, int IC);

/**
 * @brief Processes .data incorporates its values into the data list.
 *
 * @param line The structure containing the details of the .data command.
 * @param IC Pointer to the instructions counter.
 * @param DC Pointer to the data counter.
 */
void handleDataCommand(lineInfo *line, int *IC, int *DC);

/**
 * @brief Analyzes a .extern directive and registers the label as an external reference.
 * @param lineDetails The line data that includes the .extern directive.
 */
void handleExternalDirective(lineInfo *line);

/**
 * @brief Processes a given directive and invokes the corresponding handler function.
 *
 * @param line The line that contains the directive.
 * @param IC Pointer to the instructions counter.
 * @param DC Pointer to the data counter.
 */
void handleDirective(lineInfo *line, int *IC, int *DC);

/**
 * @brief Parses and validates the operands for a command.
 *
 * This function parses and validates the operands for a given command. It checks if the operands
 * are legal and updates the instruction counter (IC) accordingly.
 * @param line The line information containing the command and operands.
 * @param IC Pointer to the instructions counter.
 * @param DC Pointer to the data counter.
 */
void parseCmdOperands(lineInfo *line, int *IC, int *DC);

/**
 * @brief Processes an .entry command and appends the associated label to the list of entry labels.
 * @param line Contains the details of the line with the .entry.
 */
void handleEntryCommand(lineInfo *line);

/**
 * @brief Creates a duplicate of the given string by allocating new memory.
 *
 * This function reserves memory for a copy of the input string and transfers its content
 * into the newly allocated memory space.
 * @param str The string to be duplicated.
 * @return A pointer to the newly allocated memory containing the copied string.
 */
char *duplicateString(const char *str);

/**
 * @brief Analyzes and processes a single line of assembly input.
 *
 * This function analyzes a given line of assembly code, distinguishing between labels, directives, and operations,
 * and adjusts the instruction counter (IC) and data counter (DC) based on the analysis.
 * @param line The line information structure.
 * @param lineStr The input line string.
 * @param lineNum The line number.
 * @param IC Pointer to the instructions counter.
 * @param DC Pointer to the data counter.
 */
void analyzeAssemblyLine(lineInfo *line, char *lineStr, int lineNum, int *IC, int *DC);

 /**
 * @brief Analyzes and verifies command arguments.
 *
 * This function analyzes and verifies the arguments associated with a command. It ensures that the
 * arguments are valid and adjusts the instruction counter (IC) as needed.
 * @param line The information line that includes the command and its arguments.
 * @param IC Pointer to the instructions counter.
 * @param DC Pointer to the data counter.
 */
void analyzeCommandArguments(lineInfo *line, int *IC, int *DC);

#endif
