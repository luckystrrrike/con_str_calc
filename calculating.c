#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "calculating.h"
#include "stack.h"

/* Static function for working with an expression on the infix notation */

static int getPriority(char ch) {
	switch (ch) {
	case SUB: return PRIOR_SUB;
	case ADD: return PRIOR_ADD;
	case MUL: return PRIOR_MUL;
	case DIV: return PRIOR_DIV;
	case POW: return PRIOR_POW;
	case OP_BRACE: return PRIOR_OP_BR; // opening parethesis
	case CL_BRACE: return PRIOR_CL_BR; // closing parethesis
	default: return -1;
	}
}

/* Function for working with an expression on the RPN */

int getOperand(char* num, char expression[], char** end)
{
	unsigned counter = 0;
	char* ptr = expression;

	/* NULL pointer checking*/
	if (!ptr) return 0;

	/* Spaces skipping before */
	while (isspace((unsigned char)*ptr))
		ptr++;

	/* The unary minus checking */
	int negative_flag = 0;
	if (*ptr == SUB)
	{
		if (isdigit((unsigned char)*++ptr))   // if the next character is a digit
			negative_flag = 1;
		else
			ptr--;
	}

	/* The return of the any not digit character */
	if (!isdigit((unsigned char)*ptr))
	{
		*end = ++ptr;
		return *(ptr - 1);
	}

	/* Making a float number and the delimiter processing */
	while (isdigit((unsigned char)*ptr) || IS_DELIM(*ptr))
	{
		if (*ptr == DELIM_COMMA)
			*ptr = DELIM_DOT; // for atof()

		if (negative_flag)
		{
			num[counter++] = SUB;
			negative_flag = 0; // in order to add the minus symbol one time in the head of array
		}

		num[counter++] = *ptr;
		ptr++;
	}

	num[counter] = '\0';
	*end = ptr; // pointer to the next character (to continue reading an operand from a new location)
	return VAL;
}

double useOperator(double leftval, double rightval, char oper)
{
	double result;
	switch (oper) {
	case ADD: result = leftval + rightval; break;
	case SUB: result = leftval - rightval; break;
	case MUL: result = leftval * rightval; break;
	case DIV: result = leftval / rightval; break;
	case POW: result = pow(leftval, rightval); break;
	default: exit(1);
	}

	return result;
}

CalcErrors transformToRPN(const char source[], char result[], int max_size)
{
	Stack* ops = createEmptyStack();
	char* ptr = source;
	int counter = 0;
	int unary_min = 0; // unary minus flag

	for (ptr; *ptr; ptr++) 
	{
		/* Avoiding going beyond the array */
		if (counter >= max_size) {
			deleteStack(ops);
			return INPUT_ERR;
		}

		/* Space skipping */
		while (isspace((unsigned char)*ptr))
			ptr++;
		
		/* Unary minus checking */
		if (*ptr == SUB)
		{ 
			if (*(ptr + 1) == '0') // prevent '-0'
				ptr++;
			if (unary_min || ptr == source) { // (ptr == source) => it is a array head
				result[counter++] = SUB;
				ptr++;
			}
		}

		/* Making a number */
		if (isdigit((unsigned char)*ptr))
		{
			while (isdigit((unsigned char)*ptr) || IS_DELIM(*ptr))
				result[counter++] = *ptr++;
			ptr--; // return the extra character
			result[counter++] = SPACE;
			unary_min = 0;
		}
		else if (IS_OPERATOR(*ptr))
		{
			unary_min = 1;
			if (!ops->size) // if stack is empty (to avoid an error after pop)
				push(ops, (double)(*ptr));
			else
			{
				if (PRIOR(*ptr) <= PRIOR((char)peek(ops))) // > if priority of new operator is higher than operator 
				{											  // in the top of stack, then the old operation will be
					result[counter++] = (char)pop(ops);		  // display and new operator push in the stack < 
					result[counter++] = SPACE;
				}

				push(ops, (double)(*ptr));
			}
		}
		else if (*ptr == OP_BRACE) {
			unary_min = 1;
			push(ops, (double)(*ptr));
		}
		else if (*ptr == CL_BRACE)		// if it's a closing parethesis, then it write all of operators before the
		{								// opening parethesis not including it
			char tmp;
			while ((tmp = (char)pop(ops)) != OP_BRACE) // until the opening parathesis
			{
				result[counter++] = tmp;
				result[counter++] = SPACE;
			}
		}
		else { // if it came across a unknown symbol
			deleteStack(ops);
			return INPUT_ERR;
		}

	}

	/* Pull out remaining operators */
	while (ops->size)
	{
		result[counter++] = (char)pop(ops);
		result[counter++] = SPACE;
	}
	result[counter++] = EQ;
	result[counter] = '\0';

	deleteStack(ops);
	return NO_ERR;
}


