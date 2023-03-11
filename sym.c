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

nVal evaluate_node(const node *n)
{
   nVal leftValue;
   nVal rightValue;

   switch (n->kind) {
      case kNum: return n->e.number.value;
      case kSum: case kDiff: case kMult: case kDiv:
         leftValue = evaluate_node(n->e.binary.left);
         rightValue = evaluate_node(n->e.binary.right);

         printf("%lf, %lf\n", leftValue, rightValue);

      switch (n->kind) {
         case kSum: return leftValue + rightValue;
         case kDiff: return leftValue - rightValue;
         case kMult: return leftValue * rightValue;
         case kDiv: 
            if (rightValue == 0) // catch epsilon
               perror("division by zero");
            return leftValue / rightValue;
      }
      default: perror("internal error: illegal expression kind");        
  }  
}



void print_node(node *ptr) 
{
   node n = *ptr;
   switch (n.kind) {
      case kNum:
         nVal v = n.e.number.value;
         printf("kind: var ");
         printf("value: %lf\n", v);
         break;
      
      case kSum: case kDiff: case kMult: case kDiv:
         node *left = n.e.binary.left;
         node *right = n.e.binary.right;
         printf("kind: binary\n");
         print_node(left);
         print_node(right);
         break;
      
      case kSym:
         printf("kind: symbol");
         // getsym()
         break;

      default:
         break;
   }
}




// symrec *putsymlist (symrec *list, char *name, int sym_type) 
// {
//    printf("name: %s\n", name);
//    symrec *res = (symrec*) malloc (sizeof(symrec));
//    check_pointer(res);
//    res->name = strdup(name);
//    res->type = 0;
//    res->value.f = 0;
//    res->next = list;
//    list = res;
//    return res;
// }

symrec *addtolist(symrec *prev, const char *name, int sym_type)  
{
   symrec *res = (symrec *) malloc (sizeof(symrec));
   check_pointer(res);
   res->name = strdup(name);
   res->type = sym_type;
   res->value.f = 0;
   res->next = prev;
   return res;
}

symrec *getfromlist(symrec *start, const char *name)
{
   for (symrec *p = start; p; p = p->next)
		if (strcmp (p->name, name) == 0)
      	return p;
   
	return NULL;
}

int print_list(symrec *start) {
	for (symrec *p = start; p; p = p->next) {
     	printf("%p\t%s\t", p, p->name);

      switch (p->type) {
         case SYM_DEF:
            printf("type SYM_DEF\n");
            break;
         default: break;
      }
   }
}

int free_list(symrec *start) {
   for (symrec *p = start; p; p = p->next) {
      free(p);
   }
}


// symrec *putsym (char const *name, int sym_type)
// {
// 	symrec *res = (symrec *) malloc (sizeof (symrec));
// 	check_pointer(res);

// 	res->name = strdup (name);
// 	res->type = sym_type;
// 	res->value.f = 0; /* Set value to 0 even if fun. */
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


// struct call *new_call()
// {
// }