/* Name: Almog Hakak, ID: 211825229 */

#include "main.h"
#include "helpers.h"
#include "preprocessor.h"
#include "first_pass.h"
#include "second_pass.h"

int g_entryLabelsCount = 0;            /* Counter of entry labels. */
int g_labelCount = 0;                  /* Counter of labels. */
int g_arr[RAM_LIMIT];                  /* Array to store data values. */
lineInfo *g_entryLinesArr[LABELS_MAX]; /* Array of pointers to lineInfo structures for entry lines. */
labelInfo g_labelsArr[LABELS_MAX];     /* Array of labelInfo structures to store labels. */


/**
 * Processes the input file and performs assembly operations.
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return 0 on successful completion, non-zero on error
 */
int main(int argc, char *argv[])
{
    int i;
    FILE *file;
    char *source_file, *macro_file, *temp_file;

    if (argc < 2)
    {
        printf("ERROR: No file was given .\n");
        return 1;
    }

    /* Main loop on each File*/
    for (i = 1; i < argc; i++)
    {
        printf("Starting preprocessor \n");
        source_file = addNewFile(argv[i], ".as");       /* Creates a file with ".as". */
        temp_file = removeExtraSpacesFile(source_file); /* Handling spaces in the source file. */

        /* Handling error in allocation memory */
        if (!temp_file)
        {
            free(source_file);
            continue;
        }

         /* Run the macro preprocessor on the temp file, handle errors in current file. */
        if (!macroExecute(temp_file))
        {
            free(source_file);
            free(temp_file);
            continue;
        }

        printf("Starting first pass\n");
        macro_file = addNewFile(argv[i], ".am"); /* Creates a file with ".am". */
        remove(temp_file);
        file = fopen(macro_file, "r");          /* Opens macro file in reading mode. */
        
         /* Handling error */
        if (!file) 
        {
            printf("ERROR: File cant be open \"%s\".\n", macro_file);
            free(source_file);
            free(macro_file);
            continue;
        }

        parseFile(macro_file); /* Parse the .am file after macro expansion and first pass. */

        /* Freeing the allocated memory. */
        free(source_file);
        free(macro_file);
    }
    
    printf("Finished\n");
    return 0;
}
