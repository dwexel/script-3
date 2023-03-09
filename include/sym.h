#pragma once

typedef struct _node node;
typedef enum _kind kind;

// expressions
// data types for abstract syntax tree
enum _kind { kVar, kConst, kSum, kDiff, kMult, kDiv, kPlus, kMinus, kNot };
struct _variable { int *memory; };
struct _unaryOperation { struct _node *right; };
struct _binaryOperation { struct _node *left, *right; };
struct _node {
  enum _kind kind;
  union _expression {
		struct _variable variable;
		struct _binaryOperation binary;
		struct _unaryOperation unary;
	} e;
};

node *newnode(node n);




// functions
typedef struct symrec symrec;
typedef struct proc proc;

/* Function type. */
// for builtin functions
typedef double (func_t) (double);

// for new functions
struct proc
{
	symrec *params;
};

/* Data type for links in the chain of symbols. */
struct symrec
{
	char *name;  /* name of symbol */
	int type;    /* type of symbol: either VAR or FUN */
	union
	{
		double  f;    /* value of a VAR */
		char    s[1000]; // string var
		proc *p;
		func_t *fun;   /* value of a FUN */

	} value;
	struct symrec *next;  /* link field */
};

/* The symbol table: a chain of 'struct symrec'. */
// extern symrec *sym_table;

// symrec *putsym (char const *name, int sym_type);
// symrec *getsym (char const *name);
// void putsym
// void newsym

int print_list(symrec *start);

extern symrec *globals;


enum sym_types
{
	SYM_FUNCTION,
	SYM_VARIABLE
};


// symbol types
// #define SYMBOL_FUN 1
// #define SYMBOL_VAR 2



