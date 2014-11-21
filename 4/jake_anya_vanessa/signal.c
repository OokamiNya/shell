#include <signal.h>

static void sighandler(int signo){
  if (signo == SIGINT){
    kill(getpid(), SIGKILL);
  }
}
