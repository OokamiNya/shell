#define DELIMS ";"
#define COMM_LEN 64

typedef struct
{
  char *execstr;
} command_t;

int parse(char *in, command_t **cmds, int num_cmds);
char *strsep_save(char *stringp, char *delim, char *found);
int count_delims(char *in, char *delims);
