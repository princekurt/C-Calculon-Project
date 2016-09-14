#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include "node.h"
#include "value.h"
#include "queue.h"
#include "stack.h"
#include "scanner.h"
#include "convert.h"
#include "bst.h"

/*
 * calculon.c - A Calculator made specifically to deal with normal calculator operations 
 *              as defined through University of Alabama CS201 Assignment 1
 *           - written by Kurt Anderson
 *           - Program prints to the stdout stream. 
             - Last Updated 9/8/2016
*/

/* Options */
int authorSwitch = 0;
int postfixSwitch = 0;

value *assignValue(char *token);
value *convertToken(char * token);
value *calcExpr(queue *calcQueue, bst *binaryTree);
int priority(char *x);
int checkHiddenNumber(char *token);
int ProcessOptions(int argc, char **argv);
char *stripQuotes(char *token);
char *addQuotes(char *token);
void Fatal(char *fmt, ...);


int main(int argc, char **argv) 
{
    struct queue *captureQueue = newQueue();
    struct queue *postQueue = NULL;
    struct value *finalValue = NULL;
    struct bst *variableStorage = newBST();
    int stringFlag = 0;
    int argIndex;
    char *previousToken = NULL;
    char *token;
    FILE *in;
    
    argIndex = ProcessOptions(argc,argv);
    if (authorSwitch == 1){ // -v Displays Author Information
        fprintf(stderr, "Kurt Anderson\n");
        exit(0);
    }
    
    // Either read from File or from Stdin
    if (argIndex == argc){
        in = stdin;
    }
    else {
        in = fopen(argv[argIndex], "r");
    }
    
    //Protect against an empty file.
    if (in == NULL){
        fprintf(stderr,"file %s could not be opened for reading\n", argv[argIndex]);
        exit(1);
    }

    token = readToken(in);
    while (!feof(in)){
        if (strcmp(token,";") == 0){ //End of Expression, Evaulate it
            postQueue = convertPost(captureQueue);
            if (postfixSwitch == 0){
                finalValue = calcExpr(postQueue, variableStorage);
            }
        }
        else if (stringFlag == 1){ //A Quoted String to be processed
            token = addQuotes(token);
            struct value *workingValue = assignValue(token);
            struct node *tempNode = newNode(workingValue);
            enqueue(captureQueue, tempNode);
            stringFlag = 0;
        }
        else if (strcmp(token, "var") == 0){ //Process and Add Variable
            token = readToken(in);
            char *newVar = token;
            struct value *finalVarValue;
            struct queue *variableQueue = newQueue();
            struct queue *postVariableQueue = newQueue();
            struct value *bstValue = NULL;
            struct node *bstNode = NULL;
            
            token = readToken(in); //Get Rid of the = sign
            token = readToken(in);
        
            while(strcmp(token,";") != 0){ 
                struct value *varValue = assignValue(token);
                struct node *varNode = newNode(varValue);
                
                enqueue(variableQueue, varNode);
                token = readToken(in);
            }
            
            postVariableQueue = convertPost(variableQueue);
            finalVarValue = calcExpr(postVariableQueue, variableStorage);
            
            if (finalVarValue->type == 0){
                bstValue = newValueVariableInt(newVar, finalVarValue->iVal);
            }
            else{
                bstValue = newValueVariableReal(newVar, finalVarValue->rVal);
            }
            
            bstNode = newNode(bstValue);
            addBST(variableStorage, bstNode);
            
        }
        else if (strcmp(token, "=") == 0){ //Process a revalue of a variable
            char *updateVar = previousToken;
            struct value *finalVarValue = findVariable(variableStorage, previousToken);
            if (finalVarValue == NULL){
                Fatal("Variable %s has not been declared yet\n", updateVar);
            }
            struct queue *variableQueue = newQueue();
            struct queue *postVariableQueue = newQueue();
            struct value *bstValue = NULL;

            token = readToken(in); //Read the First Token of the Expression
        
            while(strcmp(token,";") != 0){ 
                value *varValue = assignValue(token);
                node *varNode = newNode(varValue);
                enqueue(variableQueue, varNode);
                token = readToken(in);
            }
            
            postVariableQueue = convertPost(variableQueue);
            finalVarValue = calcExpr(postVariableQueue, variableStorage);
            
            if (finalVarValue->type == 0){
                bstValue = newValueVariableInt(updateVar, finalVarValue->iVal);
            }
            else{
                bstValue = newValueVariableReal(updateVar, finalVarValue->rVal);
            }
            
            
            editBST(variableStorage,previousToken, bstValue);
            captureQueue = newQueue();
        }
        else { //Add Variable to the Queue
            struct value *workingValue = assignValue(token);
            struct node *tempNode = newNode(workingValue);
            enqueue(captureQueue, tempNode);
            
        }
        
        if (stringPending(in)){ //Check for a string as the next input
            previousToken = token;
            token = readString(in);
            stringFlag = 1;
        }
        else { //If there is no string, proceed with reading as a token
            previousToken = token;
            token = readToken(in);
        }
        
       
    }
    
    if (postfixSwitch == 1){  //Display the Postfix Output Instead of a number value
        int size = postQueue->length;
        for (int x = 0; x < size; x++){
            struct value *cursorValue = dequeue(postQueue);
            if (cursorValue->type == 0){
               fprintf(stdout, "%d ", cursorValue ->iVal);
            }
            else if (cursorValue->type == 1){
                fprintf(stdout, "%f ", cursorValue ->rVal); 
            }
            else if (cursorValue->type == 2){
                fprintf(stdout, "%s ", cursorValue ->sVal); 
            }
        }
        fprintf(stdout, "\n");     
    }
    else { //Display the number value as output
        if (finalValue->type == 0){
            fprintf(stdout,"%d\n",finalValue->iVal);
        }
        else if (finalValue->type == 2){
            fprintf(stdout,"%s\n",finalValue->sVal);
        }
        else{
            fprintf(stdout,"%f\n",finalValue->rVal);
        } 
    }
    return 0;
}

