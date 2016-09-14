#include <stdio.h>
#include <stdlib.h>
/*
 *
 * value.h - public interface to value.c, the value module
 *
 */

typedef struct value {
    int type;
    int iVal;
    double rVal;
    char *sVal;
    } value;
    
extern int INTEGER;
extern int REAL;
extern int STRING;

extern value *newValueInt(int);
extern value *newValueReal(double);
extern value *newValueString(char *);
extern value *newValueVariableInt(char* var, int intVal);
extern value *newValueVariableReal(char* var, double realVal);

