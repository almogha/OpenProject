
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
 * @brief Adjusts the addresses of all data labels in the global labels array.
 *
 * This function iterates through the global labels array and updates the
 * addresses of all data labels by adding the given instruction counter (IC)
 * value. It ensures that data labels have the correct memory address after
 * the code section is processed.
 *
 * @param IC The instruction counter value to add to the address of each data label.
 */
void adjustDataLabelAddresses(int IC);

 /**
 * @brief Validates entry labels in the global entry lines array.
 *
 * This function iterates over the global array of entry lines (`g_entryLinesArr`)
 * and checks if the associated labels are valid. It counts labels that are either 
 * external or non-existent. 
 * Any issues encountered are reported, and the total number of illegal entry labels is returned.
 *
 * @return int The number of illegal entry labels found.
 */
int validateEntryLabels(void);

 /**
 * @brief Updates the operand's value with the address of the associated label, if applicable.
 *
 * This function checks if the provided operand is of type `OP_LABEL`. If it is, the function 
 * attempts to retrieve the label's address and update the operand's value with it. If the 
 * label does not exist, an error is logged, and the function returns `FALSE`. Otherwise, the 
 * function returns `TRUE` after successfully updating the operand's value.
 * @param op A pointer to the operand information structure (`operandInfo`) containing details about the operand.
 * @param lineNum The line number in the source code where this function is being called, used for error reporting.
 * @return `TRUE` if the operand value is successfully updated or is not of type `OP_LABEL` `FALSE` if the label does not exist.
 */
boolean updateOperandLabelAddress(operandInfo *op, int lineNum);

 /**
 * @brief Extracts the numeric value from a memory word.
 *
 * This function applies a mask to a given memory word, shifts its bits,
 * and returns the processed numeric value.
 * @param memory A memoryWord struct containing the value bits and the 'are' field.
 * @return The processed numeric value from the memory word.
 */
int extractValueFromMemoryWord(memoryWord memory);

/**
 * Retrieves the ID of the operand type.
 *
 * @param op The operand information structure.
 * @return The operand type ID if valid; otherwise, returns 0.
 */
int retrieveOperandTypeId(operandInfo op);

/**
 * @brief Constructs a memoryWord representing the command and operand types from the given line information.
 * 
 * This function initializes a memoryWord with the appropriate command bits, 
 * setting the destination operand type, source operand type, and opcode based on the provided line information.
 * @param line The line information containing the command and operand details.
 * @return A memoryWord struct populated with the command bits.
 */
memoryWord constructCmdMemoryWord(lineInfo line);

/**
 * Converts an operand into a memory word structure based on the operand type and destination status.
 *
 * @param op The operand information including its type and value.
 * @param isDest Indicates if the operand is a destination (true) or source (false).
 * @return A memoryWord structure populated based on the operand type and value.
 */
memoryWord convertOperandToMemoryWord(operandInfo op, boolean isDest);

/**
 * @brief Adds a word to the memory array if there is available space.
 *
 * This function inserts a memory word into the specified memory array, provided
 * that there is still room in the array (i.e., the counter is below the RAM limit).
 * The counter is incremented after adding the word.
 * @param memoryArr Pointer to the array where the memory word will be added.
 * @param memoryCounter Pointer to the counter tracking the current number of elements in the memory array.
 * @param memory The memory word to be added.
 */
void insertWordIntoMemory(int *memoryArr, int *memoryCounter, memoryWord memory);

 /**
 * @brief Processes a line of code and updates memory accordingly.
 *
 * This function checks if the provided line of code is valid and processes it by updating operand label addresses, 
 * adding the command and operand memory words to memory, and handling different operand types.
 * It also marks the line as erroneous if label updates fail.
 * @param memoryArr Pointer to the array of memory words.
 * @param memoryCounter Pointer to the memory counter.
 * @param line Pointer to the lineInfo structure containing the line of code to be processed.
 *
 * @return boolean TRUE if the line was processed without errors, FALSE otherwise.
 */
boolean processLineToMemory(int *memoryArr, int *memoryCounter, lineInfo *line);

/**
 * @brief Adds data to a memory array with a mask applied.
 *
 * This function takes data from a global array and adds it to the provided
 * memory array until the memory counter reaches a limit or all data is processed.
 * A mask is applied to each data item before adding it to the memory array.
 * @param memoryArr Pointer to the memory array where data will be added.
 * @param memoryCounter Pointer to the counter tracking the current position in the memory array.
 * @param DC The data counter value at the end of the first pass.
 */
void insertDataWithMask(int *memoryArr, int *memoryCounter, int DC);

 /**
 * @brief Processes lines of code, adjusts label addresses, and updates memory with data (Second pass).
 *
 * This function performs the following tasks:
 * 1. Updates data label addresses based on the current instruction counter (IC).
 * 2. Validates entry labels and counts illegal entries.
 * 3. Processes each line and adds it to memory.
 * 4. Inserts data into memory with appropriate masking.
 * @param memoryArr Pointer to the array representing memory.
 * @param linesArr Pointer to the array of line information.
 * @param lineNum The number of lines to process.
 * @param IC The instruction counter value at the end of the first pass.
 * @param DC The data counter value at the end of the first pass.
 * @return The total number of errors found during processing.
 */
int secondPass(int *memoryArr, lineInfo *linesArr, int lineNum, int IC, int DC);

#endif
