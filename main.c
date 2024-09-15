#include <sys/wait.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define C_READ_BUF_SIZE 1024
#define C_TOK_BUFSIZE 64


void welcome()
{	
  printf("============================================================\n");
  printf("|                                                          |\n");
  printf("|                 Welcome to C-Shell!                      |\n");
  printf("|                                                          |\n");
  printf("|  This is CShell, my own shell.                           |\n");
  printf("|  You are using it at your own risk!                      |\n");
  printf("|                                                          |\n");
  printf("|            ⚠️   Proceed with caution ⚠️                    |\n");
  printf("|                                                          |\n");
  printf("|        Bugs are just misunderstood features              |\n");
  printf("|                                                          |\n");
  printf("============================================================\n");
}


char * cread_line()
{
	int bufsize = C_READ_BUF_SIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int ch;

	if (!buffer) {
    fprintf(stderr, "C: allocation error\n");
    exit(EXIT_FAILURE);
  }

	while(1){
		
		//read character
		//used int for the character reading because EOF is an integer
    ch = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (ch == EOF || ch == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = ch;
    }
    position++;
	}

	return buffer;
}


char **csplit_line(char *line) 
{
    int bufsize = C_TOK_BUFSIZE;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;
    char *delim = "\t\r\n\a";

    if (!tokens) {
        fprintf(stderr, "C: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, delim);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += C_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "C: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delim);
    }
    tokens[position] = NULL;
    return tokens;
}


int c_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    //inside child
    if (execvp(args[0], args) == -1) {
      perror("c");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    //error in forking
    perror("c");
  } else {
    //inside parent
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}


int c_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  return c_launch(args);
}


void cloop() {
    char *line;
    char **args;
    int stat = 1;

    do {
        printf("$ ");
        line = cread_line();
        args = csplit_line(line);
        stat = c_execute(args);
        free(line);
        free(args);
    } while (stat);
}


int main(){

	welcome();

	// the command loop
	cloop();

	return EXIT_SUCCESS;
}
