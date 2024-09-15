#include "header.h"

// cd command 
int c_cd(char **args)
{
  if (args[1] == NULL) {
    // If no argument is provided, change to the home directory
    char *home = getenv("HOME");
    if (home == NULL) {
      fprintf(stderr, "c: Unable to get HOME environment variable\n");
      return 1;
    }
    if (chdir(home) != 0) {
      perror("c");
    }
  } else {
    if (chdir(args[1]) != 0) {
      perror("c");
    }
  }
  return 1;
}
