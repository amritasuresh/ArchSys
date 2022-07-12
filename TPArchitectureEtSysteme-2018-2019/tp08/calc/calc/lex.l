%{

#include <string.h>

%}

%option nounput
%option noinput
%option noyywrap

%%

"("		{ return '('; }
")"             { return ')'; }
"+"             { return PLUS; }
"*"             { return MULT; }
"-"             { return MINUS; }

[0-9]*  {
		  yylval.value = atoi(yytext);
		  return NATURAL;
		}

.               ;
