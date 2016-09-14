#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "node.h"
#include "value.h"
#include "stack.h"
#include "queue.h"
#include "convert.h"

/*
 * convert.c - A conversion class for converted a queued expression into postfix. Also contains a helper function for this
 *           - Returns another queue with the postfix saved into this queue
 *           - written by Kurt Anderson
 *           - Last Updated 9/8/2016
*/

int priority(char *x);

struct queue *convertPost(queue *pQueue) { //Converts a Given Queue into a Postfix Queue
    struct value *evalVal;
    struct node *evalNode;
    char *evalString;
    int x = 0;
    int length = pQueue->length;
    struct stack *postEvaluator = newStack();
    struct queue *postQueue = newQueue();
    
    while (x < length){
        struct value *tempValue = dequeue(pQueue);
        struct node * tempNode;
    
        if (tempValue->type == 0){
            tempNode = newNode(tempValue);
            enqueue(postQueue,tempNode);
        }
        else if (tempValue->type == 1){
            tempNode = newNode(tempValue);
            enqueue(postQueue,tempNode);
        }
        else if (tempValue->type == 2){
            tempNode = newNode(tempValue);
           
            if (strcmp(tempValue->sVal,"(") == 0) {//Checks and Maintain Parenthesis
                push(postEvaluator,tempNode);
            } 
            else {
                if (strcmp(tempValue->sVal, ")") == 0){
                    evalVal = pop(postEvaluator);
                    evalString = evalVal->sVal;
                    while(strcmp(evalString,"(") != 0){
                        evalNode = newNode(evalVal);
                        enqueue(postQueue, evalNode);
                        evalVal = pop(postEvaluator);
                        evalString = evalVal->sVal;
                    }
                } 
                else {
                    if(postEvaluator->size > 0){
                        while(priority(tempValue->sVal) <= priority(peekStack(postEvaluator)->sVal)){ //Checks priority of opers
                            evalVal = pop(postEvaluator);
                            evalNode = newNode(evalVal);
                            enqueue(postQueue, evalNode);
                            if (postEvaluator->size == 0){
                                break;
                            }
                        }
                    }
                    push(postEvaluator, tempNode);
                }
            }
        }
        x++;
    }
    while(postEvaluator->size > 0){
        evalVal = pop(postEvaluator);
        evalNode = newNode(evalVal);
        enqueue(postQueue,evalNode);
    }
    return postQueue; 
}

//Calculates the priority of an Operand using the PEMDAS rules. Returns an int value, with higher priority being lower numbers
int priority(char *x){ //Chooses process
    if (strcmp(x,"(")== 0){
        return(0);
    }
    if (strcmp(x,"+") == 0){
        return(1);
    }
    if (strcmp(x,"-") == 0){
        return(1);
    }
    if (strcmp(x,"*") == 0){
        return(2);
    }
    if (strcmp(x,"/") == 0){
        return(2);
    }
    if (strcmp(x, "%") == 0){
        return(3);
    }
    if (strcmp(x, "^") == 0){
        return(4);
    }
    return(10);
}
