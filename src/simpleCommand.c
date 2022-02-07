#include <string.h>
#include <stdlib.h>

#define SIMPLE_CMD_ARG_LIM 10

// Simple command: cmd_and_args
// See grammar in shell.l for more details
struct SimpleCommand {
	// Number of available arg slots
	int _numAvailableArgs;

	// Number of Arguements
	int _numArgs;

	// Arguement list
	// args[0] is command
	char **args;
};

// Debug commands forward declaration
// ----------------------------START------------------------------
void printSimpleCommand(struct SimpleCommand* simcmd);
// -----------------------------END-------------------------------

// Initialize and return a pointer to an SimpleCommand object
// By default SIMPLE_CMD_ARG_LIM slots of arguements are available
// Returns NULL if allocation error
struct SimpleCommand *simpleCmdInit() {
	struct SimpleCommand *sc;

	sc = (struct SimpleCommand*)malloc(sizeof(struct SimpleCommand));

	if(!sc) {	// malloc: Allocation error
		return NULL;
	}

	sc->_numAvailableArgs = SIMPLE_CMD_ARG_LIM;
	
	sc->_numArgs = 0;

	sc->args = (char **)malloc(sizeof(char *) * sc->_numAvailableArgs);

	if(!sc->args) {	// malloc: Allocation error
		free(sc);
		return NULL;
	}

	return sc;
}

// arg is a NULL terminated char array
// returns -1 if strdup fails
// returns -2 if realloc fails
// returns current number of args in simcmd if all goes well 
int insertArg(struct SimpleCommand *simcmd, char * arg) {
	char *dup;
	dup = strdup(arg);
	
	if(!dup) { 	// strdup: allocation error
		return -1;
	}
	
	simcmd->args[simcmd->_numArgs++] = dup;

	if(simcmd->_numArgs >= simcmd->_numAvailableArgs) {
		simcmd->_numAvailableArgs = simcmd->_numAvailableArgs + SIMPLE_CMD_ARG_LIM;
		simcmd->args = realloc(simcmd->args, simcmd->_numAvailableArgs * sizeof(char**));
		
		if (!simcmd->args) {	// realloc: allocation error
			return -2;
		}
	}
}

// Debug functions, not to be used in production and hence dont need error checking
void printSimpleCommand(struct SimpleCommand* simcmd) {
	int i;
	printf("-------------------------------------------\n");
	printf("SIMPLE COMMAND: %s\n", simcmd->args[0]);
	printf("-------------------------------------------\n");
	printf("ARGS: ");
	for(i = 1; i <= simcmd->_numArgs - 1; i++) {
		if(i == simcmd->_numArgs - 1)
			printf(" %s", simcmd->args[i]);
		else
			printf(" %s,", simcmd->args[i]);
	}
	printf("\n");
	printf("-------------------------------------------\n");
}
