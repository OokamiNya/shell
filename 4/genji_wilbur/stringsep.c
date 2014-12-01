#include "stringsep.h"

/*
 * This is a function similar to strsep. It searches through a given string for the entire search query (not just one of the chars).
 * params: char ** str -> input string
		char *  query-> search query
 * It returns a pointer to the original string and puts NULL in the first place where the search query was found.
 * str is set to the first position after the found search query.
*/
void stringsep  (char ** str, char ** new, char * query)
{
	char * first;	
	
	first = *str;
	*str = strstr( *str, query);
	if (*str) {
		**str = 0;	
		*str += strlen(query);
	}

	*new = first;
}

/*
 * Params:
 * str: pointer to the string to parse through
 * query: array of strings that the function is going to search for.
 * min: pointer to an int indicating the position on query that was found first.
 * The function parses through the given string and looks for the earliest occurence
 * of any of the strings in the given search query. It sets the first character of 
 * the found string to NULL and sets str to the first position after the found string.
 * It returns the original position of str.
 * If str is NULL, then the function returns NULL.
 * If the string is not found, then the function returns the original string. min will return -1.
 * There is apparently no way to find the length of a string array. :(
 */
void stringsepar (char ** str, char ** new, char * query[], int * min)
{
	*new = *str;
	
	if ( *str == NULL ) {
		*new = NULL;
		return;
	}

	int x;
	char * tmp;
	char * min_addr = 0;
	int min_oper;

	for( x=0; x<9; x++) {
		tmp = strstr( *str, query[x] );
		if (tmp) {
			if (min_addr == 0) {
				min_addr = tmp;
				min_oper = x;
			} else if (tmp <= min_addr) {
				min_addr = tmp;
				min_oper = x;
			}
			
		}
	}

	
	if (min_addr) {
		min_addr[0] = NULL;
		*str = min_addr + strlen(query[min_oper]);
		*min = min_oper;
	} else {
		*min = -1;
		*str = NULL;
	}
}

/*
int main() {
	char * test1;
	char * test2;

	
	strcpy(test1, "Hello world!;Goodbye world!");	

	test2 = stringsep( &test1, ";");
	printf("test1: %s\n",test1);
	printf("test2: %s\n",test2);	

	return 0;
}
*/
