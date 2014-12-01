#include "prompt.h"
#include "parser.h"

//this file will contain the prompt messages and link to the main file

//prints out the prompt, with the current working directory
int print_prompt(){
	char * cwd;
	cwd = malloc(sizeof(char)*256);
	getcwd(cwd,256);
	//just the current directory
	char * cwdcopy = cwd;
	char * truncated_prompt;

	while(strchr(cwdcopy,'/')){
		truncated_prompt = strsep(&cwdcopy,"/");
		//printf("%s\n",cwd);
	}

	printf("%s$: ",cwdcopy);
	free(cwd);
	return 0;
}

//gets the input from the user
int get_input(){

	char input[256];
	//input = malloc(256 * sizeof(char));
	fgets(input,256,stdin);

	//printf("This is what you inputted: %s\n",input);
	parse_input(input);
	//free(input);
	return 0;

}
