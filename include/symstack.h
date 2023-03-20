#ifndef _SYM_H
#define _SYM_H

#include <stdbool.h>


typedef struct _symrec symrec;
typedef struct _node node;


enum _kind { kNum, kSum, kDiff, kMult, kDiv, 

	kVar,
	kCall,

	kDef 
	};

typedef double nVal;

struct _number { nVal value; };
struct _binaryOperation { node *left, *right; };

struct _var { char* name; };
struct _call { char* name; symrec* args; };

struct _node {
	enum _kind kind;
	union _expression {
		struct _number number;
		struct _binaryOperation binary;
		struct _var var;
		struct _call call;
	} e;
};

node *newnode(node n);
void print_node(const node *n);
void print_node_wsym(const node *ptr, symrec *table);

nVal evaluate(const node *n, symrec *table);




// void fill(symrec *params, numrec *args);





// symbol lists

struct _symrec {
	enum _kind kind;
	char *name;
	union {
		nVal number;
		node* expr;
	} value;
	struct _symrec *next;
};

// symrec *newsym(char *name, nVal value);
// symrec *putsym(symrec* prev_top, symrec *top);
// void print_list(symrec *top);

symrec *newsym(symrec s);
symrec *putsym(symrec *prev_top, symrec *top);
void print_list(symrec *top);

// symrec *getsym(symrec *start, const char *name);
symrec *getsym(symrec *start, int kind, const char *name);




struct actrec {
	symrec *symbols;
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