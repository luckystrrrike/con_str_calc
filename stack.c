#include <string.h>
#include "stack.h"

static void resize(Stack* st);

Stack* createEmptyStack(void)
{
	Stack* st = malloc(sizeof(Stack));
	/* Allocate error checking */
	if (st == NULL) {
		printf(MSG_MALLOC);
		exit(EXIT_FAILURE);
	}

	st->data = malloc(sizeof(double)*ELEMENTARY_SIZE_STACK);
	/* Allocate error checking */
	if (st->data == NULL) {
		printf(MSG_MALLOC);
		exit(EXIT_FAILURE);
	}

	st->max = ELEMENTARY_SIZE_STACK; 
	st->size = 0;
	return st;
}

void deleteStack(Stack* st)
{
	free(st->data);
	free(st);
}

static void resize(Stack* st)
{
	st->max += RESIZE_ADDING;
	st->data = realloc(st->data, st->max*sizeof(double));
	if (st->data == NULL) {
		printf(MSG_REALLOC);
		exit(EXIT_FAILURE);
	}
}

void push(Stack* st, double val)
{
	if (st->size == st->max)
		resize(st);
	st->data[st->size++] = val;
}

double pop(Stack* st)
{
	if (!st->size) { // if there is a request to pop the value from empty stack
		printf("ERROR (FUNCTION POP): STACK SIZE < 0\n");
		exit(EXIT_FAILURE);
	}
	st->size--;
	return st->data[st->size];
}

/* In this function just look to the top of the stack */
double peek(Stack* st)
{
	if (!st->size) { // if there is a request to pop the value from empty stack
		printf("ERROR (FUNCTION PEEK): STACK SIZE < 0\n");
		exit(EXIT_FAILURE);
	}
	return st->data[st->size-1];
}

#ifdef DEBUG
void printStack(Stack* st)
{
	int i;
	for (i = 0; i < st->size; i++)
		printf("%lf",st->data[i]);
	printf("\n");
}
#endif