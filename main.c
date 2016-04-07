#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include "calculating.h"
#include "stack.h"

#define EXPR_LENGTH 1000

int main()
{
	printf("strcalc 1.0 by luckystrrrike on April 6, 2016\n");
	printf("\n\nType your expression ('quit' to exit) and press Enter.\n");

	while (1) 
	{
		Stack* nums = createEmptyStack();
		char* temp = NULL;
		int temp_char;
		double lastval, prevval, result;
		char expr_buf[EXPR_LENGTH], rpn_expr_buf[EXPR_LENGTH], num_buf[NUM_LENGTH];
		CalcErrors err_flag = NO_ERR;

		printf("\n> ");

		if (!gets_s(expr_buf, EXPR_LENGTH)) {
			err_flag = INPUT_ERR;
			goto results;
		}

		if (!strcmp(expr_buf, "quit")) 
			break;
		
		if (transformToRPN(expr_buf, rpn_expr_buf, EXPR_LENGTH) != NO_ERR) {
			err_flag = INPUT_ERR;
			goto results;
		}
		
		temp = rpn_expr_buf;

		/* Parsing the operands from expression in the reverse Polish notation */
		while (temp_char = getOperand(num_buf, temp, &temp))
		{
			switch (temp_char) {
				/* Processing the operators */
			case SUB: case ADD: case MUL: case POW: case DIV:
				if (nums->size < 2) { // if operators are less than 2 (to avoid a pop() of empty stack)
					err_flag = INPUT_ERR;
					goto results; // break the nested loop
				}

				lastval = pop(nums); // last added operand
				prevval = pop(nums);

				result = useOperator(prevval, lastval, temp_char);

				if (errno || (!lastval && temp_char == DIV)) { // math errors (out of range, zero division etc)
					err_flag = MATH_ERR;
					errno = 0;
					goto results; // break the nested loop
				}

				push(nums, result);
				break;

				/* Adding a value in the stack */
			case VAL:   push(nums, atof(num_buf));  break;
				/* End of input or other errors */
			case EQ:    err_flag = END_CALC;  goto results;
			default:    err_flag = INPUT_ERR; goto results;
			}
		}

	results:
		switch (err_flag) {
		case MATH_ERR:      printf(MSG_MATH); break;
		case END_CALC:		nums->size == 1 ? printf("= %g\n", pop(nums)) : printf(MSG_INPUTERR); break;
		case INPUT_ERR:		printf(MSG_INPUTERR); break;
		case UNKNOWN_ERR:	printf(MSG_UNKN);
		}
		deleteStack(nums);
	}
	return EXIT_SUCCESS;
}