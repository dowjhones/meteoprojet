#include "utils.h"

// string to double
double str_to_double(double *result, const char* str)
{
    char *endPtr; 
    long value = strtod(str, &endPtr); 

    // check if no error
    if (strcmp(endPtr, ""))
        return (0);
    // check if value is a double
    if (value > __DBL_MAX__ || value < __DBL_MIN__) 
        return (0);
    // set the value in the pointer
    *result = value;
    return (1);
}

// compare two values
int compare(double a, double b, int reverse)
{
    // if descending order
    if (reverse) 
        return (a > b); 
    // if ascending order
    return (a < b);
}