#include <stdio.h>
#include <unistd.h>
#include <termios.h>
int main()
{

	char buf[8];
	int curline;
	char cmd[]="echo -e '\033[6n'";
	struct termios save,raw;
	tcgetattr(0,&save);
	cfmakeraw(&raw); tcsetattr(0,TCSANOW,&raw);
	if (isatty(fileno(stdin))) 
	{
		write(1,cmd,sizeof(cmd));
		read (0 ,buf ,sizeof(buf));

		/* It doesn't work!!?
		   sscanf(buf,"%d",curline);
		   printf("\n\rCurrent Line: %d\n\r" , curline);
		 */

		printf("\n\rCurrent Line: %c%c\n\r" , buf[2] ,buf[3] );
	}
	tcsetattr(0,TCSANOW,&save);
	return 0;
}
