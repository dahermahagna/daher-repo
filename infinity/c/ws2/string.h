#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h> /* size_t */


void check_h_file();




/*DESCRIPTION: 
*Copies the string pointed to by src, including the terminating null byte ('\0'), to the buffer  pointed  to  by  dest.
* dest need to be in a size equal or bigger then the src string
* the function add a termination sign at the end of the copied string
*@param
*dest: This is the pointer to the destination array where the content is to be copied.
*src: This is the string to be copied.
*
@return
*Pointer to the destination string dest
*/
char *Strcpy(char *dest, const char *src);

/*DESCRIPTION:
*Copies up to n characters from the string pointed to, by src to dest. 
*In a case where the length of src is less than that of n, the remainder of dest will be padded with null bytes.
*
*@param
*dest: This is the pointer to the destination array where the content is to be copied.
*src: This is the string to be copied
*n: The number of characters to be copied from source
@return
*Pointer to the dest string.  
*/
char *Strncpy(char *dest, const char *src, size_t n);

/*DESCRIPTION:
*Compares str1 and str2 without sensitivity to case.
*The strcasecmp() function operates on null terminated strings.
*The string arguments to the function are expected to contain a null character ('\0') marking the end of the string.
*@param
*str1: the source string for comparison
*str2: the string to be compared.
@return
*   an integer less than, equal to, or greater than zero if str1 is, after ignoring case, found to be less than,  to  match,  or  be  greater  than  str, respectively.
*/
int Strcasecmp(const char *str1, const char *str2);

/* DESCRIPTION:
 * Search for a character in the string.
 *
 * @param:
 * string:        Pointer to the needed string.
 * char:          The searched character
 *         
 * @return:
 * Returns pointer of the first occurrence of the character c in the string   str, and NULL in case c doesn’t exist.
 */
char *Strchr(const char *str, int c);


/*DESCRIPTION:
* Returns a pointer to a new string which is a duplicate of the string str.  
* Memory for the new string is obtained with malloc(3), and can be freed with free(3). 
*
*@param
* str: pointer to source string
@return
* On success, the strdup() function returns a pointer to the duplicated string.  
* It returns  NULL  if  insufficient memory was available, with errno set to indicate the cause of the error.
*/
char *Strdup(const char *str);



/*DESCRIPTION:
* Appends the src string to the dest string,
* overwriting the terminating null byte ('\0') at the end of dest, and then adds a terminating null byte.
* The strings may not overlap, and the  dest  string  must have  enough  space for the result.
* If dest is not large enough, program behavior is unpredictable; buffer overruns are a favorite avenue for attacking secure programs.
*
*@param
* dest: pointer to string to append to
* src: pointer to constant string to be appended
@return
* Pointer to the resulting string dest.
*/
char *Strcat(char *dest, const char *src);



/*DESCRIPTION:
*Appends the string pointed to by src to the end of the string pointed to by dest up to n characters long
* it will use at most n bytes from src; and src does not need to be null-terminated if it contains n or more bytes.
* The strings may not overlap, and the  dest  string  must have  enough  space for the result.
* If dest is not large enough, program behavior is unpredictable; buffer overruns are a favorite avenue for attacking secure programs.
*@param 
* dest: This is pointer to the destination, which should contain a C string,
*    and should be large enough to contain the concatenated resulting string which includes the additional null-character.
* src: pointer to constant string to be appended.
* n: This is the number of characters to be appended. 
*
@return
* Pointer to the resulting string dest.
*/
char *Strncat(char *dest, const char *src, size_t n);

/*DESCRIPTION"
* Finds the first occurrence of the substring needle in the string haystack.
* The terminating null bytes ('\0') are not compared.
*
* @param
*	haystack: const pointer to string haystach.
*   needle: const pointer to string needle.
@return
* Pointer to the beginning of the located substring, and  NULL  if  the  substring  is  not found. 
*/
char *Strstr(const char *haystack, const char *needle);


/* DESCRIPTION:
 * Calculates the length of the initial segment of str1 which consists entirely of characters in str2.
 * @param:
 * str1: Const pointer to string str1. 
 * str2: const pointer to string str1.
 * @return:
 * number of bytes in the initial segment of str1 which consist only of bytes from str2.
 
  */
size_t Strspn(const char *str1, const char *str2);


/* DESCRIPTION:
 * Breaks String str1 into a series of tokens using the delimiter delim
 * The first call to strtok() returns a pointer to the first token in the string  pointed to by str1. Subsequent calls to strtok() must pass a NULL pointer as the first argument, in order to get the next token in the string. The set of delimiters used in each of these calls to strtok() can be different from one call to the next. The function will modify the user's data.
 * @param:
 * str: or NULL, or the string that you want to break into tokens
 * delim: A set of the characters that separate the tokens.
 *         
 * @return:
 * Pointer to the next token, or NULL if there are no more tokens.
 */
char *Strtok(char *str, const char *delim);

/*DESCRIPTION: 
*checks if a string is a palindrome.
*takes all characters into consideration, not only letters.
*ingnores the terminator character at the end.
*
*@param
* str: pointer to the start of the string to be checked
*
@return
a value of 1 if the string is a palindrome, else 0;
*/

int IsPalindrom(const char *str);

/*DESCRIPTION: 
*receives a range of numbers (num1,num2).
*print all the numbers in the range.
*if a number contains the digit 7, or is divisible by 7 with no remainder, print 'BOOM' insted. 
*
*@param
* num1 - int, number that starts the range;
* num2 - int, number that ends the range;
*
@return
* void
*/
void SevenBoom(int num1, int num2);

/*DESCRIPTION:
*the function receives a string and deletes white spaces ('tab' and 'space') from the end and beginning of the string;
*deletes duplicates white spaces within the string (replaces sequences of multiple white space with the first white space in sequence)


*@param

*str1: the source string with whitespace to be removed.

*@return
*a pointer to the modified string (str). 
*if an error occurs, returns NULL.

*/
char * RemoveExtraWS(char *str);



#endif  /* __STRING_H__ */ 
