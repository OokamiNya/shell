#define DELIMS ";|"
#define F_IN '<'
#define F_OUT '>'
#define TEMP_FILE "/tmp/MESS_STUFF"
#define COMM_LEN 64

typedef struct
{
  char *execstr;
  char *f_in;
  char *f_out;
  int piped;
} command_t;

int parse(char **in, command_t **cmds, int num_cmds);
char *strsep_save(char **stringp, char *delim, char *found);
int count_delims(char *in, char *delims);
