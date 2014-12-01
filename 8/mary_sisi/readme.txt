
1.) features
	-Forks and executes commands
	-Parses multiple commands on one line
	-Redirects using > , >> , <
	-Pipes
	-parse works with more than one space


2.)attempted
	-there must be at least one space between special characters (; ,>, >>, <, |)
	-only one pipe per line works
	-cd doesn’t account for ~
	-sig handling ^C doesn’t work more than once
	
3.)bugs
	-there is definitely some stuff wrong with memory allocation
	(not everything is freed tight, and if something super long is typed it breaks)
	-when gibberish is entered it doesn’t come up as an error
	(it just prints the prompt again)
	-if you go into emacs form the shell then ^x out, it quits the whole shell
