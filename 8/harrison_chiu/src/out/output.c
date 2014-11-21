#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include "output.h"

char *prompt()
{

}

int main(int argc, char **argv)
{
  char wd_raw[MDR];
  if (!check_input(argc, argv, wd_raw))
    {
      DIR *wd;
      if (wd = opendir(wd_raw))
	{
	  int total = 0;
	  struct dirent *file;
	  while (file = readdir(wd))
	    {
	      if (print_info(file, wd_raw, file->d_name, &total) == -1)
		{  
		  printf("%s\n", strerror(errno));
		  return -1;
		}
	    }
	  printf("total size: %8d\n", total);
	  return 0;
	}
    }
  printf("%s\n", strerror(errno));
  return -1;
}

int check_input(int argc, char **argv, char wd_raw[])
{
  if (argc == 1)
    {
      getcwd(wd_raw, MDR);
      if (!wd_raw)
	{
	  return -1;
	}
    }
  else
    {
      strcpy(wd_raw, argv[1]);
      if (!wd_raw)
	{
	  return -1;
	}
    }
  return 0;
}

int print_info(struct dirent *file, char path[], char filename[], int *total)
{
  struct stat *info;
  char fullpath[MDR];
  int ret;

  sprintf(fullpath, "%s%s%s",
	  path, 
	  '/' == path[strlen(path)] ? "" : "/", //true if last char is slash
	  filename);

  info = malloc(sizeof(struct stat));
  if (stat(fullpath, info) != -1)
    {
      printf("%s | size: %8d | name: %s\n", 
	     file->d_type == DT_DIR ? "dir" : "   ",
	     (int)info->st_size, 
	     file->d_name);
      *total += (int)info->st_size;
    }
  else
    {
      ret = -1;
    }
  free(info);

  return ret;
}
