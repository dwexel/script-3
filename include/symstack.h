#ifndef _SYM_H
#define _SYM_H

#include <stdbool.h>


typedef struct symrec symrec;


// nodes for expressions
typedef struct _node node;
typedef double nVal;

// symbol = var
// call = call
enum _kind { kNum, kSum, kDiff, kMult, kDiv, kSym, kCall };
struct _number { nVal value; };
struct _binaryOperation { struct _node *left, *right; };

struct _symbol { char* name; };
struct _functionCall { char* name; symrec* args; };

struct _node {
	enum _kind kind;
	union _expression {
		struct _number number;
		struct _binaryOperation binary;
		struct _symbol symbol;
		struct _functionCall call;
	} e;
};

node *newnode(node n);
void print_node(const node *n);
nVal evaluate_node(const node *n);

bool semantic_verify(const node *expr, symrec *args);
nVal evaluate_node_wsym(const node *n, symrec *symbols);


// nonymous
// enum _type { number, function };
// struct _symbol {
// 	char *name;
// 	enum _type type;
// 	union _value {
// 		double num;
// 		int this_is_a_function;
// 	} v;
// };



// sym types

#define SYMBOL_NUMBER 1
#define SYMBOL_ID 2
#define SYMBOL_DEF 3

struct def {
	symrec *params;
	node *expr;
};

struct symrec {
	char *name;
	int type;
	union	{
		double f;
		struct def *fn;
	} value;
	struct symrec *next;
};

// symrec *putsym (char const *name, int sym_type);
// symrec *getsym (char const *name);

symrec *addtolist(symrec *prev, const char *name, int sym_type);
symrec *getfromlist(symrec *start, const char *name);
int print_list(symrec *start);

symrec *newlist(const char *name);
symrec *addtolist_end(symrec *start, const char *name);

// number lists
typedef struct _numrec numrec;
struct _numrec {
	nVal val;
	numrec *next;
};

numrec *newlist_num(nVal val);
numrec *addtolist_end_num(numrec *start, nVal val);
int print_list_num(numrec *start);

//
//

void fill(symrec *params, numrec *args);




struct actrec {
	// char *name;
	// struct actrec *prev;
	symrec *args;
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



// call stack functions
extern stack *call_stack;
void push_call(symrec *args);
void print_stack();



#endif