/* 02.04.2016 */

#include "stack.h"
#include <stdio.h>
#include "calculating.h"
#include <errno.h>

#define EXPR_LENGTH 1000

main()
{
	while (1) 
	{
		Stack* nums = createEmptyStack();
		ValType lastval, prevval, result;
		char* temp = NULL, temp_char;
		char num_buf[NUM_LENGTH], expr_buf[EXPR_LENGTH];
		CalcErrors err_flag = NO_ERR;

		system("cls"); // clear the screen
		printf("Enter the expression: ");
		
		/* Transform the infix notation to the reverse Polish notation and processing the error if necessary */
		if (transformToRPN(expr_buf) != NO_ERR) {
			err_flag = WRONGSYM_ERR;
			goto break_loop;
		}

		temp = expr_buf; // to don't change the pointer expr_buf
		
		/* Parsing the operands from expression in the reverse Polish notation */
		while (temp_char = getOperand(num_buf, temp, &temp))
		{
			switch (temp_char) {
			/* Processing the operators */
			case SUB: case ADD: case MUL: case POW: case DIV:
				if (nums->size < 2) { // if operators are less than 2 (to avoid a pop() of empty stack)
					err_flag = LACK_OPERAND;
					goto break_loop; // break the nested loop
				}

				lastval = pop(nums); // last added operand
				prevval = pop(nums);

				result = useOperator(prevval, lastval, temp_char);

				if (errno || (!lastval && temp_char == DIV)) { // math errors (out of range, zero division etc)
					err_flag = MATH_ERR;
					goto break_loop; // break the nested loop
				}

				push(nums, result);
				break;

			/* Adding a value in the stack */
			case VAL:	push(nums, atof(num_buf));	break;
			/* End of input or other errors */
			case EQ:	err_flag = END_CALC;		goto break_loop;
			default:	err_flag = WRONGSYM_ERR;	goto break_loop;
			}
		}

	break_loop: // if input was incorrect, loop will be broken

		printf("\n");

		/* Processing the errors or displaying an answer */
		switch (err_flag) {
		case WRONGSYM_ERR:	printf(MSG_WRONGSYM); break;
		case MATH_ERR:	printf(MSG_MATH); break;
		case END_CALC:		nums->size == 1 ? printf("= %f\n", pop(nums)) : printf(MSG_LACKOP); break;
		case LACK_OPERAND:	printf(MSG_LACKOP); break; // FIX THE TEXT
		}

		printf("\nDo you want to calculate something else? y/n > ");

		fflush(stdin);
		temp_char = getchar();

		if (temp_char != 'y' && temp_char != 'Y')
			break;
	}
}