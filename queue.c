#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "value.h"
#include "queue.h"

/*
 * queue.c   - A series of functions to create and manipulate the queue struct.
 *           - Keywords Enqueue and Dequeue are used for manipulation of queues
 *           - written by Kurt Anderson
 *           - Last Updated 9/8/2016
*/

// Creates a New Queue
queue *newQueue(void) {
    queue *workingQueue;
    workingQueue = malloc(sizeof(queue));
    workingQueue->root = NULL;
    workingQueue->tailPointer = NULL;
    workingQueue->length = 0;
    return workingQueue;
    
}

// Adds an element to a created queue
void enqueue(queue* q, node* newNode) {
    if (q->root == NULL) {
      q->root = q->tailPointer = newNode; 
      q->length = 1;
    }
    else {
        q->tailPointer->next = newNode;
        q->tailPointer = newNode;
        q->length += 1;
    }
    
}

// Removes an element of a queue returning it as a value
value *dequeue(queue* q) {
    struct value *val;
    struct node *releasedNode;
    if (q->length == 0) {
        return NULL;
    }
    else {
        val = q->root->val;
        releasedNode = q->root;
        q->root = q->root->next;
        free(releasedNode);
        q->length -= 1;
    }
    return val;
    
}

