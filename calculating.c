#include "calculating.h"
#include "stack.h"
#include <stdio.h>
#include <math.h>

/* Function for working with an expression on the RPN */

int getOperand(char* num, char expression[], char** end)
{
	unsigned counter = 0;
	char* ptr = expression;

	/* NULL pointer and \0 checking*/
	if (!ptr) return 0;

	/* Spaces skipping before */
	while (isspace(*ptr))
		ptr++;

	/* The unary minus checking */
	int negative_flag = 0;
	if (*ptr == SUB)
	{
		if (isdigit(*++ptr))   // if the next character is a digit
			negative_flag = 1;
		else
			ptr--;
	}

	/* The return of the any not digit character */
	if (!isdigit(*ptr))
	{
		*end = ++ptr;
		return *(ptr - 1);
	}

	/* Making a float number and the delimiter processing */
	while (isdigit(*ptr) || IS_DELIM(*ptr))
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

/* Function for working with an expression on the infix notation */

int getPriority(char ch) {
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

int isOperator(char x)
{
	return (x == SUB || x == ADD || 
			x == MUL || x == DIV || 
			x == POW);
}

CalcErrors transformToRPN(char result[])
{
	Stack* ops = createEmptyStack();
	char temp_ch;
	unsigned counter = 0;

	while ((temp_ch = getchar())) 
	{ 
		/* Spaces skipping before */
		while (isspace(temp_ch))  
			temp_ch = getchar();

		/* Ending the input, fully write the remaining contents of stack*/
		if (temp_ch == EQ) 
		{
			while (ops->size)
			{ 
				result[counter++] = (char)pop(ops);
				result[counter++] = SPACE;
			}
			result[counter++] = EQ;
			result[counter] = '\0';
			return NO_ERR;
		}

		/* Cheking the unary minus, and if it's not unary, return character after minus used to check back 
		and process a minus as an operator*/
		if (temp_ch == SUB)
		{
			temp_ch = getchar(); // read next symbol and if the next character is a digit
			if (isdigit(temp_ch)) { 
				if (temp_ch != '0') // to don't allow the '-0'
					result[counter++] = SUB;
			}
			else {
				ungetc(temp_ch, stdin);
				temp_ch = SUB;
			}
		}

		/* Making a number */
		if (isdigit(temp_ch))
		{
			while (isdigit(temp_ch) || IS_DELIM(temp_ch) )
			{ 
				result[counter++] = temp_ch;
				temp_ch = getchar();
			}	
			ungetc(temp_ch, stdin); // return the extra character
			result[counter++] = SPACE;
		}

		/* Else check the operator and push it to the stack */
		else 
		{
			if (isOperator(temp_ch)) 
			{
				if (!ops->size) // if stack is empty (to avoid an error after pop)
					push(ops, (double)temp_ch);
				else 
				{
					if (PRIOR(temp_ch) <= PRIOR((char)peek(ops))) // > if priority of new operator is higher than operator 
					{											  // in the top of stack, then the old operation will be
						result[counter++] = (char)pop(ops);		  // display and new operator push in the stack < 
						result[counter++] = SPACE;
					}

					push(ops, (double)temp_ch);
				}
			}
			/* Operators inside parathesises processing */
			else if (temp_ch == OP_BRACE)
				push(ops, (double)temp_ch);
			else if (temp_ch == CL_BRACE)  // if it's a closing parethesis, then it write all of operators before the
			{							   // opening parethesis not including it
				char tmp;
				while ((tmp = (char)pop(ops)) != OP_BRACE) // until the opening parathesis
				{
					result[counter++] = tmp;
					result[counter++] = SPACE;
				}
			}
			/* Any other symbols */
			else  
				return WRONGSYM_ERR;
		}	
	}
}
