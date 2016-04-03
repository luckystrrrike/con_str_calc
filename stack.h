#define ELEMENTARY_SIZE_STACK 5 // initial the length of stack
#define RESIZE_ADDING 5 /* how much will be add when the stack size 
							will be equal to ELEMENTARY_SIZE */
#include <stdlib.h>	

#define ValType double

typedef struct stack
{
	ValType* data;
	size_t max; // such value when the stack size should be extend
	size_t size;
} Stack;

Stack*		createEmptyStack();
void		push(Stack* s, ValType val);
void		deleteStack(Stack* st);
static void resize(Stack* st);
ValType		pop(Stack* st);
ValType		peek(Stack* st);
void		printStack(Stack* st);

#define DEBUG_PRINT_STACK(ST) printStack(ST); printf("\n");