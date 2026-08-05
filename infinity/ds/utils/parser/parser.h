#ifndef __PARSER_H__
#define __PARSER_H__

/*DESCRIPTION:
* return num or parantheses from the beginning of string.
*check the exit_status before using the returned number.
*
*@param
*     str - Math expression
*     end_ptr - pointer to the next char after the number
*    parantheses - in case of parantheses return parantheses otherwise NULL
*@return
* the number on the beginning of the string.
*/
double GetNum(char *str, char **end_ptr, char *parantheses);

/*DESCRIPTION:
* return the operator in the begging.
*check the exit_status before using the returned number.
*
*@param
*     str - Math expression
*     end_ptr - pointer to the next char after the number
*
*@return
* the operstor in the beginning of the string.
*/
char GetOp(char *str, char **end_ptr);

#endif /* __PARSER_H__ */