#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void redirect_in(char * from, char * to, int mode){
  //> redirecting stdout to a file
  //STDOUT_FILENO
	int fd, fd1;
	fd = open(to, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	fd1 = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	dup2(fd1, STDOUT_FILENO);
}


//returns 1 if has redirect symbol (< , >)
//returns 2 if has two redirect symbols right next to each other
//else returns 0
//how to handle double redirection			  
int has_redirect(char * input){
	char * s = input;
	int len = strlen(s);
	int i;
	for (i = 0; i < len; i++) {
		if (s[i] == '<') {
			if (s[i + 1] == '<')
				return 2;
				return 1;
		}
		if (s[i] == '>') {
			if (s[i + 1] == '>')
				return 2;
			else
				return 1;
		}
	}
	return 0;
}
