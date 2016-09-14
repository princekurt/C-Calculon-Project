/*
 *
 * stack.h - public interface to stack.c, the queue module
 *
 */

typedef struct stack {
    struct node *root;
    int size;
    
} stack;

extern void push(stack* q, node* newNode);
extern value *pop(stack* q);
extern stack *newStack(void);
extern value *peekStack(stack* s);