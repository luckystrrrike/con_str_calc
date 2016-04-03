#include <float.h>

/* Errors processing */
typedef enum calcerr {
	NO_ERR,
	MATH_ERR,
	WRONGSYM_ERR,
	LACK_OPERAND,
	END_CALC
} CalcErrors;

#define MSG_WRONGSYM "ERROR! You entered the wrong symbol(s).\n"
#define MSG_LACKOP	"ERROR! The input format is not correct. \n"
#define MSG_MATH "ERROR! Math error! \n"

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
int			isOperator(char op);
static int	getPriority(char op);
CalcErrors	transformToRPN(char result[]); 

#define NUM_LENGTH DBL_DIG // maximum length of number