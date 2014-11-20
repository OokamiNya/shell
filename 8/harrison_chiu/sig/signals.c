#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>


static void sighandler(int signo);
void sigusr(char *);
void sigint();

int main()
{
  printf("\nSIGUSR1: %d\nSIGUSR2: %d\nSIGINT: %d\n", SIGUSR1, SIGUSR2, SIGINT);
  while (1)
    {
      signal(SIGUSR1, sighandler);
      signal(SIGUSR2, sighandler);
      signal(SIGINT, sighandler);
    }
}

static void sighandler(int signo)
{
  printf("SIGNAL CAUGHT: %d\n", signo);
  if (signo == SIGUSR1)
    {
      sigusr("who");
    }
  else if (signo == SIGUSR2)
    {
      sigusr("ps");
    }
  else if (signo == SIGINT)
    {
      sigint();
    }
}

void sigusr(char *comm)
{
  int pid = fork();
  if (pid == 0)
    {
      umask(0000);
      char *filename;
      sprintf(filename, "%s.txt", comm);
      int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 644);
      dup2(fd, STDOUT_FILENO);
      execlp(comm, comm, NULL);
    }
  else
    {
      printf("writing to %s.txt\n", comm);
    }
}

void sigint()
{
  char fnames[2][8];
  strcpy(fnames[0], "who");
  strcpy(fnames[1], "ps");
  int count;
  for (count = 0; count < 2; count++)
    {
      char *fname;
      char *buf = malloc(1024 * sizeof(char));
      strcpy(fname, fnames[count]);
      sprintf(fname, "%s.txt", fname);
      printf("Reading from %s:\n", fname);

      int fd = open(fname, O_RDONLY);
      if (fd < 0)
      	printf("%s", strerror(errno));
      else
	{	  
	  read(fd, buf, 1024);
	  printf("%s", buf); 
	}
      
    }
  exit(0);
}
