#include <stdio.h>
#include "string.h"
#include <string.h>
#include <stdlib.h>


int strcasecmp(const char *str1, const char *str2);
char *strdup(const char *str);

int main()
{
	

  	/*****************************
   	****                     *****
	***  functions variables  ****
  	****                     *****
	******************************/

   	/* test for strcpy variables */

  	char str_cpy1[35] = "dsdfaher";
  	char str_cpy2[35] = "\"\"\"\"";

  	char str_copied1[35] = "ddddd";
  	char str_copied2[35];
  	char str_copied3[35];
  	char str_copied4[35];
  
  	/*test for nstrncpy variables*/

  	char str_ncpy1[] = "daherdaherdaher";
  	char str_ncpy2[] = "  asdfasd asd";

  	char str_copied5[35];
  	char str_copied6[35];
   	char str_copied7[35];
  	char str_copied8[35];

	/*test for Strcasecmp variables*/

  	char str_casecmp1[] = "helL everyoNe";
  	char str_casecmp2[] = "hello eVerYone";
   	int check_casecmp1 = 0;
  	int check_casecmp2 = 0;

  	/*test for Strchr variables*/

  	char *str_chr1 = "daher hdaher";
  	char *check_chr1;
  	char *check_chr2;
  	char c1 = ' ';


  	/*test for Strdup variables*/

   	char *str_dup1 = "daher hdaher";
   	char *check_dup1;
  	char *check_dup2;

  	/*test for Strcat variables*/

  	char str_cat1[] = "dsdfaher";
  	char str_cat2[] = "\"\"\"\"";

  	char *check_cat1;
  	char *check_cat2;

  	/*test for Strncat variables*/ 

  	char str_n_cat1[] = "dsdfahsdsder";
  	char str_n_cat2[] = "\"dadada\"";
  	size_t append=10;

  	char *check_n_cat1;
  	char *check_n_cat2;


  	/*test for Strstr variables*/

	char str_str1[] = "dsdfaher";
  	char str_str2[] = "\"fae\"";

  	char *check_str1;
  	char *check_str2;

  	/*test of strspn variables*/

  	char str_spn1[] = "daherdaherdaher";
   	char str_spn2[] = "dah";
	int mine_spn = 0;
  	int check_spn = 0;


  	/*test of IsPalindrom variables*/

  	char is_pali1[] = "d h d";
    	char is_pali2[] = "dahhad";
    	char is_pali3[] = "da  ad";
    	char is_pali4[] = " ";

    	/*test of SevenBoom variables*/
    	int small_boom = 0;
    	int large_boom = 0;

    	/*test of RemoveExtraWS variables*/

   	char ws_remove1[] = "      daher daher da d   da d asd her ad                        dada ad a d ad append     ";
   	char *ws_remove2;

  	/**************************************************************************
	**************************                       **************************
  	*******************            functions tests          *******************
  	**************************                       **************************
	***************************************************************************/

  	check_h_file();
  	
  	
	printf("\n");
  	Strcpy(str_copied1 , str_cpy1);
  	Strcpy(str_copied2 , str_cpy2);
	strcpy(str_copied3 , str_cpy1);
  	strcpy(str_copied4 , str_cpy2);

	if(strcmp(str_copied1,str_copied3) == 0 && strcmp(str_copied2,str_copied4) == 0)
    		printf("Strcpy is working \n");
  	else
      		printf("Strcpy is not workig \n");
 


  	/*test for strncpy*/



 	Strncpy(str_copied5,str_ncpy1,3);
	Strncpy(str_copied6,str_ncpy2,4);
 	strncpy(str_copied7,str_ncpy1,3);
  	strncpy(str_copied8,str_ncpy2,4);


  	if(strcmp(str_copied5 ,str_copied7) == 0 && strcmp(str_copied6 ,str_copied8) == 0 )
 		printf("Str_n_cpy is working\n");
  	else
  		printf("Str_n_cpy is not workig\n");

  
    	/*test for Strcasecmp*/

	check_casecmp1=Strcasecmp(str_casecmp1, str_casecmp2);
  	check_casecmp2=strcasecmp(str_casecmp1, str_casecmp2);
	if(check_casecmp1==check_casecmp2)
		printf("Strcasecmp is working\n");
  	else
    		printf("Strcasecmp is not workig\n");


  	/*test for Strchr*/

  	check_chr1 = Strchr(str_chr1,c1);
  	check_chr2 = strchr(str_chr1,c1);
  	
  	if(check_chr1==check_chr2)
  		printf("Strchr is working\n");
  	else
    	printf("Strchr is not workig\n");

  	/*test for Strdup*/

    	check_dup1 = Strdup(str_dup1);
  	check_dup2 = strdup(str_dup1);

  	if(strcmp(check_dup1, check_dup2) == 0)
	   	printf("Strdup is working\n");
  	else
    		printf("Strdup is not workig\n");


  	/*test for Strcat*/

  	check_cat1 = Strcat(str_cat1, str_cat2);
  	check_cat2 = strcat(str_cat1, str_cat2);

  	if(strcmp(check_cat1, check_cat2) == 0)
	   	printf("Strcat is working\n");
  	else
    		printf("Strcat is not workig\n");   
    

    	/*test for Strncat*/
    
  	check_n_cat1 = Strncat(str_n_cat1, str_n_cat2,append);
  	check_n_cat2 = strncat(str_n_cat1, str_n_cat2,append);

  	if(strcmp(check_n_cat1, check_n_cat2) == 0)
		  printf("Strncat is working\n");
  	else
    		printf("Strncat is not workig\n"); 


    	/*test for Strstr*/

    	check_str1 = Strcat(str_str1, str_str2);
  	check_str2 = strcat(str_str1, str_str2);

  	if(strcmp(check_str1, check_str2) == 0)
	   	printf("Strstr is working\n");
  	else
    		printf("Strstr is not workig\n");

    	/*test of strspn*/


    	mine_spn = Strspn(str_spn1 , str_spn2);
  	check_spn = strspn(str_spn1 , str_spn2);

  	if(mine_spn == check_spn)
    		printf("Str_spn is working\n");
  	else
    		printf("Str_spn is not workig\n");


    	/*test of IsPalindrom*/

    
    	if(IsPalindrom(is_pali1) == 1 && 
    	IsPalindrom(is_pali2) == 1 && 
      	IsPalindrom(is_pali3) == 1&& 
      	IsPalindrom(is_pali4) == 1)
    		printf("IsPalindrom is working\n");
  	else
    		printf("IsPalindrom is not workig\n");

	
	/*test of SevenBoom*/

   	small_boom = 65;
   	large_boom = 85;
   	SevenBoom(small_boom,large_boom);

   	

    	/*test of RemoveExtraWS*/

    	printf("before the RemoveExtraWS -> [%s]",ws_remove1);
    	ws_remove2 = RemoveExtraWS(ws_remove1);
    	printf("\nafter the RemoveExtraWS -> [%s]\n",ws_remove2);
    
    	return 0;
}


