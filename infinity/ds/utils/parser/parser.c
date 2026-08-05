#include <assert.h>
#include <stdlib.h>
#include "parser.h"


/*---------------FUNCTION DECLERATION---------------------*/

/*-----------------------ENUM--------------------------*/

/*--------------------------------------------------------*/

double GetNum(char *str, char **end_ptr, char *parantheses)
{
    assert(NULL != str);
    assert(NULL != end_ptr);
    if('(' == *str || '[' == *str || '{' == *str)
    {
        *parantheses = *str;
        *end_ptr = ++str;
        return 0;
    }    
    
    return strtod(str,end_ptr);
}
/*--------------------------------------------------------*/

char GetOp(char *str, char **end_ptr)
{
    assert(NULL != str);
    assert(NULL != end_ptr);
    *end_ptr = str + 1;
    return *str;
}
/*--------------------------------------------------------*/