//Removes Quotes from a string and returns that string
char *stripQuotes(char *token){
    token++;
    token[strlen(token)-1] = 0;
    return token;
}

//Adds Quotes to a String and returns that String
char *addQuotes(char *token){
    char *c = malloc(sizeof(char) * (strlen(token) + 3));
    sprintf(c,"\"%s\"", token);
    return c;
}

//Converts the string into a real number or an integer
value *convertToken(char *token){
    int decimalFlag = 0;
    int i;
    value *returnValue;
    
    token = stripQuotes(token);
    
    for(i = 0; i < strlen(token); i++){
        if (token[i] == '.') {
            decimalFlag = 1;
        }
    }
    if (decimalFlag == 1){
        returnValue = newValueReal(atof(token));
        returnValue->sVal = "CNV";
    }
    else {
        returnValue = newValueInt(atoi(token));
        returnValue->sVal = "CNV";
    }
    return returnValue;
}

//Checks to See if a real or integer is in a string. Returns 1 for True, 0 for False.
int checkHiddenNumber(char *token){ 
    int invalidFlag = 0;
    int i;
    if (token[0] == '"' && token[strlen(token) - 1] == '"'){
        for(i = 0; i < strlen(token); i++){
            if (isalpha(token[i])){
                invalidFlag = 1;
            }
        }
    }
    else {
        return 0;
    }
    if (invalidFlag == 0){
        return 1;
    }
    else {
        return 0;
    }
}

