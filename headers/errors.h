
/* Name: Almog Hakak, ID: 211825229
*
* Errors Functions
*/

#ifndef ERRORS_H
#define ERRORS_H

#include "main.h"


/**
 * @brief Outputs an internal error message and terminates the program.
 * 
 * This function outputs an internal error message to the standard output and ends the program
 * with a failure status.
 * @param message The error message to be displayed.
 */
void logAndExitOnInternalError(const char *message);

/**
 * Prints an error message with the line number.
 * @param lineNum The line number where the error occurred.
 * @param format The format string for the error message.
 * @param ... Additional arguments for the format string.
 */
void printError(int lineNum, const char *format, ...);

#endif
