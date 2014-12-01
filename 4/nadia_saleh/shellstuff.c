#include "shellstuff.h"

int x = 1;
int f = 0; 


int main() {
  while (x) {
    char cwd[200];
    getcwd(cwd,sizeof(cwd));
    printf("%s she_sells_sea_shells$ ", cwd);

    char s[100];
    fgets (s, sizeof(s), stdin);

    char *s1 = s;
    s1 = strsep(&s1, "\n");
    char *s2;
    while ((s2 = strsep(&s1, ";"))) {
      s2 = rmspaces(s2);
      char *c;
      int tmp;
      int fd;
      if (strchr(s2, '>')) {
	c = strsep (&s2, ">");
	c = rmspaces(c);
	s2 =  rmspaces(s2);
	tmp = dup(STDOUT_FILENO);
	fd = open(s2, O_WRONLY|O_CREAT, 0644);
	dup2 (fd, STDOUT_FILENO);
	execute(c);
	dup2(tmp, STDOUT_FILENO);
	close(fd);
      }
      else if (strchr(s2, '<')) {
	c = strsep (&s2, "<");
	c = rmspaces(c);
	s2 =  rmspaces(s2);
	tmp = dup(STDIN_FILENO);
	fd = open(s2, O_RDONLY, 0644);
	dup2 (fd, STDIN_FILENO);
	execute(c);
	dup2(tmp, STDIN_FILENO);
	close(fd);
      }
      else if (strchr(s2, '|')) {
	int tmp_in;
	int tmp_out;
	c = strsep(&s2, "|");
	c = rmspaces(c);
	s2 = rmspaces(s2);
	tmp_in = dup (STDIN_FILENO);
	tmp_out = dup (STDOUT_FILENO);
	tmp = open (".pipe", O_WRONLY|O_CREAT, 0644);
	dup2(tmp, STDOUT_FILENO);
	execute(c);
	dup2(tmp_out, STDOUT_FILENO);
	close(tmp);
	tmp = open (".pipe", O_RDONLY, 0644);
	dup2(tmp, STDIN_FILENO);
	execute(s2);
	dup2(tmp_in, STDIN_FILENO);
	close(tmp);
	int f = fork();
	if (!f) {
	  execlp("rm", "rm", ".pipe", NULL);
	  exit(0);
	}
	else {
	  wait(&f);
	  f = 0;
	}
      }

      else {
	execute(s2);
      }
    }
  }

  return 0;
}



void execute( char * s1 ) {
  char *s2;
  char * args[10];
  int c = 0;
  while ((s2 = strsep(&s1, " "))) {
    args[c] = s2;
    c++;
  }
  args[c] = NULL;
  //exit
  if (!strcmp(args[0], "exit")) {
    x = 0;   
  }
  //cd
  else if (!strcmp(args[0], "cd")) {
    chdir(args[1]);
  }
  else {
    //fork
    int f = fork();
    if (!f) {
      execvp(args[0], args);
      exit(0);
    }
    else {
      wait(&f);
      f = 0;
    }
  }
}

char* rmspaces( char *str ){
  if (str[0] == ' ') {
    str = &str[1];
  }
  if (str[strlen(str)-1]==' ') {
    str[strlen(str)-1]=0;
  }
  return str;
}
