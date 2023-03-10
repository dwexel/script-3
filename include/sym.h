#ifndef _SYM_H
#define _SYM_H

// nodes

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

typedef struct symrec symrec;
struct symrec {
	char *name;
	int type;
	union	{
		double f;
		char *c;
	} value;
	struct symrec *next;
};

// symrec *putsym (char const *name, int sym_type);
// symrec *getsym (char const *name);
// void putsym
// void newsym

symrec *putsymlist (symrec *list, char *name, int sym_type);

symrec *addtolist(symrec *prev, char *name, int sym_type);

int print_list(symrec *start);

extern symrec *globals;






#endif