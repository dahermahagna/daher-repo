#ifndef __WS3_H__
#define __WS3_H__


/* version 1.1 */

/*DESCRIPTION: 
*Prints a list of all the environment variables in lowercase.
*
*@param
*envp: environment variables recived from main
*
@return
*in case of success- return 0
*in case of faile- return non-zero
*/
int PrintLowerEnv(char *envp[]);


/*DESCRIPTION: 
*Returns the soldier number (starting from 1) of the last soldier standing according to the 'Josephus' algorithm.
*For any number of soldiers greater than zero.
*
**@param
*num_of_soldiers: the number of soldiers/elements.
*
@return
*return the number of the last standing soldier (starting from 1).
*in case of failure - return 0
*/
size_t Josephus(size_t num_of_soldiers);



/*DESCRIPTION: 
*Prints the sizes of all the data types in the C language.
*/
void PrintTypeSizes();


#endif /*__WS3_H__*/

