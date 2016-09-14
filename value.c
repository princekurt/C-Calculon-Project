#include <stdio.h>
#include <stdlib.h>
#include "value.h"

/*
 * value.c - A series of functions to manipulate the value struct
 *           - written by Kurt Anderson
             - Last Updated 9/8/2016
*/

static value *newValue(int);

// Constants to differentiate value types
int INTEGER = 0;
int REAL = 1;
int STRING = 2;

// Creates a value of type INTEGER
value *newValueInt(int i){
    value *v = newValue(INTEGER);
    v->iVal = i;
    return v;
}

// Creates a new Value of type REAL
value *newValueReal(double r) {
    value *v = newValue(REAL);
    v->rVal = r;
    return v;
}

// Creates a new Value of type STRING
value *newValueString(char* str){
    value *v = newValue(STRING);
    v->sVal = str;
    return v;
}

// Creates a new value of type VARIABLE and sets that variables iVal paraemeter to a given int
value *newValueVariableInt(char* var, int intVal){
    value *v = newValue(INTEGER);
    v->sVal = var;
    v->iVal = intVal;
    return v;
}

// Creates a new value of type VARIABLE and sets that variables rVal paraemeter to a given real number
value *newValueVariableReal(char* var, double realVal){
    value *v = newValue(REAL);
    v->sVal = var;
    v->rVal = realVal;
    return v;
    
}

/***** Private Interface *****/
// Creates a new value and mallocs the data required
static value *newValue(int type){
    value *v;
    v = malloc(sizeof(value));
    v->type = type;
    v->iVal = 0;
    v->rVal = 0;
    v->sVal = 0;
    return v;
}