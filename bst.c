#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "value.h"
#include "bst.h"

/*
 * bst.c - A binary search tree class that uses nodes and values to create the tree. Sorted using incoming strings 
 *           - written by Kurt Anderson
 *           - usees the value.c and node.c files created by the same author
             - Last Updated 9/8/2016
*/

value *findVariable(bst *workingBST, char *findNode);

//Creates a new Binary Search tree of the type Struct bst *<NewTree> 
//Returns a struct bst *
bst *newBST(void){
    bst *workingBST;
    workingBST = malloc(sizeof(bst));
    workingBST->root = NULL;
    workingBST->size = 0;
    return workingBST;
}

//Adds a node to an already created binary search tree. 
void addBST(bst *workingBST, node *newNode){
    int direction = 0;
    struct node *cursor, *previousCursor;
    struct value *tempVal = findVariable(workingBST, newNode->val->sVal); 
    if (tempVal != NULL){ //Variable already in the Tree
        return;
    }
    if (workingBST->root == NULL){
        workingBST->root = newNode;
        return;
    }
    cursor = workingBST->root;
    while(cursor != NULL){
        if (strcmp(newNode->val->sVal, cursor->val->sVal) > 0){
            previousCursor = cursor;
            cursor = cursor->next;
            direction = 1;
        }
        else {
            previousCursor = cursor;
            cursor = cursor->leftNext;
            direction = 0;
        }
    }
    if (direction == 1){
        previousCursor->next = newNode;
    }
    else {
        previousCursor->leftNext = newNode;
    }
}

//Finds a variable that has been put into a binary search tree
value *findVariable(bst *workingBST, char *findString){
    struct node *cursor;
    
    if(workingBST->root == NULL){
        return NULL;
    }
    
    if (strcmp(findString, workingBST->root->val->sVal) == 0){//See if the variable is at the root
        return workingBST->root->val;
    }
    
    cursor = workingBST->root;
    while(cursor != NULL){ //Scan through the tree to locate the Variable
        if (strcmp(findString,cursor->val->sVal) == 0) {
            break;
        }
        
        if (strcmp(findString, cursor->val->sVal) > 0){
            cursor = cursor->next;
        }
        else {
            cursor = cursor->leftNext;
        }
    }
    
    if (cursor == NULL){//Cursor never found a match
        return NULL;
    }
    else {
        return cursor->val;
    }
    
}

void editBST(bst *workingBST, char * findString, value *newValue){
    struct node *cursor;
    
    if(workingBST->root == NULL){
        return;
    }
    
    if (strcmp(findString, workingBST->root->val->sVal) == 0){
        workingBST->root->val = newValue;
    }
    
    cursor = workingBST->root;
    while(cursor != NULL){
        if (strcmp(findString,cursor->val->sVal) == 0) {
            break;
        }
        
        if (strcmp(findString, cursor->val->sVal) > 0){
            cursor = cursor->next;
        }
        else {
            cursor = cursor->leftNext;
        }
    }
    
    if (cursor == NULL){
        return;
    }
    else {
        cursor->val = newValue;
    }
    
}