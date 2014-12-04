#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <errno.h>

#include "s4.h"

/* char *cleanUpString()
Inputs: char *s - string that needs to be cleaned
Returns: String with fixed spaces */
char * cleanUpString(char * s, char *ret){
	int i;
	int j = 0;
	for (i = 0; s[i]; i++){
		while (s[i]  == ' ' && s[i+1] == ' '){
			i++;
		}
		if (s[0] == ' ' && j == 0) i++;
		if ((s[i] == '>' && ret[j-1] != ' ' && ret[j-1] != '2' && ret[j-1] != '>') ||
		    (s[i] == '2' && s[i+1] == '>' && ret[j-1] != ' ' && ret[j-1] != '>')){
			ret[j] = ' ';
			j++;
		}
		if (s[i] != ' ' && s[i] != '>' && ret[j-1] == '>'){
			ret[j] = ' ';
			j++;
		}
		ret[j] = s[i];
		j++;
	}
	if (ret[j-1] == ' ') ret[j-1] = 0;
	return ret;
}


/* int execute()
Inputs: char *r - command to be executed, ex. ls -l, cat > out.txt
        int in - file descriptor with input value
        int out - file descriptor to write output value to

handles cd case
handles >, >>, 2>, 2>>, &>, <

executes the command and returns the childpid so that the main process can wait for the child process to finish */
int execute(char* r, int in, int out){
	int f = fork();
	if (f == 0){
		dup2(in, STDIN_FILENO);
		dup2(out, STDOUT_FILENO);
                char clean[1000] = {0};
                r = cleanUpString(r, clean);

		int n = count_tokens(r, ' '), lend = 0;
		char **c = split(r, ' ');

		char *reduced[1000] = {0};

		int i=0, fd;
		while(i<n){
            // "cd" case
            if (!strcmp("cd", c[i])) {
                // handle the error
                if (chdir(c[i+1]) == -1)
                	printf("%s\n", strerror(errno));
                return -1;
                        }
			if(!strcmp(">", c[i])){
				fd = open(c[i+1], O_CREAT | O_WRONLY | O_TRUNC, 0777);
				dup2(fd, STDOUT_FILENO);

			}
			else if(!strcmp(">>", c[i])){
				fd = open(c[i+1], O_CREAT | O_WRONLY | O_APPEND, 0777);
				dup2(fd, STDOUT_FILENO);
			}
			else if(!strcmp("2>", c[i])){
				fd = open(c[i+1], O_CREAT | O_WRONLY | O_TRUNC, 0777);
				dup2(fd, STDERR_FILENO);
			}
			else if(!strcmp("2>>", c[i])){
				fd = open(c[i+1], O_CREAT | O_WRONLY | O_APPEND, 0777);
				dup2(fd, STDERR_FILENO);
			}
			else if(!strcmp("&>", c[i])){
				fd = open(c[i+1], O_CREAT | O_WRONLY | O_TRUNC, 0777);
				dup2(fd, STDOUT_FILENO);
				dup2(fd, STDERR_FILENO);
			}
			else if(!strcmp("<", c[i])){
				fd = open(c[i+1], O_RDONLY);
				dup2(fd, STDIN_FILENO);
			}
			else {
				reduced[i] = c[i];
				lend++;
				i--;
			}
			i += 2;
		}
		if(lend){
			execvp(reduced[0], reduced);
			printf("Unknown Command\n");
                        exit(0);
		}
	}
        return f;
}
