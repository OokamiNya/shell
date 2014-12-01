#include "strip_spaces.h"

/*
 * Strips spaces from the input string.
 * It removes spaces from the beginning of the input,
 * removes spaces from the end of the input,
 * and turns instances of more than one space into a single space.
 */
char * strip_spaces(char * input){
	int i = 0;
	//starting from the beginning, get rid of the space
	while(input[i] == ' '){
		input = &input[1];
	}

	//going through the input
	while(input[i]){
		//two consecutive spaces
		if (input[i] == ' ' && input[i+1] == ' '){
			int numConsec = 2;
			while(input[i+numConsec] == ' '){
				numConsec++;
			}
			//at this point, i + numConsec is the first non-space character
			int j = 1;
			while(input[i+numConsec]){
				input[i+j] = input[i+numConsec];
				j++;
				numConsec++;
			}
			//at this point, i + numConsec is the null at the end
			input[i+j] = 0;
		}
		i++;
	}
	//end of input
	if (input[i-1] == ' '){
		input[i-1] = 0;
	}

	return input;
}

