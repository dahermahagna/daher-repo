#ifndef __CALC_H__
#define __CALC_H__

#include "parser.h"

/* DESCRIPTION: 
 * calculate mathmtical expression and return the result
 * note- before you use the result check the exit status
 *
 *		@param
 *		math_exp - pointer to expression string
 *      exit_status - pointer to exit status
 *
 * @return
 *  return the result
 */

double Calculate(char *math_exp, int *exit_status);

enum calc_status
{
    SUCCESS = 0, 
    MATH_ERROR,
    SYNTAX_ERROR,
    MALLOC_ERROR
};
#endif /* __CALC_H__ */