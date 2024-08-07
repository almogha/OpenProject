/* Name: Almog Hakak, ID: 211825229 */

#include "main.h"
#include "errors.h"
#include "helpers.h"
#include "first_pass.h"

int initialAssemblerPass(FILE *file, lineInfo *linesArr, int *linesCount, int *IC, int *DC) /* Documentation in "assembler.h". */
{
	char lineStr[LINE_MAX_LENGTH + 2]; /* +2 for the \n and \0 at the end */
	int errorsFound = 0;
	*linesCount = 0;

	
	while (!feof(file)) /* Read lines and parse them. */
	{
		if (fetchLine(file, lineStr, LINE_MAX_LENGTH + 2)) 
		{
			if (*linesCount >= LINES_MAX_LENGTH) /* Checks if the file is too long. */
			{
				printf("ERROR: The file is too long. Max number of lines in a file is %d.\n", LINES_MAX_LENGTH);
				return ++errorsFound;
			}

			analyzeAssemblyLine(&linesArr[*linesCount], lineStr, *linesCount + 1, IC, DC); /* Parse a line. */

			if (linesArr[*linesCount].isError) /* Update errorsFound. */
			{
				errorsFound++;
			}

			if (*IC + *DC >= RAM_LIMIT) /* Check if the number of memory words needed is small enough. */
			{
				
				printError(*linesCount + 1, "ERROR: The max memory words is %d, too much data and code.", RAM_LIMIT); /* dataArr is full. Stop reading the file. */
				printf("Memory is full, file reading terminated.\n");
				return ++errorsFound;
			}
			++*linesCount;
		}
		else if (!feof(file))
		{
			
			printError(*linesCount + 1, "ERROR: The max line length is %d, line is too long.", LINE_MAX_LENGTH); /* Line is too long. */
			errorsFound++;
			 ++*linesCount;
		}
	}

	return errorsFound;
}

boolean fetchLine(FILE *file, char *line_data, size_t maxLength) /* Documentation in "assembler.h". */
{
	char *endOfLine;

	if (!fgets(line_data, maxLength, file))
	{
		return FALSE;
	}
	endOfLine = strchr(line_data, '\n'); /* Check if the line is too long (no '\n' was present). */

	if (endOfLine)
	{
		*endOfLine = '\0';
	}
	else
	{
		char c;
		boolean ret = (feof(file)) ? TRUE : FALSE; /* Return FALSE, unless it's the end of the file. */

		do /* Keep reading chars until you reach the end of the line ('\n') or EOF. */
		{
			c = fgetc(file);
		} while (c != '\n' && c != EOF);

		return ret;
	}

	return TRUE;
}

labelInfo *insertLabelIfValid(labelInfo label, lineInfo *line) /* Documentation in "assembler.h". */
{
	if (!isLegalLabel(line->lineStr, line->lineNum, TRUE)) /* Check if the label is legal. */
	{
		line->isError = TRUE; /* Illegal label name. */
		return NULL;
	}
	/* Checks if the label already exists. */
	if (isExistingLabel(line->lineStr))
	{
		printError(line->lineNum, "ERROR: Label already exists.");
		line->isError = TRUE;
		return NULL;
	}
	strcpy(label.name, line->lineStr); /* Add the name to the label. */
	if (g_labelCount < LABELS_MAX) /* Add the label to g_labelsArr and to the lineInfo. */
	{
		g_labelsArr[g_labelCount] = label;
		return &g_labelsArr[g_labelCount++];
	}
	
	printError(line->lineNum, "ERROR: Too many labels - max is %d.", LABELS_MAX, TRUE); /* Too many labels. */
	line->isError = TRUE;
	return NULL;
}

