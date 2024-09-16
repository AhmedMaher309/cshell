#include "header.h"


void welcome()
{	
  printf(ANSI_COLOR_CYAN
         "============================================================\n"
         "|                                                          |\n"
         "|                 Welcome to " ANSI_COLOR_YELLOW "C-Shell!" ANSI_COLOR_CYAN "                      |\n"
         "|                                                          |\n"
         "|  This is CShell, my own shell.                           |\n"
         "|  You are using it at your own risk!                      |\n"
         "|                                                          |\n"
         "|                                                          |\n"
         "|        * Bugs are just misunderstood features *          |\n"
         "|                                                          |\n"
         "============================================================\n"
         ANSI_COLOR_RESET);
}

// function to print the current directory in colors
void print_prompt() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf(ANSI_COLOR_GREEN "cshell" ANSI_COLOR_RESET ":" ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "$ ", cwd);
    } else {
        printf(ANSI_COLOR_GREEN "cshell" ANSI_COLOR_RESET "$ ");
    }
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
    char *delim = "\t\r\n\a ";

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
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    //inside child
    if (execvp(args[0], args) == -1) {
			fprintf(stderr, ANSI_COLOR_RED "c: %s: command not found\n" ANSI_COLOR_RESET, args[0]);
    }
    exit(EXIT_FAILURE);

  } else if (pid < 0) {
    //error in forking
    perror(ANSI_COLOR_RED "c" ANSI_COLOR_RESET);

  } else {
    //inside parent
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}



typedef int(*Command)(char **);
Command commands[] = {c_cd, c_exit};
const char *command_names[] = {"cd", "exit"};

int c_execute(char **args)
{
    if (args[0] == NULL) {
        return 1;
    }
    for(size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++){
        if(strcmp(args[0], command_names[i]) == 0){
            return commands[i](args);
        }
    }
    return c_launch(args);
}


void cloop() {
    char *line;
    char **args;
    int stat = 1;

    do {
			  print_prompt();
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
