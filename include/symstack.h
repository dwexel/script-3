#ifndef _SYM_H
#define _SYM_H

#include <stdbool.h>

typedef struct _node node;

enum _kind { 
	kNum,

	kSum, 
	kDiff, 
	kMult, 
	kDiv,
	kPow,

	kVar,

	kCall,
	kDef,
	kArg

	};

typedef double nVal;

struct _number { nVal value; };
struct _binary { node *left, *right; };
struct _id { char* name; };
struct _link { char* name; node* expr; };
struct _li { char* name; node* expr; node* next; };

struct _node {
	enum _kind kind;
	union _expression {
		struct _number number;
		struct _binary binary;
		struct _id id;
		struct _link link;
		struct _li li;
	} e;
};

node *newnode(node n);

void print_node(const node *n);

nVal evaluate(const node *expression, const node *lookup);







// symbol lists

// enum symrec_kind {
// 	kDef,
// 	kState,
// };

// struct _symrec {
// 	enum symrec_kind kind;
// 	char *name;
// 	node *expr;
// 	symrec *next;
// };


// symrec *newsym(symrec s);
// symrec *putsym(symrec *prev_top, symrec *top);
// symrec *getsym(symrec *start, int kind, const char *name);
// symrec *getsym(symrec *start, const char *name);

// void print_list(symrec *top);







struct actrec {
	// symrec *symbols;
	node *expr;
};

typedef struct actrec T;
typedef struct stack stack;

/* Minimal stack size (expressed in number of elements) for which space is allocated. It should be at least 1. */
#define MINIMUM_SIZE 1
/* How much more memory is allocated each time a stack grows out of its allocated segment. */
#define GROWTH_FACTOR 2

struct stack {
   T *bottom;
   T *top;
   T *allocated_top;
};


// stack functions
stack* new_stack(void);
void push(stack *s, T x);
T pop(stack *s);
T peek(stack *s);
bool empty(stack *s);
void destroy(stack *s);
void compress(stack *s);




#endif