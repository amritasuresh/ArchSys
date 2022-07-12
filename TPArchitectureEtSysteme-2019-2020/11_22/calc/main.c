#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <glob.h>

#include "global.h"

// name of the program, to be printed in several places
#define NAME "calc"

int compute (struct expr *expr)
{
	switch (expr->type)
	{
	    case C_CONST:
		return expr->value;

	    case C_PLUS:
		return compute(expr->left) + compute(expr->right);

	    case C_MINUS:
		return compute(expr->left) - compute(expr->right);

	    case C_MULT:
		return compute(expr->left) * compute(expr->right);

	    default: // can't happen
		printf("something strange happened\n");
		break;
	}

	// can't happen, just to satisfy the compiler
	return 0;
}

int main (int argc, char **argv)
{
	char *prompt = malloc(strlen(NAME)+3);
	sprintf(prompt,"%s> ",NAME);

	while (1)
	{
		char *line = readline(prompt);
		if (!line) break;	// user pressed Ctrl+D; quit shell
		if (!*line) continue;	// empty line

		add_history(line);	// add line to history

		struct expr *expr = parser(line);
		if (!expr) continue;	// some parse error occurred; ignore
		
		printf("The result is %d!\n",compute(expr));
	}

	printf("goodbye!\n");
	return 0;
}
