#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "src/simpleCommand.c"
#include "src/command.c"
#include "src/commandTable.c"

struct Command *cmd;
struct SimpleCommand *simcmd;
struct CmdTableEntry *cmdTabEnt;
struct CommandTable *cmdTab;

#include "lex.yy.c"
#include "y.tab.c"
#include "src/builtin.c"
#include "src/prompt.c"
#include "src/execute.c"

void dsh_loop(void);

int main() {
	// Load config files, if any.
	
	// Run command loop.
	dsh_loop();

	//Perform any shutdown/cleanup.
	
	//return EXIT_SUCCESS;
	return 0;
}
