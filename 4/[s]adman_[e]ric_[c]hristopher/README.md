Our shell implements basic features including basic commands, such as ls, cd, exit, cat. 
=======================
It also includes simple redirection using > , < , and pipes.
=======================
But not only that, it also comes with the special ability to seperate commands with semicolons! 
=======================

FAILED FEATURES:
  We had tried to implement >> and << thinking it should be similiar to > and <, but with the way we implemented redirection,     trying to add >> and << somehow broke the original redirection commands.

BUGS:
  Occasionally, you will have to use exit multiple times, or wait a while after inputting exit for the shell to actually quit.
  
  If you try to break and mess around with redirection, such as putting multiple pipes and nonsense, your shell will be spammed   with faces, as you deserve it.
  
Function Headers:
  
