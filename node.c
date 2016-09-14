#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "value.h"
/*
 * node.c - A function to help with the creation of the node struct class. This node can be used for a bst, queue, and stack
 *           - Returns the created node
 *           - written by Kurt Anderson
 *           - Last Updated 9/8/2016
*/


extern node *newNode(struct value *x);

//Creates a new node. Takes in paremeter of type value to be stored. 
node *newNode(struct value *x) {
    node *n;
    n = malloc(sizeof(node));
    n->val = x;
    n->next = NULL;
    n->leftNext = NULL;
    return n;
}








