#include "symstack.h"


#define check_pointer(p) if (!p) {puts("Out of memory."); exit(EXIT_FAILURE);}

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>


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
         switch (n->kind) {
            case kSum: return leftValue + rightValue;
            case kDiff: return leftValue - rightValue;
            case kMult: return leftValue * rightValue;
            case kDiv: 
               if (rightValue == 0) // catch epsilon
                  perror("division by zero");
               return leftValue / rightValue;
         }
      default:
         perror("internal error: illegal expression kind"); 
   }  
}

nVal evaluate_node_wsym(const node *n, symrec *symbols)
{
   nVal leftValue;
   nVal rightValue;

   switch (n->kind) {
      case kNum: return n->e.number.value;
      case kSym:
         // printf("symbol: %s\n", n->e.symbol.name);
         symrec *sym = getfromlist(symbols, n->e.symbol.name);
         // printf("value: %lf\n", sym->value.f);
         return sym->value.f;

      case kSum: case kDiff: case kMult: case kDiv:
         leftValue = evaluate_node_wsym(n->e.binary.left, symbols);
         rightValue = evaluate_node_wsym(n->e.binary.right, symbols);
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

void print_node(const node *ptr) 
{
   node n = *ptr;
   switch (n.kind) {
      case kNum:
         nVal v = n.e.number.value;
         printf("kind: var, value: %lf\n", v);
         break;
      
      case kSum: case kDiff: case kMult: case kDiv:
         node *left = n.e.binary.left;
         node *right = n.e.binary.right;
         printf("kind: binary\n");
         print_node(left);
         print_node(right);
         break;
      
      case kSym:
         printf("kind: symbol\n");
         printf("id: %s\n", n.e.symbol.name);
         // symrec *sym = getfromlist(symbols, n->e.symbol.name);
         // printf("value: %lf\n", sym->value.f);
         break;

      case kCall:
         printf("kind: call\n");
         printf("name: %s\n", n.e.call.name);
         printf("args: \n");
         print_list(n.e.call.args);
         break;

      default:
         break;
   }
}

bool semantic_verify(const node *expr, symrec *args)
{
   // print_node(expr);
   nVal leftValue;
   nVal rightValue;

   switch (expr->kind) {
      case kNum: break;
      case kSum: case kDiff: case kMult: case kDiv:
         if (!semantic_verify(expr->e.binary.left, args)) return false;
         if (!semantic_verify(expr->e.binary.right, args)) return false;
         break;

      case kSym:
         // printf("symbol found\n");
         // printf("id: %s\n", expr->e.symbol.name);
         if (!getfromlist(args, expr->e.symbol.name)) {
            printf("error, var not in scope: %s\n", expr->e.symbol.name);
            return false;
         }
         break;
      
      default: 
         perror("unknown type\n");
         break;
   }
   return true;
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

symrec *newlist(const char *name)
{
   symrec *new = malloc(sizeof(symrec));
   check_pointer(new);
   new->name = strdup(name);
   new->value.f = 0;
   new->next = NULL;
   return new;
}


symrec *addtolist_end(symrec *start, const char *name)
{
   symrec *new = (symrec *) malloc (sizeof(symrec));
   check_pointer(new);
   new->name = strdup(name);
   new->value.f = 0;
   new->next = NULL;

   while (start->next) {
      start = start->next;
   }

   start->next = new;
   return new;
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
         default:
            printf("%lf", p->value.f);
            break;
      }
      printf("\n");
   }
}

// int free_list(symrec *start) {
//    symrec *next;
//    for (symrec *p = start; p; p = next) {
//       next = p->next;
//       free(p);
//    }
// }

numrec *newlist_num(nVal val)
{
   numrec *new = malloc(sizeof(numrec));
   check_pointer(new);
   new->val = val;
   new->next = NULL;
   return new;
}

numrec *addtolist_end_num(numrec *start, nVal val)
{
   numrec *new = malloc(sizeof(numrec));
   check_pointer(new);
   new->val = val;
   new->next = NULL;

   while (start->next) {
      start = start->next;
   }
   start->next = new;
   return new;
}

int print_list_num(numrec *start)
{
	for (numrec *p = start; p; p = p->next) {
     	printf("%p\t%lf\n", p, p->val);
   }
}

void fill(symrec *params, numrec *args)
{
   symrec *a = params;
   numrec *b = args;

   while (1) {
      a->value.f = b->val;
      a = a->next;
      b = b->next;

      if (!a) {
         puts("a ends");
         break;
      }
      if (!b) {
         perror("b ends\n");
         break;
      }
   }  
}

// numrec *num_addtolist(numrec *prev, nVal val)
// {
//    numrec *num = malloc(sizeof(numrec));
//    check_pointer(num);
//    num->val = val;
//    num->next = prev;
//    return num;
// }

// void print_num_list(numrec *top)
// {
//    printf("printing num list:\n");
//    for (numrec *p = top; p; p = p->next)
//       printf("val: %lf\n", p->val);
// }

// numrec *num_getfromlist(numrec *top)
// {
//    for (numrec *p = top; p; p = p->next)
// 		if (strcmp (p->name, name) == 0)
//       	return p;
// 	return NULL;
// }

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



/* Minimal stack size (expressed in number of elements) for which space is allocated. It should be at least 1. */
#define MINIMUM_SIZE 1
/* How much more memory is allocated each time a stack grows out of its allocated segment. */
#define GROWTH_FACTOR 2


struct stack{
   T *bottom;
   T *top;
   T *allocated_top;
};

/* Creates a new stack. */
stack *new_stack(void)
{
   stack *s = malloc(sizeof(stack)); 
   check_pointer(s);
   s->bottom = malloc(MINIMUM_SIZE * sizeof(T));
   check_pointer(s->bottom);
   s->top = s->bottom - 1; 
   s->allocated_top = s->bottom + MINIMUM_SIZE - 1; 
   return s;
}

/* Frees all the memory used for a stack. */
void destroy(stack *s)
{
   free(s->bottom);
   free(s);
}

/* Returns true if there are no elements on the stack. This is different from the stack not having 
enough memory reserved for even one element, which case is never allowed to arise. */
bool empty(stack *s)
{
   return s->top < s->bottom ? true : false;
}

/* Puts a new element on the stack, enlarging the latter's memory allowance if necessary. */
void push(stack *s, T x)
{
   if (s->top == s->allocated_top)
   {
      ptrdiff_t qtty = s->top - s->bottom + 1;
      ptrdiff_t new_qtty = GROWTH_FACTOR * qtty;
      s->bottom = realloc(s->bottom, new_qtty * sizeof(T));
      check_pointer(s->bottom);
      s->top = s->bottom + qtty - 1;
      s->allocated_top = s->bottom + new_qtty - 1;
   }
   // increment the stack pointer, and set the "top" member to x
   *(++s->top) = x;
}

T pop(stack *s)
{
   return *(s->top--);
}

T peek(stack *s)
{
   return *(s->top);
}


/* Frees any memory the stack isn't actually using. The allocated portion still isn't allowed to shrink
smaller than MINIMUM_SIZE. If all the stack's memory is in use, nothing happens. */
void compress(stack *s)
{
   if (s->top == s->allocated_top) return;
   ptrdiff_t qtty = s->top - s->bottom + 1;
   if (qtty < MINIMUM_SIZE) qtty = MINIMUM_SIZE;
   size_t new_size = qtty * sizeof(T);
   s->bottom = realloc(s->bottom, new_size);
   check_pointer(s->bottom);
   s->allocated_top = s->bottom + qtty - 1;
}

// callstack
// specific features here!!!!!
stack *call_stack;
void push_call(symrec *args)
{
   T a = { args };
   push(call_stack, a);
}

void print_stack()
{
   printf("\nemptying callstack\n");
   int i = 0;
   while (!empty(call_stack)) {
      T call = pop(call_stack);
      printf("call %i\n", i);
      print_list(call.args);
      i++;
      // printf("call name: %s\n", call.name);
   }
}