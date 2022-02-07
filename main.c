#include <stdio.h>

#include "src/commandTable.c"

struct Command *cmd;
struct SimpleCommand *simcmd;
struct CmdTableEntry *cmdTabEnt;
struct CommandTable *cmdTab;

#include "lex.yy.c"
#include "y.tab.c"

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
	cmdTab = cmdTableInit();

	int i = 0;	

	do {
		printf("> ");
		yyparse();
		
		//status = dsh_execute(args);
		cmdTabEnt = cmdTabEntryInit(cmd);
		insertCommand(cmdTab, cmdTabEnt);

		// Set cmd and simcmd to NULL to recieve the next command.
		cmd = NULL;
		simcmd = NULL;
		cmdTabEnt = NULL;
		
		i++;

	} while (i < 5);//status);
	
	printCommands(cmdTab);
	
}

