#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

#include "global.h"

// This is the file that you should work on.

// Currently, the program can only execute simple commands and simply
// emits error messages if the user has entered something more complicated.
// To fix this, you need to complete apply_redirects() and execute().

// declaration
int execute (struct cmd *cmd);

// name of the program, to be printed in several places
#define NAME "myshell"

// Some helpful functions

void errmsg (char *msg)
{
	fprintf(stderr,"error: %s\n",msg);
}

int wait_child ()
{
	int result;
	wait(&result);
	if (result < 0) errmsg(strerror(errno));
	return WEXITSTATUS(result);
}

// apply_redirects() should modify the file descriptors for standard
// input/output/error (0/1/2) of the current process to the files
// whose names are given in cmd->input/output/error.
// append is like output but the file should be extended rather
// than overwritten.

void apply_redirects (struct cmd *cmd)
{
	if (cmd->input || cmd->output || cmd->append || cmd->error)
	{
		errmsg("I do not know how to redirect, please help me!");
		exit(-1);
	}
}

// The function execute() takes a command parsed at the command line.
// The structure of the command is explained in output.c.
// Returns the exit code of the command in question.

int execute (struct cmd *cmd)
{
	switch (cmd->type)
	{
	    case C_PLAIN:

		if (fork())
		{
			// father - wait for child to terminate
			return wait_child();
		}
		else
		{	// child - execute the command
			apply_redirects(cmd);
			execvp(cmd->args[0],cmd->args);

			// if we get to this line, the command has failed
			errmsg(strerror(errno));
			exit(-1);
		}
		break;

	    case C_SEQ:
	    case C_AND:
	    case C_OR:
	    case C_PIPE:
	    case C_VOID:
		errmsg("I do not know how to do this, please help me!");
		return -1;
	}

	// Just to satisfy the compiler
	errmsg("This cannot happen!");
	return -1;
}

int main (int argc, char **argv)
{
	char *prompt = malloc(strlen(NAME)+3);
	printf("welcome to %s!\n",NAME);
	sprintf(prompt,"%s> ",NAME);

	while (1)
	{
		char *line = readline(prompt);
		if (!line) break;	// user pressed Ctrl+D; quit shell
		if (!*line) continue;	// empty line

		add_history (line);	// add line to history

		struct cmd *cmd = parser(line);
		if (!cmd) continue;	// some parse error occurred; ignore
		//output(cmd,0);	// activate this for debugging
		execute(cmd);
	}

	printf("goodbye!\n");
	return 0;
}