//Runs the calculations of the values stored in the queue
value *calcExpr(queue *calcQueue, bst *binaryTree){
    struct stack *holdStack = newStack();
    struct value *tempValue;
    int length = calcQueue->length;
    int y;
    struct node *tempNode = NULL;
    for (y = 0; y < length; y++) {
        tempValue = dequeue(calcQueue);
        if (tempValue->type == 0){
            tempNode = newNode(tempValue);
            push(holdStack, tempNode);
        }
        else if (tempValue->type == 1){
            tempNode = newNode(tempValue);
            push(holdStack, tempNode);   
        }
        else if (tempValue->type == 2){
            char *x = tempValue->sVal;
            if (isalpha(x[0])){
                value *varValue = findVariable(binaryTree, tempValue->sVal);
                if (varValue == NULL){
                    Fatal("Variable %s has not been declared yet\n", tempValue->sVal);
                }
                tempNode = newNode(varValue);
                push(holdStack, tempNode);   
            }
            else if (x[0] == '"'){
                tempNode = newNode(tempValue);
                push(holdStack, tempNode);
            }
            else{
                double firstNum;
                double secondNum;
                double answer;
                int intflag = 0;
                struct value *finalValue;
                struct value *topNum = pop(holdStack);
                struct value *bottomNum = pop(holdStack);
                
                if (bottomNum->type == 2 && topNum->type == 2){ //If both are strings, it needs to be concatonated
                    char *topString = stripQuotes(topNum->sVal);
                    char *bottomString = stripQuotes(bottomNum->sVal);
                    char *c = malloc(sizeof(char) * (strlen(topString) + strlen(bottomString) + 1));
                    if (c == 0) Fatal("out of memory\n");
                    sprintf(c,"%s%s",bottomString,topString);
                    
                    c = addQuotes(c);
                    finalValue = newValueString(c);
                    tempNode = newNode(finalValue);
                    push(holdStack, tempNode);
                    
                }
                else if (bottomNum->type == 2 && topNum->type !=2) { //Checks to see if the conv number really needs to be a string
                    char *recastValue =  malloc(sizeof(char) * (strlen(bottomNum->sVal) + 200));
                    if (topNum->type == 0){
                        sprintf(recastValue,"%d",topNum->iVal);
                    }
                    else {
                        sprintf(recastValue,"%f",topNum->rVal);
                    }
                    char *topString = recastValue;
                    char *bottomString = stripQuotes(bottomNum->sVal);
                    char *c = malloc(sizeof(char) * (strlen(topString) + strlen(bottomString) + 1));
                    if (c == 0) Fatal("out of memory\n");
                    sprintf(c,"%s%s",bottomString,topString);
                    
                    c = addQuotes(c);
                    finalValue = newValueString(c);
                    tempNode = newNode(finalValue);
                    push(holdStack, tempNode);
                }
                else if (topNum->type == 2 && bottomNum->type !=2) {//Checks the other number for the aboe
                    char *recastValue = malloc(sizeof(char) * (strlen(topNum->sVal) + 200));
                    if (bottomNum->type == 0){
                        sprintf(recastValue,"%d",bottomNum->iVal);
                    }
                    else {
                        sprintf(recastValue,"%f",bottomNum->rVal);   
                    }
                    char *topString = stripQuotes(topNum->sVal);
                    char *bottomString = recastValue;
                    char *c = malloc(sizeof(char) * (strlen(topString) + strlen(bottomString) + 1));
                    if (c == 0) Fatal("out of memory\n");
                    sprintf(c,"%s%s",bottomString,topString);
                    
                    c = addQuotes(c);
                    finalValue = newValueString(c);
                    tempNode = newNode(finalValue);
                    push(holdStack, tempNode);
                }
                else {
                    int topConvertedFlag = 0;
                    int bottomConvertedFlag = 0;
                    
                    //Grabs the correct number type from the value objects
                    if (bottomNum->type == 0){
                        firstNum = (double) bottomNum->iVal;
                    }
                    else {
                        firstNum = bottomNum->rVal;
                    }
                    
                    if (topNum->type == 0){
                        secondNum = (double) topNum->iVal;
                    }
                    else {
                        secondNum = topNum->rVal;
                    }
                    
                    //Checks to see if they are both integers
                    if (bottomNum->type == 0 && topNum->type == 0) {
                        intflag = 1;
                    }
                    
                    //Sets flags for if numbers were inputted through a string
                    if (bottomNum->sVal != NULL){
                        if (strcmp(bottomNum->sVal, "CNV") == 0){
                            bottomConvertedFlag = 1;
                        }
                    }
                    if(topNum->sVal != NULL){
                       if (strcmp(topNum->sVal, "CNV") == 0) {
                            topConvertedFlag = 1;
                        } 
                    }
                    
                    //Casts the Numbers to provide correct calcualtions if they were inputted from a string. 
                    if (topConvertedFlag == 1){
                        if (bottomNum->type == 0){
                            int secondNumTemp = (int) secondNum;
                            secondNum = (double) secondNumTemp;
                        }
                    }
                    if (bottomConvertedFlag == 1){
                        if (topNum->type == 0){
                            int firstNumTemp = (int) firstNum;
                            firstNum = (double) firstNumTemp;
                        }
                    }
                    
                    //Perfroms the operations given through the operator
                    if (strcmp(x, "^") == 0){
                        answer =  powf(firstNum,secondNum);  
                    }
                    else if (strcmp(x,"+") == 0){
                        answer = firstNum + secondNum;
                    }
                    else if (strcmp(x,"-") == 0){
                        answer = firstNum - secondNum;
                    }
                    else if (strcmp(x,"*") == 0){;
                        answer = firstNum * secondNum;
                       
                    }
                    else if (strcmp(x,"/") == 0){
                        answer = firstNum / secondNum;
                    }
                    else if (strcmp(x,"%") == 0){
                        answer = fmod(firstNum, secondNum);
                    }
                    
                    //Does the final cast if the numbers were parsed from a string
                    if (topConvertedFlag == 1){
                        if (bottomNum->type == 0){
                            finalValue = newValueInt((int) answer);
                        }
                        else if (bottomNum->type == 1){
                            finalValue = newValueReal(answer);
                        }
                    }
                    else if (bottomConvertedFlag == 1){
                         if (topNum->type == 0){
                            finalValue = newValueInt((int) answer);
                        }
                        else if (topNum->type == 1){
                            finalValue = newValueReal(answer);
                        }
                    }
                    else { //Does a final Cast if the numbers were not input from a string
                        if (intflag == 1){
                            finalValue = newValueInt((int) answer);
                    }
                        else {
                            finalValue = newValueReal(answer);
                        }  
                    }
                    
                    //Pushes the new Node onto the stack
                    tempNode = newNode(finalValue);
                    push(holdStack, tempNode);
                }
            }
        }
    
    }
    value *returnValue = pop(holdStack);
    return (returnValue);
}


