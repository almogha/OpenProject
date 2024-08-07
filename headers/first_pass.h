
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
 * @param IC A pointer to the instruction counter.
 * @param DC A pointer to the data counter.
 * @param lineNum The current line number used for error messages.
 * @return TRUE if the value was successfully inserted, FALSE if there is insufficient space.
 */
boolean insertValueIntoDataArray(int num, int *IC, int *DC, int lineNum);

/**
 * @brief Adds a string to the data array by converting each character to its ASCII value.
 *
 * @param str The string to be added.
 * @param IC A pointer to the instruction counter.
 * @param DC A pointer to the data counter.
 * @param lineNum The line number (used for error reporting).
 * @return TRUE if the string was added successfully, FALSE if there is not enough space.
 */
boolean addStringToData(char *str, int *IC, int *DC, int lineNum);

/**
 * @brief Finds and processes a label in a line of assembly code.
 *
 * @param line The line information containing the potential label.
 * @param IC The instruction counter.
 * @return A pointer to the next character after the label in the line, or NULL if no label is found.
 */
char *findLabel(lineInfo *line, int IC);

/**
 * @brief Removes the last added label from the label array and prints a warning.
 *
 * @param lineNum The line number where the label was found (used for warning message).
 */
void removeLastLabel(int lineNum);

/**
 * @brief Parses a .data directive and adds its values to the data array.
 *
 * @param line The line information containing the .data directive.
 * @param IC A pointer to the instruction counter.
 * @param DC A pointer to the data counter.
 */
void parseDataDirc(lineInfo *line, int *IC, int *DC);

/**
 * @brief Parses a .string directive and adds its values to the data array.
 *
 * @param line The line information containing the .string directive.
 * @param IC A pointer to the instruction counter.
 * @param DC A pointer to the data counter.
 */
void parseStringDirc(lineInfo *line, int *IC, int *DC);

/**
 * @brief Parses an .extern directive and adds the label as an external label.
 * @param line The line information containing the .extern directive.
 */
void parseExternDirc(lineInfo *line);

/**
 * @brief Parses an .entry directive and adds the label to the entry labels list.
 * @param line The line information containing the .entry directive.
 */
void parseEntryDirc(lineInfo *line);

/**
 * @brief Parses a directive and calls the appropriate parsing function.
 *
 * @param line The line information containing the directive.
 * @param IC A pointer to the instruction counter.
 * @param DC A pointer to the data counter.
 */
void parseDirective(lineInfo *line, int *IC, int *DC);

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
 * @param IC A pointer to the instruction counter.
 * @param DC A pointer to the data counter.
 */
void parseCmdOperands(lineInfo *line, int *IC, int *DC);

/**
 * @brief Parses a command and its operands.
 *
 * This function identifies and parses a command in a line, extracts and validates its operands,
 * and updates the instruction counter (IC) accordingly.
 * @param line The line information containing the command and operands.
 * @param IC A pointer to the instruction counter.
 * @param DC A pointer to the data counter.
 */
void parseCommand(lineInfo *line, int *IC, int *DC);

/**
 * @brief Allocates memory for a string and copies its content.
 *
 * This function allocates memory for a new string and copies the content of the input string
 * to the newly allocated memory.
 * @param str The input string to be copied.
 * @return A pointer to the newly allocated and copied string.
 */
char *allocString(const char *str);

/**
 * @brief Parses a line of assembly code.
 *
 * This function parses a line of assembly code, identifies labels, directives, and commands,
 * and updates the instruction counter (IC) and data counter (DC) accordingly.
 * @param line The line information structure to be filled.
 * @param lineStr The input line string to be parsed.
 * @param lineNum The line number (used for error reporting).
 * @param IC A pointer to the instruction counter.
 * @param DC A pointer to the data counter.
 */
void parseLine(lineInfo *line, char *lineStr, int lineNum, int *IC, int *DC);

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
 * @param IC A pointer to the instruction counter.
 * @param DC A pointer to the data counter.
 * @return Returns the number of errors found during the first pass.
 */
int firstPass(FILE *file, lineInfo *linesArr, int *linesFound, int *IC, int *DC);

#endif