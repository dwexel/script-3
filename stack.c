#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "include/stack.h"

#define check_pointer(p) if (!p) {puts("Out of memory."); exit(EXIT_FAILURE);}

#define MINIMUM_SIZE 1
 /* Minimal stack size (expressed in number of elements) for which
 space is allocated. It should be at least 1. */
#define GROWTH_FACTOR 2
 /* How much more memory is allocated each time a stack grows
 out of its allocated segment. */


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
    
    // top starts at -1 (no values on the stack)
    s->top = s->bottom - 1; 

    // allocated top = max size? a little confused
    s->allocated_top = s->bottom + MINIMUM_SIZE - 1; 
    return s;
}

/* Frees all the memory used for a stack. */
void destroy(stack *s)
{
    free(s->bottom);
    free(s);
}

/* Returns true if there are no elements on the stack. This
is different from the stack not having enough memory reserved
for even one element, which case is never allowed to arise. */
bool empty(stack *s)
{
    return s->top < s->bottom ? true : false;
}

/* Puts a new element on the stack, enlarging the latter's
memory allowance if necessary. */
void push(stack *s, T x)
{
    if (s->top == s->allocated_top)
    {
        //uses standard defs?
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

/* Removes and returns the topmost element. The result of popping
an empty stack is undefined. */
T pop(stack *s)
{
    return *(s->top--);
}

/* Returns the topmost element 
without removing it */
T peek(stack *s)
{
    return *(s->top);
}


/* Frees any memory the stack isn't actually using. The
allocated portion still isn't allowed to shrink smaller than
MINIMUM_SIZE. If all the stack's memory is in use, nothing
happens. */
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