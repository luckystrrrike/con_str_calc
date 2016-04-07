#include <float.h>

/* Errors processing */
typedef enum calcerr {
	NO_ERR,
	MATH_ERR,
	INPUT_ERR,
	END_CALC,
	UNKNOWN_ERR
} CalcErrors;

#define MSG_INPUTERR "ERROR! The input format is not correct. Try again.\n"
#define MSG_MATH	 "ERROR! Math error! Try again.\n"
#define MSG_UNKN	 "UNKNOWN ERROR! Try again.\n"

/* Operators, delimiters, other characters */
#define VAL 1
#define SUB '-'
#define ADD '+'
#define MUL '*'
#define DIV '/'
#define EQ '='
#define POW '^'
#define OP_BRACE '('
#define CL_BRACE ')'

#define IS_OPERATOR(x) ((x) == SUB || (x) == ADD || (x) == MUL || (x) == DIV || (x) == POW)

#define DELIM_DOT '.'
#define DELIM_COMMA ','
#define IS_DELIM(x) ((x) == DELIM_DOT || (x) == DELIM_COMMA)

#define SPACE ' '

/* Priority of operators */
#define PRIOR_SUB 1                 
#define PRIOR_ADD 1
#define PRIOR_MUL 2
#define PRIOR_DIV 2
#define PRIOR_POW 3
#define PRIOR_OP_BR 0
#define PRIOR_CL_BR 0

#define PRIOR(x) getPriority(x)

/*Calculation and parsing expressions on the RPN (Reverse Polish Notation) */
int		getOperand(char* num, char expression[], char** end);
double	useOperator(double leftval, double rightval, char oper);

/*transform the infix notation to the reverse Polish notation*/
CalcErrors	transformToRPN(const char source[], char result[], int max_size);

#define NUM_LENGTH DBL_DIG // maximum length of number