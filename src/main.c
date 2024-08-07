/* Name: Almog Hakak, ID: 211825229 */

#include "main.h"
#include "errors.h"
#include "helpers.h"
#include "preprocessor.h"
#include "first_pass.h"
#include "second_pass.h"


int g_entryLabelsCount = 0;            /* Counter of entry labels. */
int g_labelCount = 0;                  /* Counter of labels. */
int g_arr[RAM_LIMIT];                  /* Array to store data values. */
lineInfo *g_entryLinesArr[LABELS_MAX]; /* Array of pointers to lineInfo structures for entry lines. */
labelInfo g_labelsArr[LABELS_MAX];     /* Array of labelInfo structures to store labels. */

const directive g_dircArr[] = 
{
	{ "data", parseDataDirc } ,
	{ "string", parseStringDirc } ,
	{ "extern", parseExternDirc },
	{ "entry", parseEntryDirc },
	{ NULL }
};	

const command g_opArr[] =	
{
	{ "mov", 0, 2 } , 
	{ "cmp", 1, 2 } ,
	{ "add", 2, 2 } ,
	{ "sub", 3, 2 } ,
	{ "lea", 4, 2 } ,
	{ "clr", 5, 1 } ,
	{ "not", 6, 1 } ,
	{ "inc", 7, 1 } ,
	{ "dec", 8, 1 } ,
	{ "jmp", 9, 1 } ,
	{ "bne", 10, 1 } ,
	{ "red", 11, 1 } ,
	{ "prn", 12, 1 } ,
	{ "jsr", 13, 1 } ,
	{ "rts", 14, 0 } ,
	{ "stop", 15, 0 } ,
	{ NULL }
}; 

/**
 * Processes the input file and performs assembly operations.
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return 0 on successful completion, non-zero on error
 */
int main(int argc, char *argv[])
{
    int IC = 0, DC = 0, errorsCount = 0, linesCount = 0, ramArr[RAM_LIMIT] = {0}, i;
    char *source_file, *macro_file, *temp_file, macroFileName[FILENAME_MAX_LENGTH];
    lineInfo linesArr[LINES_MAX_LENGTH];
    FILE *file;

    if (argc < 2)
    {
        printf("ERROR: No file was given.\n");
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
        if (!processMacros(temp_file))
        {
            free(source_file);
            free(temp_file);
            continue;
        }

        printf("Starting first pass\n");
        macro_file = addNewFile(argv[i], ".am"); /* Creates a file with ".am". */
        remove(temp_file);
        file = fopen(macro_file, "r");           /* Opens macro file in reading mode. */
        
         /* Handling error */
        if (!file) 
        {
            printf("ERROR: File cant be open \"%s\".\n", macro_file);
            free(source_file);
            free(macro_file);
            continue;
        }

        clearData(linesArr, linesCount, IC + DC); /* Reset data. */
        sprintf(macroFileName, "%s", macro_file);

        file = fopen(macroFileName, "r");
        if (file == NULL)
        {
            printf("ERROR: Can't open the file \"%s\".\n", macroFileName);
            return 1;
        }

        errorsCount += firstPass(file, linesArr, &linesCount, &IC, &DC);

        printf("Starting second pass\n");
        errorsCount += secondPass(ramArr, linesArr, linesCount, IC, DC);

        if (errorsCount == 0)
        {
            createObjectFile(macro_file, IC, DC, ramArr);    /* .ob file creation. */
            createExternFile(macro_file, linesArr, linesCount); /* .ext file creation. */
            createEntriesFile(macro_file);                      /* .ent file creation. */
            printf("Outputs were created for file %s.\n", macro_file);
        }
        else
        {
            printf("Number of Errors: %d found in %s.\n", errorsCount, macro_file);
        }

        clearData(linesArr, linesCount, IC + DC); /* Clear the data and reset global variables. */
        fclose(file);

        /* Freeing the allocated memory. */
        free(source_file);
        free(macro_file);
    }
    
    printf("Finished\n\n");
    return 0;
}
