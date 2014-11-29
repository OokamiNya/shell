//CODE I DELETED FROM THE "ELSEIF" FOR '<'

/*
						  
    int f = fork();
    int status;
    if (!f){
      printf("i: %d\n", i);

      int fd = open("foo.txt", O_RDWR | O_CREAT, 0644);
      //dup2(STDIN_FILENO , fd);

      char * s = (char *) malloc(sizeof(char*));
      int r = read(fd,s , sizeof(s));
      close(fd);

      char ** part1 = (char**)malloc(sizeof(char*) * i);   
    
      int j = 0;
      while(j < i){
	part1[j] = args[j];
	j++;
      }

      part1[j] = s;
      part1[j+ 1] = 0;

      print_array(part1);

      execvp(part1[0] , part1);
      //close, and restting stdout not nessecarry b/c its a child
    }else{
      wait(&status);
    }
  }else if((i = contains(args,"cd")) != -1){
    chdir(args[1]);
    //~ doesn't  work
  }else if((i = contains(args,"exit")) != -1){
    exit(-1);
  }else{
    //fork
    int f = fork();
    int status;
    if(!f){
      //child will execute command
      print_array(args);
      execvp(args[0], args);
    }else{
      //the parent will wait until the child has finished running
      wait(&status);
    }
*/
