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

// execute a cd command
int do_chdir (struct cmd *cmd)
{
	if (!cmd->args[1] || cmd->args[2])
	{
		errmsg("cd: please give exactly one argument");
		return -1;
	}

	if (chdir(cmd->args[1]) < 0)
	{
		errmsg(strerror(errno));
		return 1;
	}
	return 0;
}

// Apply the redirections associated with a command.
// The redirections are permanent, so they ought to be executed
// in a child process. Exits with -1 in case of failure.

void apply (char *filename, int target, int flags)
{
	if (!filename) return;
	int fd = open(filename,flags,0666);
	if (fd >= 0 && dup2(fd,target) >= 0) { close(fd); return; }
	errmsg(strerror(errno));
	exit(-1);
}

void apply_redirects(struct cmd *cmd)
{
	apply(cmd->input, 0, O_RDONLY);
	apply(cmd->output, 1, O_WRONLY | O_CREAT | O_TRUNC);
	apply(cmd->append, 1, O_WRONLY | O_CREAT | O_APPEND);
	apply(cmd->error, 2, O_WRONLY | O_CREAT | O_TRUNC);
}

// executes a pipe command, used by execute
int execpipe (struct cmd *cmd)
{
	int rchild;
	int pipefd[2];

	// shell starts two children to execute the two sides of the pipe,
	// then waits for both of them to finish, returns status of second.
	pipe(pipefd);
	if ((rchild = fork()))
	{
		if (fork())
		{
			int r1,r2,p1;
			close(pipefd[0]);
			close(pipefd[1]);
			p1 = wait(&r1);
			wait(&r2);
			return WEXITSTATUS((p1 == rchild)? r1 : r2);
		}
		else
		{
			dup2(pipefd[1],1);
			close(pipefd[0]);
			close(pipefd[1]);
			exit(execute(cmd->left));
		}
	}
	else
	{
		dup2(pipefd[0],0);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(execute(cmd->right));
	}
}

// This is the main function.
// It executes a command parsed at the command line.
// The structure of the command is explained in output.c.
// Returns the exit code of the command in question.

int execute (struct cmd *cmd)
{
	switch (cmd->type)
	{
	    case C_PLAIN:
		if (!strcmp(cmd->args[0],"cd"))
			return do_chdir(cmd);
		else if (fork())
			// father - wait for child to terminate
			return wait_child();
		else
		{	// child - execute the command
			signal(SIGINT,SIG_DFL);	// enable Ctrl+C
			apply_redirects(cmd);   // redirections
			execvp(cmd->args[0],cmd->args); // execute

			// if we arrive here, the command has failed
			errmsg(strerror(errno));
			exit(-1);
		}

	    // les enchâinements simples ...
	    case C_SEQ:
		execute(cmd->left);
		return execute(cmd->right);

	    case C_AND:
		return execute(cmd->left) || execute(cmd->right);

	    case C_OR:
		return execute(cmd->left) && execute(cmd->right);

	    // ... et avec pipe
	    case C_PIPE:
		return execpipe(cmd);

	    case C_VOID:
		// commande en paranthèse comme (find | grep bla) 2> /dev/null
		// on génère un fils qui applique les redirections puis
		// exécute la commande en parenthèse dans cmd->left
		if (fork())
			return wait_child();
		else
		{
			apply_redirects(cmd);
			exit(execute(cmd->left));
		}
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

	// make shell ignore Ctrl+C
	signal(SIGINT,SIG_IGN);

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
