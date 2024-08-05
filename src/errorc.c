/* Name: Almog Hakak, ID: 211825229 */

#include "main.h"
#include "errors.h"

void logAndExitOnInternalError(const char *message)
{
    fprintf(stdout, "Internal Error: %s\n", message);
    exit(EXIT_FAILURE);
}