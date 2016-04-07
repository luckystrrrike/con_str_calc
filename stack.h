#include <stdlib.h>	

#define ELEMENTARY_SIZE_STACK 5 // initial the length of stack
#define RESIZE_ADDING 5 /* how much will be add when the stack size 
							will be equal to ELEMENTARY_SIZE */

typedef struct stack
{
	double* data;
	size_t max; // such value when the stack size should be extend
	size_t size;
} Stack;

Stack*		createEmptyStack(void);
void		push(Stack* s, double val);
double		pop(Stack* st);
double		peek(Stack* st);
void deleteStack(Stack* st);

#ifdef DEBUG
void		printStack(Stack* st);
#endif

#define MSG_MALLOC "ERROR! MALLOC RETURNS NULL!\n"
#define MSG_REALLOC "ERROR! REALLOC RETURN NULL!\n "