#include <stdio.h>
#include <stdlib.h>

/*
 *
 * node.h - public interface to node.c, the binary search tree module
 *
 */

typedef struct node {
  struct value *val;
  struct node *next;
  struct node *leftNext;
} node;

extern node *newNode(struct value *x);

