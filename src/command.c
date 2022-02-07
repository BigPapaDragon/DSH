#define SIMPLE_CMD_ARG_LIM 10
#define CMD_SIMPLE_CMD_LIM 5

#include <string.h>
#include <stdlib.h>

struct SimpleCommand {
	// Number of available arg slots
	int _numAvailableArgs;

	// Number of Arguements
	int _numArgs;

	// Arguement list
	// args[0] is command
	char **args;
};

struct Command {
	// Number of available SimpleCommand Slots
	int _numAvailableSimpleCmds;

	// Number of Simple Command
	int _numSimpleCmds;

	// Array of Simple Command
	struct SimpleCommand **simpleCmds;

	// Input Re-direction
	char *_inFile;

	// Output Re-direction
	char *_outFile;

	// Error Re-direction
	char *_errFile;

	// Background
	int _backGround;
};

struct CommandTable {
	// Number of available Command slots
	int _numAvailableCommand;

	// Number of Commands:
	int _numCommands;

	// Command array
	struct Command *cmds;
};

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

// Initialize and return a pointer to an Command object
// By default CMD_SIMPLE_CMD_LIM slots of SimpleCommand are available
// Returns NULL if allocation error
struct Command *CmdInit() {
	struct Command *cmd;

	cmd = (struct Command *)malloc(sizeof(struct Command));
	
	if(!cmd) {	// malloc: allocation error
		return NULL;
	}
	
	cmd->_numAvailableSimpleCmds = CMD_SIMPLE_CMD_LIM;

	cmd->simpleCmds = (struct SimpleCommand**) malloc(sizeof(struct SimpleCommand*) * cmd-> _numAvailableSimpleCmds);
	
	cmd->_numSimpleCmds = 0;

	if(!cmd->simpleCmds) {	// malloc: allocation error
		return NULL;
	}

	cmd->_inFile = NULL;
	cmd->_outFile = NULL;
	cmd->_errFile = NULL;
	cmd->_backGround = 0;

	return cmd;
}

// simcmd is an initialized SimpleCommand Object
// return -1 if allocation error
// returns number of available SimpleCommands if all goes well
int insertSimpleCmd (struct Command *cmd, struct SimpleCommand *simcmd) {
	
	cmd->simpleCmds[cmd->_numSimpleCmds++] = simcmd;
	
	if(cmd->_numSimpleCmds >= cmd->_numAvailableSimpleCmds) {
		cmd->_numAvailableSimpleCmds = cmd->_numAvailableSimpleCmds + CMD_SIMPLE_CMD_LIM;
		cmd->simpleCmds = realloc(cmd->simpleCmds, cmd->_numAvailableSimpleCmds * sizeof(struct SimpleCommand*));
		
		if (!cmd->simpleCmds) {	// realloc: allocation error
			return -1;
		}
	}

	return cmd->_numSimpleCmds;
}

// inputFile is a NULL terminated char array
// Returns -1 if strdup allocation error
// Returns 1 if all goes well
int setInputRedir (struct Command *cmd, char *inputFile) {
	cmd->_inFile = strdup(inputFile);
	if(!cmd->_inFile) {	// strdup: Allocation error
		return -1;
	}
	return 0;
}

// outputFile is a NULL terminated char array
// Returns -1 if strdup allocation error
// Returns 1 if al goes well
int setOutputRedir (struct Command *cmd, char *outputFile) {
	cmd->_outFile = strdup(outputFile);
	if(!cmd->_outFile) {	// strdup: Allocation error
		return -1;
	}
	return 0;
}

// errorFile is a NULL terminated char array
// Returns -1 if strdup allocation error
// Returns 1 if al goes well
int setErrorFile (struct Command *cmd, char *errorFile) {
	cmd->_errFile = strdup(errorFile);
	if(!cmd->_errFile) {	// strdup: Allocation error
		return -1;
	}
	return 0;
}

int setBackground (struct Command *cmd) {
	cmd->_backGround = 1;
}

int main() {
	printf("All_well \n");
}
