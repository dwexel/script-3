#include "sym.h"

// for reference
// symrec *sym_table;

#define check_pointer(p) if (!p) {puts("Out of memory."); exit(EXIT_FAILURE);}


/* The mfcalc code assumes that malloc and realloc
   always succeed, and that integer calculations
   never overflow.  Production-quality code should
   not make these assumptions.  */
#include <assert.h>
#include <stdlib.h> /* malloc, realloc. */
#include <string.h> /* strlen. */
#include <stdio.h>


node *newnode(node n)
{
   node *p = malloc(sizeof(node));
   check_pointer(p);
   *p = n;
   return p;
}




int print_list(symrec *start) {
	for (symrec *p = start; p; p = p->next) {
   	printf("%p\t%s\t%d\n", p, p->name, p->type);
   }
}


// symrec *putsym (symrec *list, char const *name, int sym_type)
// {
//    symrec *res = (symrec *) malloc (sizeof (symrec));
//    check_pointer(res);
   
//    res->name = strdup (name);
//    res->type = sym_type;
// }

// symrec *putsym (char const *name, int sym_type)
// {
// 	symrec *res = (symrec *) malloc (sizeof (symrec));
// 	check_pointer(res);

// 	res->name = strdup (name);
// 	res->type = sym_type;
// 	res->value.f = 0; /* Set value to 0 even if fun. */
// 	// it's a union dummy

// 	res->next = sym_table;
// 	sym_table = res;
// 	return res;
// }

// symrec *getsym (char const *name)
// {
// 	for (symrec *p = sym_table; p; p = p->next)
// 		if (strcmp (p->name, name) == 0)
// 			return p;
// 	return NULL;
// }

// int main()
// {
// 	init_table();
// 	printf("%p\n\n", getsym("sin"));

// 	// sym_table is always set to the latest entry
// 	for (symrec *p = sym_table; p; p = p->next)
// 		printf("%p   %s   %d\n", p, p->name, p->type);
// 	return 0;
// }


// reusable linked lists