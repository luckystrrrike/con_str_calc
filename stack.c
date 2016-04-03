#include "stack.h"
#include <string.h>

Stack* createEmptyStack()
{
	Stack* st = (Stack*)malloc(sizeof(Stack));
	st->data = (ValType*)malloc(sizeof(ValType)*ELEMENTARY_SIZE_STACK);
	st->max = ELEMENTARY_SIZE_STACK; 
	st->size = 0;
	return st;
}

void deleteStack(Stack* st)
{
	free(st->data);
	free(st);
	st = NULL;
}

static void resize(Stack* st)
{
	st->max += RESIZE_ADDING;
	st->data = realloc(st->data, st->max*sizeof(ValType));
}

void push(Stack* st, ValType val)
{
	if (st->size == st->max)
		resize(st);
	st->data[st->size++] = val;
}

ValType pop(Stack* st)
{
	if (!st->size) { // if there is a request to pop the value from empty stack
		printf("ERROR (FUNCTION POP): STACK SIZE < 0\n");
		exit(EXIT_FAILURE);
	}
	st->size--;
	return st->data[st->size];
}

/* In this function just look to the top of the stack */
ValType peek(Stack* st)
{
	if (!st->size) { // if there is a request to pop the value from empty stack
		printf("ERROR (FUNCTION PEEK): STACK SIZE < 0\n");
		exit(EXIT_FAILURE);
	}
	return st->data[st->size-1];
}

void printStack(Stack* st)
{
	int i;
	for (i = 0; i < st->size; i++)
		printf("%f ",st->data[i]);
}