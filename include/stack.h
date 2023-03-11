#pragma once

// call stack type
struct actrec {
	char name[100];
};
typedef struct actrec T;
typedef struct stack stack;

// stack functions
stack* new_stack(void);
void push(stack *s, T x);
T pop(stack *s);
T peek(stack *s);
bool empty(stack *s);
void destroy(stack *s);
void compress(stack *s);


// extern stack* call_stack;




