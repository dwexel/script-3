#include "symstack.h"


#define check_pointer(p) if (!p) {puts("Out of memory."); exit(EXIT_FAILURE);}

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>



node *newnode(node n) 
{
	node *p = malloc(sizeof(node));
	check_pointer(p);
	*p = n;
	return p;
}

void print_node(const node *ptr)
{
	node n = *ptr;
	switch (n.kind) {
		case kNum:
			printf("number:\t");
			printf("%lf\n", n.e.number.value);
			break;
		
		case kSum: case kDiff: case kMult: case kDiv:
			printf("binary:\t");
			switch (n.kind) {
				case kSum: printf("sum\n"); break;
				case kDiff: printf("diff\n"); break;
				case kMult: printf("mult\n"); break;
				case kDiv: printf("div\n"); break;
			}
			node *left = n.e.binary.left;
			node *right = n.e.binary.right;
			print_node(left);
			print_node(right);
			break;
		
		case kVar:
			printf("var:\t");
			printf("%s\n", n.e.id.name);
			break;
		
		case kCall:
			printf("call:\t");
			printf("%s\n", n.e.link.name);
			print_node(n.e.link.expr);
			break;
		
		case kArg: case kDef:
			switch (n.kind){
				case kArg: printf("arg:\t"); break;
				case kDef: printf("def:\t"); break;
			}
			printf("%s\n", n.e.li.name);
			print_node(n.e.li.expr);
			if (n.e.li.next) print_node(n.e.li.next);
			break;
		
		default:
			printf("error: unknown node type ");
			printf("%d\n", n.kind);
			break;
	}
}

node *lookup(const char* name, const node *table, enum _kind kind)
{
	for (node *p = table; p; p = p->e.li.next)
	{
		if (kind == p->kind)
		{
			if (strcmp(name, p->e.li.name) == 0)
			{
				return p;
			}
		}
	}
	return NULL;
}


nVal evaluate(const node *expression, const node *table)
{
	node n = *expression;
	nVal leftValue;
	nVal rightValue;

	switch (n.kind) {
		case kNum:
			return n.e.number.value;
			break;

      case kSum: case kDiff: case kMult: case kDiv: case kPow:
         leftValue = evaluate(n.e.binary.left, table);
         rightValue = evaluate(n.e.binary.right, table);
         switch (n.kind) {

            case kSum: return leftValue + rightValue;
            case kDiff: return leftValue - rightValue;
            case kMult: return leftValue * rightValue;
            case kDiv:
               if (rightValue == 0) perror("division by zero");
               return leftValue / rightValue;
				case kPow: return pow(leftValue, rightValue);
         }
			break;

		case kVar:
			// print_node(table);
			node *arg = lookup(n.e.id.name, table, kArg);
			if (arg) return evaluate(arg->e.li.expr, table);
			break;

		case kCall:
			node *def = lookup(n.e.link.name, table, kDef);
			if (def) return evaluate(def->e.li.expr, n.e.link.expr);
			break;

		default:
			printf("error %d\n", n.kind);
			break;
	}
}


// {
//    nVal leftValue;
//    nVal rightValue;
//    switch (n->kind) {
//       case kNum: return n->e.number.value;
//       case kSum: case kDiff: case kMult: case kDiv:
//          leftValue = evaluate(n->e.binary.left, table);
//          rightValue = evaluate(n->e.binary.right, table);
//          switch (n->kind) {
//             case kSum: return leftValue + rightValue;
//             case kDiff: return leftValue - rightValue;
//             case kMult: return leftValue * rightValue;
//             case kDiv: 
//                if (rightValue == 0) {
//                   // catch epsilon
//                   perror("division by zero");
//                }
//                return leftValue / rightValue;
//          }
//       case kVar: case kCall:
//          symrec *sym = NULL;
//          switch (n->kind) {
//             case kVar:
//             sym = getsym(table, kVar, n->e.var.name);
//             if (sym) return sym->value.number;
//             case kCall:
//             sym = getsym(table, kDef, n->e.call.name);
//             if (sym) return evaluate(sym->value.expr, n->e.call.args);
//          }
//          if (!sym) printf("runtime error, symbol could not be found\n");
//          break;

//       default:
//          perror("internal error: illegal expression kind"); 
//    } 
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


// symrec *newsym(symrec s)
// {
// 	symrec *p = malloc(sizeof(symrec));
// 	check_pointer(p);
// 	*p = s;
// 	return p;
// }

// symrec *putsym(symrec *prev_top, symrec *top)
// {
// 	top->next = prev_top;
// 	return top;
// }

// void print_list(symrec *top)
// {
// 	for (symrec *p = top; p; p = p->next)
// 	{
// 	  	printf("%p\t%s\t", p, p->name);
// 		switch (p->kind)
// 		{
// 			case kDef: 
// 				printf("def\n");
// 				break;
// 			case kState: 
// 				printf("statement\t");
// 				print_node(p->expr);
// 				// printf(p->value)
// 				break;
// 			default: 
// 				break;
// 		}
// 	}
// }

// symrec *getsym(symrec *start, const char *name)
// {
// 	for (symrec *p = start; p; p = p->next)
// 	{
// 		// printf("\nkind test: %i\t%i\n", p->kind, kind);
// 		// printf("%s\n", p->name);
// 		if (strcmp (p->name, name) == 0) 
// 		{
// 			return p;
// 		}
// 	}
// 	return NULL;
// }



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