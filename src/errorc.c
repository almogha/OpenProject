/* Name: Almog Hakak, ID: 211825229 */

#include "main.h"
#include "errors.h"

void logAndExitOnInternalError(const char *message)
{
    fprintf(stdout, "Internal Error: %s\n", message);
    exit(EXIT_FAILURE);
}

void printError(int lineNum, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf("line %d ", lineNum); /* Print the error message with the line number. */
    vprintf(format, args);        /* Print the formatted error message. */
    printf("\n");
    va_end(args);
}
