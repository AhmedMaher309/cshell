#include <sys/wait.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define C_READ_BUF_SIZE 1024
#define C_TOK_BUFSIZE 64



int c_cd(char ** args);
