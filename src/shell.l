%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h" 	// generated via yacc -d
%}

%%

"\n"	{
		return NEWLINE;
	}

[ \t]	{
		/* Ignore Whitespaces */
	}

">"	{
		return GREAT;
	}

"<"	{
		return LESS;
	}

">>"	{
		return GREATGREAT;
	}

">&"	{
		return GREATAMPERSAND;
	}

">>&"	{
		return GREATGREATAMPERSAND;
	}

"|"	{
		return PIPE;
	}

"&"	{
		return AMPERSAND;
	}

[^ \t\n][^ \t\n]* {
		yylval.string_val = strdup(yytext);
		return WORD;
	}

%%
