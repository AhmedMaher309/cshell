#include <sys/wait.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define C_READ_BUF_SIZE 1024
#define C_TOK_BUFSIZE 64

// ANSI color codes
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"



int c_cd(char ** args);
int c_exit(char ** args);
