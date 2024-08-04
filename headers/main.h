/* Name: Almog Hakak, ID: 211825229 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

/* Constants */
#define RAM_LIMIT 4096
#define NUM_OF_REG 7
#define WORD_LENGTH 15
#define BYTE_LENGTH 8
#define BASE_OCTAL 8
#define BASE_DECIMAL 10
#define INITIAL_ADDRESS 100 
#define LABEL_MAX_LENGTH 31
#define LINES_MAX_LENGTH 300
#define LABELS_MAX LINES_MAX_LENGTH
#define LINE_MAX_LENGTH 80
#define FILENAME_MAX_LENGTH 256
#define SINGLE_DIGIT 1
#define DOUBLE_DIGIT 2
#define TRIPLE_DIGIT 3
#define QUADRUPLE_DIGIT 4
#define FALSE 0
#define TRUE 1
#define INFINITE_LOOP for(;;)

/*The numbers are assigned as the bits which will be turned on regarding the matching opType. */
typedef enum { NUMBER = 1, LABEL = 2, INDREGISTER = 4, REGISTER = 8, INVALID = -1 } opType; /* Operands */

/*The numbers are assigned as the bits which will be turned on regarding the matching ARE type. */
typedef enum { EXTERNAL = 1, RELOCATABLE = 2, ABSOLUTE = 4 } areType;

/*-- typedef unsigned int bool;  Only get TRUE or FALSE values */
typedef unsigned int boolean; /* TRUE or FALSE values */


typedef struct node /* Node Structure */
{
    char *name; /* The name of the macro. */
    char *content; /* The content of the macro. */
    int line; /* The line number where the macro is defined. */
    struct node *next; /* Pointer to the next node in the list. */
} node;

typedef struct /* Command Structure */
{
    char *name; /* The name of the command. */
    unsigned int opcode : 4; /* The opcode of the command, using 4 bits. */
    int numOfParams; /* The number of parameters the command takes. */
} command;

typedef struct /* Directive Structure */
{
    char *name; /* The name of the directive. */
    void (*parseFunc)(); /* Function pointer to the function that parses this directive. */
} directive;

typedef struct /* Labels Structure */
{
	int address; /* The address it contains. */
	char name[LABEL_MAX_LENGTH]; /* The name of the label. */					
	boolean isExtern; /* Extern flag. */
	boolean isData; /* Data flag (.data or .string). */
} labelInfo;

typedef struct /* Operand Structure */
{
	int value; /* Value. */
	char *str; /* String. */
	opType type; /* Type. */
	int address; /* The address of the operand in the memory. */
} operandInfo;

typedef struct /* Line Structure */
{
	int lineNum; /* The number of the line in the file. */
	int address; /* The address of the first word in the line. */
	char *originalString; /* The original pointer, allocated by malloc. */
	char *lineStr; /* The text it contains (changed while using parseLine). */
	boolean isError; /* Represent whether there is an error or not. */
	labelInfo *label; /* A poniter to the lines label in labelArr. */
	char *commandStr; /* The string of the command or directive. */
	const command *cmd;	/* A pointer to the command in g_cmdArr. */
	operandInfo op1; /* The 1st operand. */
	operandInfo op2; /* The 2nd operand. */
} lineInfo;

typedef struct /* Memory Word Structure - 15 bits */
{
	unsigned int are : 3;
	union /* 12 bits */
	{
		struct /* Commands (only 12 bits) */
		{
			unsigned int dest : 4; /* Destination op addressing method ID. */
			unsigned int src : 4; /* Source op addressing method ID. */
			unsigned int opcode : 4; /* Command ID. */
		} cmdBits;

		struct /* Registers (only 6 bits) */
		{
			unsigned int destBits : 3; /* DEST Register. */
			unsigned int srcBits : 3; /* SRC Register. */
		} regBits;
		/* Other operands */
		int value : 12; /* (12 bits) */

	} valueBits; /* End of 12 bits union. */

} memoryWord;

/**
 * @brief Declares an external array of `command` structures.
 * This array holds information about the available commands in the assembler.
 * Each `command` structure typically includes the command's name, opcode, and the number of parameters it accepts.
 */
extern const command g_cmdArr[];

/**
 * @brief Declares an external array of `labelInfo` structures.
 * This array stores information about labels encountered during the assembly process.
 * Each `labelInfo` structure contains details like the label's address, name, and flags
 * indicating if it is external or associated with data.
 */
extern labelInfo g_labelArr[LABELS_MAX];

/**
 * @brief Declares an external array of pointers to `lineInfo` structures.
 * This array holds pointers to lines that define entry labels.
 */
extern lineInfo *g_entryLines[LABELS_MAX];

/**
 * @brief Declares an external integer variable to track the number of labels.
 * This variable keeps track of the number of labels currently stored in `g_labelArr`.
 */
extern int g_labelNum;

/**
 * @brief Declares an external integer variable to track the number of entry labels.
 * This variable keeps track of the number of entry labels currently stored in `g_entryLines`.
 */
extern int g_entryLabelsNum;

/**
 * @brief Declares an external array of integers to store numerical data.
 * This array stores the numerical data values encountered during the assembly process.
 */
extern int g_dataArr[RAM_LIMIT];

#endif