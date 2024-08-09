
/* Name: Almog Hakak, ID: 211825229 
*
* Second Pass Functions
*/

#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "main.h"
#include "errors.h"
#include "helpers.h"

/**
 * @brief Updates the addresses of data labels by adding the instruction counter (IC) value.
 *
 * This function iterates through the global label array and updates the address of each data label
 * by adding the value of the instruction counter (IC). This is necessary for adjusting data labels
 * after the instruction section has been processed.
 * @param IC The instruction counter value to be added to the data labels' addresses.
 */
void updateDataLabelsAddress(int IC);

/**
 * @brief Counts the number of illegal entry labels and reports errors.
 *
 * This function iterates through the global entry lines array and checks each entry label.
 * It reports errors if an entry label is found to be an external label or if the label does not exist.
 * The function returns the total number of errors found.
 * @return The total number of errors found in the entry labels.
 */
int countIllegalEntries(void);

/**
 * @brief Updates the address of a label operand.
 *
 * This function checks if the operand is of type OP_LABEL. If so, it retrieves the label information
 * and updates the operand's value with the label's address. If the label does not exist, an error is reported.
 * @param op A pointer to the operand information to be updated.
 * @param lineNum The line number where the operand is located (used for error reporting).
 * @return Returns TRUE if the label exists and the address was updated, otherwise FALSE.
 */
boolean updateLabelOpAddress(operandInfo *op, int lineNum);

/**
 * @brief Extracts the numerical value from a memory word.
 *
 * This function applies a bitmask to the value in a memory word to extract the relevant bits.
 * It combines the value bits and the ARE (Addressing, Relocatable, External) bits into a single integer.
 * @param memory The memory word to extract the value from.
 * @return The extracted numerical value from the memory word.
 */
int getNumFromMemoryWord(memoryWord memory);

/**
 * @brief Retrieves the type ID of an operand.
 *
 * This function checks if the operand type is valid and returns its type ID.
 * If the operand type is invalid, it returns 0.
 * @param op The operand information.
 * @return The type ID of the operand if valid, otherwise 0.
 */
int getOpTypeId(operandInfo op);

/**
 * @brief Creates a memory word for a command line.
 *
 * This function creates and initializes a memory word for a given command line.
 * It sets the ARE type to ARE_ABS and encodes the destination and source operand types and the opcode.
 *
 * @param line The line information containing the command and operands.
 * @return The created memory word for the command.
 */
memoryWord getCmdMemoryWord(lineInfo line);

/**
 * @brief Creates a memory word for an operand.
 *
 * This function creates and initializes a memory word for a given operand.
 * It sets the ARE type and encodes the operand value based on its type (register, indirect register, label, or number).
 *
 * @param op The operand information.
 * @param isDest A flag indicating if the operand is a destination operand.
 * @return The created memory word for the operand.
 */
memoryWord getOpMemoryWord(operandInfo op, boolean isDest);

/**
 * @brief Adds a memory word to the memory array.
 *
 * This function adds a memory word to the memory array at the current memory counter position.
 * It increments the memory counter after adding the word.
 *
 * @param memoryArr The memory array to add the word to.
 * @param memoryCounter A pointer to the current memory counter.
 * @param memory The memory word to add.
 */
void addWordToMemory(int *memoryArr, int *memoryCounter, memoryWord memory);

/**
 * @brief Adds a line to the memory array, updating operand addresses as needed.
 *
 * This function adds the memory words for a line to the memory array, updating the addresses of label operands.
 * It handles different operand types (register, indirect register, label, number) and adds the appropriate memory words.
 * @param memoryArr The memory array to add the line to.
 * @param memoryCounter A pointer to the current memory counter.
 * @param line A pointer to the line information.
 * @return Returns TRUE if no error was found, otherwise FALSE.
 */
boolean addLineToMemory(int *memoryArr, int *memoryCounter, lineInfo *line);

/**
 * @brief Adds data to the memory array.
 *
 * This function adds data words to the memory array, applying a bitmask to each data word.
 * It increments the memory counter for each data word added.
 * @param memoryArr The memory array to add the data to.
 * @param memoryCounter A pointer to the current memory counter.
 * @param DC The data counter indicating the number of data words.
 */
void addDataToMemory(int *memoryArr, int *memoryCounter, int DC);

/**
 * @brief Performs the second pass of reading and processing an assembly language file.
 * 
 * This function updates the addresses of data labels, checks for illegal entries,
 * and adds the parsed lines and data to the memory array. It returns the total
 * number of errors encountered during this process.
 * @param memoryArr An array of integers representing the memory of the assembly program.
 * @param linesArr An array of lineInfo structures holding information about each parsed line.
 * @param lineNum The number of lines in the linesArr array.
 * @param IC The instruction counter value at the end of the first pass.
 * @param DC The data counter value at the end of the first pass.
 * @return The total number of errors encountered during the second pass.
 */
int secondPass(int *memoryArr, lineInfo *linesArr, int lineNum, int IC, int DC);

#endif
