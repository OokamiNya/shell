#include "prompt.h"
#include "parser.h"

#include <signal.h>
//This file will contain the main function

static void sighandler(int signo){
	if (signo == SIGINT){
		printf("You keyboard interrupted!\n");
		exit(3);
	}
}

int main(){

	signal(SIGINT,sighandler);

	while (1){
		print_prompt();
		get_input();
	}

}
