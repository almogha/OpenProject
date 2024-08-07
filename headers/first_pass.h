
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
 * @description This function attempts to insert a new label into an existing label array, provided the label meets the necessary criteria and is not a duplicate.
 *
 * @param label The label data intended for insertion.
 * @param line The associated line data which includes the label.
 * @return A pointer to the newly added label in the label array, or NULL if the label is invalid or already present.
 */
labelInfo *insertLabelIfValid(labelInfo label, lineInfo *line);

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
 * @brief Processes .string directive and appends its content to the data buffer.
 *
 * @param line A structure holding the details of the .string directive.
 * @param IC Pointer to the instructions counter.
 * @param DC Pointer to the data counter.
 */

void handleStringDirective(lineInfo *line, int *IC, int *DC);

/**
 * @brief Analyzes a .extern directive and registers the label as an external reference.
 * @param lineDetails The line data that includes the .extern directive.
 */
void handleExternalDirective(lineInfo *line);

/**
 * @brief Processes an .entry command and appends the associated label to the list of entry labels.
 * @param line Contains the details of the line with the .entry.
 */
void handleEntryCommand(lineInfo *line);

/**
 * @brief Processes a given directive and invokes the corresponding handler function.
 *
 * @param line The line that contains the directive.
 * @param IC Pointer to the instructions counter.
 * @param DC Pointer to the data counter.
 */
void handleDirective(lineInfo *line, int *IC, int *DC);

/**
 * @brief Parses and validates operand information.
 *
 * @param operand The operand information to be parsed and validated.
 * @param lineNum The line number (used for error reporting).
 */
void parseOpInfo(operandInfo *operand, int lineNum);

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
 * @brief Analyzes and verifies command arguments.
 *
 * This function analyzes and verifies the arguments associated with a command. It ensures that the
 * arguments are valid and adjusts the instruction counter (IC) as needed.
 * @param line The information line that includes the command and its arguments.
 * @param IC Pointer to the instructions counter.
 * @param DC Pointer to the data counter.
 */
void analyzeCommandArguments(lineInfo *line, int *IC, int *DC);

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
 * @brief Reads a line from a file.
 *
 * This function reads a line from a file, ensuring the line does not exceed the maximum length.
 * If the line is too long, it reads until the end of the line or file.
 * @param file The file pointer to read from.
 * @param line_data The buffer to store the read line.
 * @param maxLength The maximum length of the line to read.
 * @return Returns TRUE if a line is successfully read, otherwise FALSE.
 */
boolean readLine(FILE *file, char *line_data, size_t maxLength);

/**
 * @brief Performs the first pass of the assembler.
 *
 * This function performs the first pass of the assembler, reading and parsing each line of the source file.
 * It updates the instruction counter (IC), data counter (DC), and line information array (linesArr).
 * @param file The file pointer to the source file.
 * @param linesArr The array to store parsed line information.
 * @param linesFound A pointer to the number of lines found.
 * @param IC Pointer to the instructions counter.
 * @param DC Pointer to the data counter.
 * @return Returns the number of errors found during the first pass.
 */
int firstPass(FILE *file, lineInfo *linesArr, int *linesFound, int *IC, int *DC);

#endif