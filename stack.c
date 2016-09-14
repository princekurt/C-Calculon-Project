#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "value.h"
#include "stack.h"

/*
 * stack.c - A series of functions to manipulate the stack struct
 *           - written by Kurt Anderson
             - Last Updated 9/8/2016
*/

// Creates a new Stack
stack *newStack(void) {
    stack *workingStack;
    workingStack = malloc(sizeof(stack));
    workingStack->root = NULL;
    workingStack->size = 0;
    return workingStack;
    
}

// Adds an elemenet of type node to an already created stack
void push(stack* s, node* newNode) {
    if (s->root == NULL) {
      s->root = newNode; 
      s->size = 1;
    }
    else {
        newNode->next = s->root;
        s->root = newNode;
        s->size += 1;
    }
    
}

// Removes an element of type value from an already created stack.
value *pop(stack* s) {
    struct value *val;
    node *releasedNode;
    
    if (s->size == 0) {
        return NULL;
    }
    else {
        val = s->root->val;
        releasedNode = s->root;
        s->root = s->root->next;
        free(releasedNode);
        s->size -= 1;
        if (s->size == 0){
            s->root = NULL;
        }
    }
    return val;
    
}

//Peeks at the element on the top of the stack, and returns that element as a value object
value *peekStack(stack* s){
    if (s->root != NULL){
        return(s->root->val); 
    }
    else{
        return(NULL);
    }
  
}