//Assigns a value depending on the type of the var. Returns this new Value
value *assignValue(char *token) {
    struct value *tempValue;
    int i = 0;
    int decimalFlag = 0;
    int charFlag = 0;
    int operFlag = 0;
    if(checkHiddenNumber(token)){//Checks to see if the string is holding a number that needs parsing. 
        value *converted = convertToken(token);
        return converted;
    }
    
    if (isalpha(token[0]) == 0 && isdigit(token[0]) == 0){
        if (strlen(token) > 1){
            for(i = 0; i < strlen(token); i++){
                if (isalpha(token[i])){
                    charFlag = 1;
                    break;
                }
                else if (token[i] == '.'){
                    decimalFlag = 1;
                    break;
                }  
            }
        }
        else {
            operFlag = 1;   
        }
           
    }
    else {
        for(i = 0; i < strlen(token); i++){
            if (isalpha(token[i])){
                charFlag = 1;
                break;
            }
            else if (token[i] == '.'){
                decimalFlag = 1;
                break;
            }
        }
    }
   
    if (decimalFlag == 1){
        double d;
        sscanf(token, "%lf", &d);
        tempValue = newValueReal(d);
    }
    else if (operFlag == 1){
        tempValue = newValueString(token);
    }
    else if (charFlag == 1){
        tempValue = newValueString(token);
    }
    else {
        int converted = atoi(token);
        tempValue = newValueInt(converted);
    }
    return tempValue;
}

//Processes a system ending error. 
void Fatal(char *fmt, ...) {
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
    }
 
 //Processes the options needed to be executed from the command line
int ProcessOptions(int argc, char **argv) {
    int argIndex;
    int argUsed;
    int separateArg;

    argIndex = 1;

    while (argIndex < argc && *argv[argIndex] == '-')
        {
        /* check if stdin, represented by "-" is an argument */
        /* if so, the end of options has been reached */
        if (argv[argIndex][1] == '\0') return argIndex;

        separateArg = 0;
        argUsed = 0;

        if (argv[argIndex][2] == '\0')
            {
            separateArg = 1;
            }
           
        switch (argv[argIndex][1])
            {
            case 'v':
                authorSwitch = 1;
                break;
            case 'd':
                postfixSwitch = 1;
                break;
            default:
                Fatal("option %s not understood\n",argv[argIndex]);
            }

        if (separateArg && argUsed)
            ++argIndex;

        ++argIndex;
        }

    return argIndex;
}