boolean insertValueIntoDataArray(int num, int *IC, int *DC, int lineNum) /* Documentation in "assembler.h". */
{
	if (*DC + *IC < RAM_LIMIT) /* Checks if there is enough space in g_arr for the data. */
	{
		g_arr[(*DC)++] = num;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

char *locateAndProcessLabel(lineInfo *line, int IC) /* Documentation in "assembler.h". */
{
	char *labelEnd = strchr(line->lineStr, ':');
	labelInfo label = { 0 };
	label.address = INITIAL_ADDRESS + IC;

	if (!labelEnd) /* Find the label (or return NULL if there isn't) */
	{
		return NULL;
	}
	*labelEnd = '\0';

	if (!isOneWord(line->lineStr)) /* Check if the ':' came after the first word */
	{
		*labelEnd = ':'; /* Fix the change in line->lineStr. */
		return NULL;
	}

	line->label = insertLabelIfValid(label, line); /* Check of the label is legal and add it to the labelList. */
	return labelEnd + 1; /* +1 to make it point at the next char after the \0. */
}

void handleDataCommand(lineInfo *line, int *IC, int *DC) /* Documentation in "assembler.h". */
{
	char *operandTok = line->lineStr, *endOfOp = line->lineStr;
	int operandValue;
	boolean foundComma;

	if (line->label) /* Make the label a data label (if there is one). */
	{
		line->label->isData = TRUE;
		line->label->address = INITIAL_ADDRESS + *DC;
	}

	if (isWhiteSpaces(line->lineStr)) /* Checks if there are params. */
	{
		printError(line->lineNum, "ERROR: No parameter.");
		line->isError = TRUE;
		return;
	}

	INFINITE_LOOP /* Find all the params and add them to g_arr */
	{
		if (isWhiteSpaces(line->lineStr)) /* Get next param or break if there is none. */
		{
			break;
		}
		operandTok = getFirstOperand(line->lineStr, &endOfOp, &foundComma);
		
		if (isLegalNum(operandTok, WORD_LENGTH - 3, line->lineNum, &operandValue)) /* Add the param to g_arr. */
		{
			if (!insertValueIntoDataArray(operandValue, IC, DC, line->lineNum))
			{
				line->isError = TRUE; /* Not enough memory. */
				return;
			}
		}
		else
		{
			line->isError = TRUE; /* Illegal number. */
			return;
		}
		line->lineStr = endOfOp; /* Change the line to start after the parameter. */
	}

	if (foundComma)
	{
		printError(line->lineNum, "ERROR: Commas found after the last parameter."); /* Comma after the last param. */
		line->isError = TRUE;
		return;
	}
}

void handleStringDirective(lineInfo *line, int *IC, int *DC) /* Documentation in "assembler.h". */
{
	char *str;
    if (line->label) /* Make the label a data label (if there is one). */
    {
        line->label->isData = TRUE;
        line->label->address = INITIAL_ADDRESS + *DC;
    }
    trimStr(&line->lineStr);

    if (isLegalStringParam(&line->lineStr, line->lineNum))
    {
		str = line->lineStr;
        while (*str)
        {
            if (!insertValueIntoDataArray((int)*str, IC, DC, line->lineNum))
            {
                line->isError = TRUE; /* Not enough memory. */
                return;
            }
            str++;
        }
		/* Ensure the string is null-terminated in the data array */
        if (!insertValueIntoDataArray(0, IC, DC, line->lineNum))
        {
            line->isError = TRUE; /* Not enough memory. */
            return;
        }
    }
    else
    {
        line->isError = TRUE; /* Illegal string. */
        return;
    }
}

void handleExternalDirective(lineInfo *line) /* Documentation in "assembler.h". */
{
	labelInfo label = { 0 }, *labelPointer;

	if (line->label) /* If there is a label in the line, remove the it from labelArr. */
	{
		g_labelCount--;
		printf("WARNING: At line %d: The assembler ignored the label before the directive.\n", line->lineNum);
	}

	trimStr(&line->lineStr);
	labelPointer = insertLabelIfValid(label, line);

	if (!line->isError) /* Make the label an extern label. */
	{
		labelPointer->address = 0;
		labelPointer->isExtern = TRUE;
	}
}

void handleEntryCommand(lineInfo *line) /* Documentation in "assembler.h". */
{
	if (line->label) /* If there is a label in the line, remove the it from labelArr. */
	{
		g_labelCount--;
		printf("WARNING: At line %d: The assembler ignored the label before the directive.\n", line->lineNum);

	}

	trimStr(&line->lineStr); /* Add the label to the entry labels list. */

	if (isLegalLabel(line->lineStr, line->lineNum, TRUE))
	{
		if (isExistingEntryLabel(line->lineStr))
		{
			printError(line->lineNum, "ERROR: Label already defined as an entry label.");
			line->isError = TRUE;
		}
		if (g_entryLabelsCount < LABELS_MAX)
		{
			g_entryLinesArr[g_entryLabelsCount++] = line;
		}
	}
}

void handleDirective(lineInfo *line, int *IC, int *DC) /* Documentation in "assembler.h". */
{
	int i = 0;
	while (g_dircArr[i].name)
	{
		if (!strcmp(line->commandStr, g_dircArr[i].name))
		{	
			g_dircArr[i].parseFunc(line, IC, DC); /* Call the parse function for this type of directive. */
			return;
		}
		i++;
	}	
	
	printError(line->lineNum, "ERROR: No such directive as \"%s\".", line->commandStr); /* line->commandStr isn't a real directive. */
	line->isError = TRUE;
}

boolean areLegalOpTypes(const command *cmd, operandInfo op1, operandInfo op2, int lineNum) /* Documentation in "assembler.h". */
{
	/* Checks First Operand. */
	if (cmd->opcode == 4 && op1.type != OP_LABEL) /* "lea" command (opcode is 4) can only get a label as the 1st op. */
	{
		printError(lineNum, "ERROR: Source operand for \"%s\" command must be a label.", cmd->name);
		return FALSE;
	}

	/* Checks Second Operand.*/
	if (op2.type == OP_NUMERIC && cmd->opcode != 1 && cmd->opcode != 12) /* 2nd operand can be a number only if the command is "cmp" (opcode is 1) or "prn" (opcode is 12). */
	{
		printError(lineNum, "ERROR: Destination operand for \"%s\" command can't be a number.", cmd->name);
		return FALSE;
	}
	return TRUE;
}

void parseOpInfo(operandInfo *operand, int lineNum) /* Documentation in "assembler.h". */
{
	int value = 0;

	if (isWhiteSpaces(operand->str))
	{
		printError(lineNum, "ERROR: Empty parameter.");
		operand->type = OP_INVALID;
		return;
	}

	if (*operand->str == '#') /* Checks if the type is a OP_NUMERIC. */
	{
		operand->str++; /* Remove the '#'. */
		if (isspace(*operand->str)) /* Checks if the number is legal. */
		{
			printError(lineNum, "ERROR: There is a white space after the '#'.");
			operand->type = OP_INVALID;
		}
		else
		{
			operand->type = isLegalNum(operand->str, WORD_LENGTH - 3, lineNum, &value) ? OP_NUMERIC : OP_INVALID;
		}
	 }
	
	else if (isIndirectRegister(operand->str, &value)) /* Checks if the type is OP_INDIRECT_REG. */
	{
		operand->type = OP_INDIRECT_REG;
	}
	else if (isRegister(operand->str, &value)) /* Checks if the type is OP_REGULAR_REG. */
	{
		operand->type = OP_REGULAR_REG;
	}
	else if (isLegalLabel(operand->str, lineNum, FALSE)) /* Checks if the type is OP_LABEL. */
	{
		operand->type = OP_LABEL;
	}
	else /* The type is OP_INVALID. */
	{
		printError(lineNum, "ERROR: \"%s\" is an invalid parameter.", operand->str);
		operand->type = OP_INVALID;
		value = -1;
	}

	operand->value = value;
}
	
void parseCmdOperands(lineInfo *line, int *IC, int *DC) /* Documentation in "assembler.h". */
{
	char *startOfNextPart = line->lineStr;
	boolean foundComma = FALSE;
	int numOfOpsFound = 0;

	/* Reset the op types. */
	line->op1.type = OP_INVALID;
	line->op2.type = OP_INVALID;
	
	INFINITE_LOOP /* Get the parameters. */
	{
		/* If both of the operands are registers, or indirect registers they will only take 1 memory word (instead of 2). */
		if (!(line->op1.type == OP_REGULAR_REG && line->op2.type == OP_REGULAR_REG) && !(line->op1.type == OP_REGULAR_REG && line->op2.type == OP_INDIRECT_REG) && !(line->op1.type == OP_INDIRECT_REG && line->op2.type == OP_REGULAR_REG) && !(line->op1.type == OP_INDIRECT_REG && line->op2.type == OP_INDIRECT_REG))
		{	
			if (*IC + *DC < RAM_LIMIT) /* Checks if there is enough memory. */
			{
				++*IC; /* Count the last command word or operand. */
			}
			else
			{
				line->isError = TRUE;
				return;
			}
		}
		
		if (isWhiteSpaces(line->lineStr) || numOfOpsFound > 2) /* Checks if there are still more operands to read. */
		{
			break; /* If there are more than 2 operands it's illegal. */
		}

		if (numOfOpsFound == 1) /* If there are 2 ops, make the destination become the source op. */
		{
			line->op1 = line->op2;
			line->op2.type = OP_INVALID; /* Reset op2. */
		}
		
		line->op2.str = getFirstOperand(line->lineStr, &startOfNextPart, &foundComma); /* Parse the opernad. */
		parseOpInfo(&line->op2, line->lineNum);

		if (line->op2.type == OP_INVALID)
		{
			line->isError = TRUE;
			return;
		}

		numOfOpsFound++;
		line->lineStr = startOfNextPart;
	} /* While loop end. */

	if (numOfOpsFound != line->cmd->numOfParams) /* Checks if there are enough operands. */
	{
		
		if (numOfOpsFound < line->cmd->numOfParams) /* Checks if there are more or less operands than needed. */
		{
			printError(line->lineNum, "ERROR: Not enough operands.", line->commandStr);
		}
		else
		{
			printError(line->lineNum, "ERROR: Too many operands.", line->commandStr);
		}
		line->isError = TRUE;
		return;
	}

	if (foundComma) /* Check if there is a comma after the last param. */
	{
		printError(line->lineNum, "Don't write a comma after the last parameter.");
		line->isError = TRUE;
		return;
	}
	
	if (!areLegalOpTypes(line->cmd, line->op1, line->op2, line->lineNum)) /* Checsk if the operands types are legal. */
	{
		line->isError = TRUE;
		return;
	}
}

void analyzeCommandArguments(lineInfo *line, int *IC, int *DC) /* Documentation in "assembler.h". */
{
	int cmdId = getCmdId(line->commandStr);

	if (cmdId == -1)
	{
		line->cmd = NULL;
		if (*line->commandStr == '\0')
		{	
			printError(line->lineNum, "ERROR: Can't write a label to an empty line.", line->commandStr); /* The command is empty, but the line isn't empty so it's only a label. */
		}
		else
		{
			printError(line->lineNum, "ERROR: No such command as \"%s\".", line->commandStr); /* Illegal command. */
		}
		line->isError = TRUE;
		return;
	}

	line->cmd = &g_opArr[cmdId];
	parseCmdOperands(line, IC, DC);
}

char *duplicateString(const char *str) 
{
	char *newString = (char *)malloc(strlen(str) + 1);
	if (newString) 
	{
		strcpy(newString, str); 
	}

	return newString;
}

void analyzeAssemblyLine(lineInfo *line, char *lineStr, int lineNum, int *IC, int *DC) /* Documentation in "assembler.h". */
{
	char *startOfNextPart = lineStr;

	line->lineNum = lineNum;
	line->address = INITIAL_ADDRESS + *IC;
	line->originalString = duplicateString(lineStr);
	line->lineStr = line->originalString;
	line->isError = FALSE;
	line->label = NULL;
	line->commandStr = NULL;
	line->cmd = NULL;

	if (!line->originalString)
	{
		printf("ERROR: Malloc failed, not enough memory.");
		return;
	}
	if (isCommentOrEmpty(line)) /* Check if the line is a comment. */
	{	
		return;
	}
	startOfNextPart = locateAndProcessLabel(line, *IC); /* Find a label and add it to the label list. */

	if (line->isError)
	{
		return;
	}
	if (startOfNextPart) /* Update the line if startOfNextPart isn't NULL. */
	{
		line->lineStr = startOfNextPart;
	}	
	line->commandStr = getFirstTok(line->lineStr, &startOfNextPart); /* Find the command token. */
	line->lineStr = startOfNextPart;
	
	if (isDirective(line->commandStr)) /* Parse the command / directive. */
	{
		line->commandStr++; /* Remove the '.' from the command. */
		handleDirective(line, IC, DC);
	}
	else
	{
		analyzeCommandArguments(line, IC, DC);
	}
	if (line->isError)
	{
		return;
	}
}
