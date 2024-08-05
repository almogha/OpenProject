/* Name: Almog Hakak, ID: 211825229 */

#include "main.h"
#include "errors.h"
#include "helpers.h"


char *allocateMemory(size_t size)
{
    char *ptr = (char *)malloc(size);
    if (!ptr)
    {
        logAndExitOnInternalError("ERROR: Allocation of memory failed");
    }
    return ptr;
}

char *extractMacroData(FILE *fp, fpos_t *pos, int *line_count)
{
    char str[LINE_MAX_LENGTH];
    int macro_length = 0;
    char *macro;

    fsetpos(fp, pos);
    while (fgets(str, LINE_MAX_LENGTH, fp) && strcmp(str, "endmacr\n") != 0)
    {
        (*line_count)++;             /* Increment line count for each line read. */
        macro_length += strlen(str); /* Calculate total length of the macro. */
    }
    fsetpos(fp, pos);

    if (feof(fp))
    {
        logAndExitOnInternalError("ERROR: Cant find macro ending");
        return NULL;
    }

    macro = allocateMemory(macro_length + 1);
    macro[0] = '\0';
    while (fgets(str, LINE_MAX_LENGTH, fp) && strcmp(str, "endmacr\n") != 0)
    {
        strcat(macro, str);
    }
    return macro;
}

int analyzeMacroDefinition(char *str, char **name, int line_count, char *file_name)
{
    char *temp_name = strtok(NULL, " \n");
    if (!temp_name)
    {
        logAndExitOnInternalError("ERROR:  Word cant be found in macro");
        return 0;
    }
    *name = allocateMemory(strlen(temp_name) + 1);
    strcpy(*name, temp_name);
    return 1;
}

int importMacros(char *file_name, MacroNode **head)
{
    FILE *fp = fopen(file_name, "r");
    char str[LINE_MAX_LENGTH];
    char *name, *content;
    int line_count = 0;
    fpos_t pos;

    if (!fp)
    {
        logAndExitOnInternalError("ERROR: Failed to open file");
        return 0;
    }

    while (fgets(str, LINE_MAX_LENGTH, fp))
    {
        line_count++; /* Increment line count for each line read. */
        if (strcmp(strtok(str, " "), "macr") == 0)
        {
            if (!analyzeMacroDefinition(str, &name, line_count, file_name))
            {
                fclose(fp);
                return 0;
            }
            fgetpos(fp, &pos);
            content = extractMacroData(fp, &pos, &line_count);
            if (!content)
            {
                fclose(fp);
                return 0;
            }
            addToTheList(head, name, content, line_count);
        }
    }
    fclose(fp);
    return 1;
}

char *substitutePlaceholder(char *str, MacroNode *macr)
{
    char *pos = strstr(str, macr->name);
    size_t new_len;
    char *new_str;

    if (!pos)
    {
        return NULL;
    }
    new_len = strlen(str) + strlen(macr->content) - strlen(macr->name) + 1;
    new_str = allocateMemory(new_len);
    strncpy(new_str, str, pos - str);
    new_str[pos - str] = '\0';
    strcat(new_str, macr->content); /* Concatenate macro content. */
    strcat(new_str, pos + strlen(macr->name)); /* Concatenate remaining string. */
    return new_str;
}

void replaceMacroReferences(char *input_file, MacroNode *head)
{
    FILE *fp_in = fopen(input_file, "r"); /* Open the input file for reading. */
    FILE *fp_out = fopen("temp_output_file", "w"); /* Open a temporary file for writing. */
    char str[LINE_MAX_LENGTH];
    char *modified_str;
    char *token;
    MacroNode *current;

    if (!fp_in || !fp_out)
    {
        logAndExitOnInternalError("ERROR: Failed to open file");
        if (fp_in) fclose(fp_in);
        if (fp_out) fclose(fp_out);
        return;
    }

    while (fgets(str, LINE_MAX_LENGTH, fp_in))
    {
        char *original_str = stringDuplicate(str); /* Duplicate the original string. */

        token = strtok(str, " \n");

        if (token && strcmp(token, "macr") == 0) /* Check for macro declaration. */
        {
            while (fgets(str, LINE_MAX_LENGTH, fp_in))
            {
                token = strtok(str, " \n");
                if (token && strcmp(token, "endmacr") == 0)
                {
                    break; /* Skip lines until the end of macro declaration. */
                }
            }
            continue;
        }

        current = head;
        while (current != NULL)
        {
            modified_str = substitutePlaceholder(original_str, current); /* Replace macros in the line. */
            if (modified_str)
            {
                strcpy(original_str, modified_str);
                free(modified_str);
            }
            current = current->next;
        }
        fprintf(fp_out, "%s", original_str); /* Write the modified line to the temporary file.*/
        free(original_str);
    }

    fclose(fp_in); /* Close the input file. */
    fclose(fp_out); /* Close the temporary file. */

    remove(input_file); /* Remove the original input file. */
    rename("temp_output_file", input_file); /* Rename the temporary file to the original input file. */
}

int processMacros(char *file_name)
{
    MacroNode *head = NULL;
    char *new_file_name = addNewFile(file_name, ".am"); /* Create the new .am file name. */

    if (!importMacros(file_name, &head))
    {
        free(new_file_name);
        return 0;
    }

    replaceMacroReferences(file_name, head); /* Process macro calls in the file. */

    if (!copyFile(new_file_name, file_name))
    {
        logAndExitOnInternalError("Failed to copy processed file to new file");
        freeList(head);
        free(new_file_name);
        return 0;
    }

    freeList(head); /* Free the macro list. */
    printf("Macro execution completed, output file: %s\n", new_file_name);
    free(new_file_name); /* Free the new file name. */
    return 1;
}