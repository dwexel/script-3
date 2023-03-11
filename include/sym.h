#ifndef _SYM_H
#define _SYM_H

// nodes for expressions

typedef struct _node node;
typedef double nVal;

enum _kind { kNum, kSum, kDiff, kMult, kDiv, kSym };
struct _number { nVal value; };
struct _binaryOperation { struct _node *left, *right; };
struct _symbol { char* name; };

struct _node {
	enum _kind kind;
	union _expression {
		struct _number number;
		struct _binaryOperation binary;
		struct _symbol symbol;
	} e;
};

node *newnode(node n);
void print_node(node *n);
nVal evaluate_node(const node *n);

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

#define SYM_DEF 1
#define SYM_CALL 2
#define SYM_PARAM 3


typedef struct symrec symrec;

struct symrec {
	char *name;
	int type;
	union	{
		double f;
		char *c;

		node *exprpointer;
	} value;
	struct symrec *next;
};

// symrec *putsym (char const *name, int sym_type);
// symrec *getsym (char const *name);
// void putsym
// void newsym

// symrec *putsymlist (symrec *list, char *name, int sym_type);

symrec *addtolist(symrec *prev, const char *name, int sym_type);
symrec *getfromlist(symrec *start, const char *name);



int print_list(symrec *start);




// struct call {
// 	char *name;
// 	symrec *args;
// };

// struct def {
// 	char *name;
// 	symrec *params;
// };





#endif