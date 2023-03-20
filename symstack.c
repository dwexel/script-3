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

// nVal evaluate_node(const node *n)
// {
//    nVal leftValue;
//    nVal rightValue;

//    switch (n->kind) {
//       case kNum: return n->e.number.value;
//       case kSum: case kDiff: case kMult: case kDiv:
//          leftValue = evaluate_node(n->e.binary.left);
//          rightValue = evaluate_node(n->e.binary.right);
//          switch (n->kind) {
//             case kSum: return leftValue + rightValue;
//             case kDiff: return leftValue - rightValue;
//             case kMult: return leftValue * rightValue;
//             case kDiv: 
//                if (rightValue == 0) // catch epsilon
//                   perror("division by zero");
//                return leftValue / rightValue;
//          }
//       default:
//          perror("internal error: illegal expression kind"); 
//    }  
// }

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
      
      case kVar:
         printf("kind: var\n");
         printf("id: %s\n", n.e.var.name);
         break;

      case kCall:
         printf("kind: call\n");
         break;

      default:
         break;
   }
}

void print_node_wsym(const node *ptr, symrec *table)
{
   node n = *ptr;
   switch (n.kind) {
      case kNum:
         nVal v = n.e.number.value;
         printf("kind: number, value: %lf\n", v);
         break;
      
      case kSum: case kDiff: case kMult: case kDiv:
         node *left = n.e.binary.left;
         node *right = n.e.binary.right;
         printf("kind: binary\n");
         print_node(left);
         print_node(right);
         break;
      
      case kVar: case kCall:
         symrec *s = NULL;
         switch (n.kind) {
            case kVar:
               printf("kind: var");
               s = getsym(table, n.e.var.name);
               break;

            case kCall:
               printf("kind: call");
               s = getsym(table, n.e.call.name);
               
               break;
         }

         if (!s) {
            printf("error: symbol could not be found\n");
         }

         break;

      // case kSym:
      //    printf("kind: symbol\n");
      //    printf("id: %s\n", n.e.symbol.name);
         // printf("")
         // break;

      // case kCall:
      //    printf("kind: call");
      //    symrec *fn = getsym(table, n.e.call.name);
      //    printf("ref: %p\n", fn);
         
         // recurse

      default:
         break;
   }
}

// bool semantic_verify(const node *expr, symrec *args)
// {
//    // print_node(expr);
//    nVal leftValue;
//    nVal rightValue;

//    switch (expr->kind) {
//       case kNum: break;
//       case kSum: case kDiff: case kMult: case kDiv:
//          if (!semantic_verify(expr->e.binary.left, args)) return false;
//          if (!semantic_verify(expr->e.binary.right, args)) return false;
//          break;

//       case kSym:
//          // printf("symbol found\n");
//          // printf("id: %s\n", expr->e.symbol.name);
//          if (!getfromlist(args, expr->e.symbol.name)) {
//             printf("error, var not in scope: %s\n", expr->e.symbol.name);
//             return false;
//          }
//          break;
      
//       default: 
//          perror("unknown type\n");
//          break;
//    }
//    return true;
// }

// symrec *addtolist_end(symrec *start, const char *name)
// {
//    symrec *new = (symrec *) malloc (sizeof(symrec));
//    check_pointer(new);
//    new->name = strdup(name);
//    new->value.f = 0;
//    new->next = NULL;

//    while (start->next) {
//       start = start->next;
//    }

//    start->next = new;
//    return new;
// }

// int free_list(symrec *start) {
//    symrec *next;
//    for (symrec *p = start; p; p = next) {
//       next = p->next;
//       free(p);
//    }
// }


symrec *newsym(symrec s)
{
   symrec *p = malloc(sizeof(symrec));
   check_pointer(p);
   *p = s;
   return p;
}

symrec *putsym(symrec *prev_top, symrec *top)
{
   top->next = prev_top;
   return top;
}

void print_list(symrec *top)
{
	for (symrec *p = top; p; p = p->next)
   {
     	printf("%p\t%s\t", p, p->name);
      switch (p->kind)
      {
         case kNum:
            printf("num\t%g\n", p->value.number);
            break;

         case kDef:
            printf("def\n");
            break;

         default:
            break;
      }
   }
}

symrec *getsym(symrec *start, const char *name)
{
   for (symrec *p = start; p; p = p->next)
   {
		if (strcmp (p->name, name) == 0)
      {
         return p;
      }  
   }
	return NULL;
}


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
void destroy(stack *s) {
   free(s->bottom);
   free(s);
}

/* Returns true if there are no elements on the stack. This is different from the stack not having 
enough memory reserved for even one element, which case is never allowed to arise. */
bool empty(stack *s) {
   return s->top < s->bottom ? true : false;
}

/* Puts a new element on the stack, enlarging the latter's memory allowance if necessary. */
void push(stack *s, T x) {
   if (s->top == s->allocated_top) {
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

T pop(stack *s) {
   return *(s->top--);
}

T peek(stack *s) {
   return *(s->top);
}


/* Frees any memory the stack isn't actually using. The allocated portion still isn't allowed to shrink
smaller than MINIMUM_SIZE. If all the stack's memory is in use, nothing happens. */
void compress(stack *s)
{
   if (s->top == s->allocated_top) 
      return;

   ptrdiff_t qtty = s->top - s->bottom + 1;
   if (qtty < MINIMUM_SIZE) 
      qtty = MINIMUM_SIZE;
   
   size_t new_size = qtty * sizeof(T);
   s->bottom = realloc(s->bottom, new_size);
   check_pointer(s->bottom);
   s->allocated_top = s->bottom + qtty - 1;
}

// callstack specific features here!!!!!

// stack *call_stack;

// void push_call(symrec *args)
// {
//    T a = { args };
//    push(call_stack, a);
// }

// int push_call(node *call)
// {
//    // T frame = {  }
// }

// execute
// void execute(struct actrec *scope)
// {
// }

// void print_stack()
// {
//    printf("\nemptying callstack\n");
//    for (int i = 0; !empty(call_stack); i++)
//    {
//       T call = pop(call_stack);
//       printf("call %i\n", i);
//    }
// }