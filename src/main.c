#include <stdio.h>
#include "lex.yy.c"
#include "y.tab.c"
#include "src/command.c"

void dsh_loop(void);

int main() {
	// Load config files, if any.
	
	// Run command loop.
	dsh_loop();

	//Perform any shutdown/cleanup.
	
	//return EXIT_SUCCESS;
	return 0;
}

void dsh_loop(void) {
	
	struct Command *cmd;
	struct SimpleCommand *simcmd;
	
	do {
		printf("> ");
		yyparse();
		//status = dsh_execute(args);
	} while (1);//status);
}

