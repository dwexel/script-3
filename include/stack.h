#pragma once

struct activation_record
{
	char *name;
};

// call stack type
typedef struct activation_record T;

// stack definition 
typedef struct stack stack;

// stack functions
stack* new_stack(void);
void push(stack *s, T x);
T pop(stack *s);
T peek(stack *s);
bool empty(stack *s);
void destroy(stack *s);
void compress(stack *s);



