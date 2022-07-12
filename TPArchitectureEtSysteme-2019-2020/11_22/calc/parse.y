%{

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "global.h"

struct expr* mainexpr;
int yylex();
void yyerror (char*);

%}

%union 
{
	struct expr* expr;
	int value;
}

%token <value> NATURAL
%type <expr> expr

%left PLUS MINUS
%left MULT

%%

main    : expr { mainexpr = $1; }

expr    : NATURAL
	  {
		$$ = calloc(1,sizeof(struct expr));
		$$->type = C_CONST;
		$$->value = $1;
	  }
	| expr PLUS expr
	  {
		$$ = calloc(1,sizeof(struct expr));
		$$->type = C_PLUS;
		$$->left = $1;
		$$->right = $3;
	  }
	| expr MINUS expr
	  {
		$$ = calloc(1,sizeof(struct expr));
		$$->type = C_MINUS;
		$$->left = $1;
		$$->right = $3;
	  }
	| expr MULT expr
	  {
		$$ = calloc(1,sizeof(struct expr));
		$$->type = C_MULT;
		$$->left = $1;
		$$->right = $3;
	  }
	| '(' expr ')' { $$ = $2; }

%%

#include "lex.c"

void yyerror (char *info)
{ 
	fprintf(stderr,"cannot parse input: %s; discarded\n",info);
}

struct expr* parser (char *command)
{
	yy_scan_string(command);
	if (yyparse()) return NULL;
	return mainexpr;
}
