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

/* A R E type as bits*/
typedef enum { 
    ARE_EXT = 1,          /* External */
    ARE_RELOC = 2,        /* Relocatable */
    ARE_ABS = 4           /* Absolute */
} AREKind;

/* Numbers as bit flags corresponding to each operand type. */
typedef enum { 
    OP_NUMERIC = 1,       /* Numeric operand */
    OP_LABEL = 2,         /* Label operand */
    OP_INDIRECT_REG = 4,  /* Indirect register operand */
    OP_REGULAR_REG = 8,   /* Register operand */
    OP_INVALID = -1       /* Invalid operand */
} OperandType; 

typedef unsigned int boolean; /* TRUE or FALSE values */

/* Directive Structure */
typedef struct {
    char *name;          /* Directive name. */
    void (*parseFunc)(); /* Function pointer to the function that parses this directive. */
} directive;

/* Command Structure */
typedef struct 
{
    char *name;              /* Command name. */
    unsigned int opcode : 4; /* opcode uses 4 bits. */
    int numOfParams;         /* Number of parameters  */
} command;

/* Macro Node Structure */
typedef struct macroNode{
    char *name;                 /* Macro identifier. */
    int line;                   /* Line number where the macro is declared. */
    char *content;              /* Macro definition. */
    struct macroNode *next;     /* Link to the next macro node. */
} MacroNode;

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
	OperandType type; /* Type. */
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
extern labelInfo g_labelsArr[LABELS_MAX];

/**
 * @brief Declares an external array of pointers to `lineInfo` structures.
 * This array holds pointers to lines that define entry labels.
 */
extern lineInfo *g_entryLinesArr[LABELS_MAX];

/**
 * @brief Declares an external integer variable to track the number of labels.
 * This variable keeps track of the number of labels currently stored in `g_labelsArr`.
 */
extern int g_labelCount;

/**
 * @brief Declares an external integer variable to track the number of entry labels.
 * This variable keeps track of the number of entry labels currently stored in `g_entryLinesArr`.
 */
extern int g_entryLabelsCount;

/**
 * @brief Declares an external array of integers to store numerical data.
 * This array stores the numerical data values encountered during the assembly process.
 */
extern int g_arr[RAM_LIMIT];

#endif