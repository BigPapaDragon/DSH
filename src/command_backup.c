#define SIMPLE_CMD_ARG_LIM 10
#define CMD_SIMPLE_CMD_LIM 5
#define CMD_HIS_TABLE_LIM 1000

#include <string.h>
#include <stdlib.h>

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

// Command: command_line 
// See grammar in shell.l for more details
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

long unsigned CURR_CMD_ID = 0;

// Command table entry/Row
// Saves the id of command and corresponding command
struct CmdTableEntry {
	// ID of the command in the shell
	int _id;
	// Pointer to the command
	struct Command *_cmd;
}

// Command Table
// Maintains history of the encountered commands
struct CommandTable {
	// Number of available Command slots
	int _numAvailableCommand;

	// Number of Commands:
	int _numCommands;

	// Command array
	struct CmdTableEntry **tableEntry;
};

// Initialize and return a pointer to an CommandTable object
// By default CMD_HIS_TABLE_LIM slots of command are available
// Returns NULL if allocation error
struct CommandTable *cmdTableInit() {
	struct CommandTable *cmdTab;

	cmdTab = (struct CommandTable *) malloc(sizeof(struct CommandTable));

	if (!cmdTab) // malloc: allocation error
		return NULL;

	cmdTab->_numAvailableCommand = CMD_HIS_TABLE_LIM;

	cmdTab->_numCommands = 0;

	cmdTab->tableEntry = (struct CmdTableEntry **) malloc (sizeof(struct CmdTableEntry *) * CMD_HIS_TABLE_LIM);

	if(!cmdTab->tableEntry) // malloc: allocation error
		return NULL;

	return cmdTab;
}

// Initialize and return a pointer to an ComTableEntry object
// cmd is an initialized Command object
// Returns NULL if allocation error
struct CmdTableEntry *cmdTabEntryInit(struct Command *cmd) {
	struct CmdTableEntry *cmdTabEnt;

	cmdTabEnt = (struct CmdTableEntry *) malloc(sizeof(struct CmdTableEntry));
	
	if(!cmdTabEnt) // malloc: allocation error
		return NULL;
	
	cmdTabEnt->_id = CURR_CMD_ID++;
	cmdTabEnt->_cmd = cmd;

	return cmdTabEnt;
}

// cmdEntry is an initialized CmdTableEntry Object
// return -1 if allocation error
// returns number of Command Table Entries if all goes well
int insertCommand (struct CommandTable *cmdTab, struct CmdTableEntry *cmdEntry) {
	cmdTab->tableEntry[cmdTab->_numCommands++] = cmdEntry;

	if(cmdTab->_numCommands >= cmdTab->_numAvailableCommand) {
		cmdTab->_numAvailableCommand = cmdTab->_numAvailableCommand + CMD_HIS_TABLE_LIM;
		cmdTab->tableEntry = realloc(cmdTab->CmdTableEntry, cmdTab->_numAvailableCommand * sizeof(struct CmdTableEntry*));

		if(!cmdTab->tableEntry) // realloc: allocation error
			return -1;
	}

	return cmdTab->_numCommands;
}

// Debug commands forward declaration
// ----------------------------START------------------------------
void printSimpleCommand(struct SimpleCommand* simcmd);
void printCommand(struct Command* cmd);
void printCommandStats(struct Command* cmd);
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

// Initialize and return a pointer to an Command object
// By default CMD_SIMPLE_CMD_LIM slots of SimpleCommand are available
// Returns NULL if allocation error
struct Command *cmdInit() {
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

void printCommand(struct Command* cmd) {
	int i;
	printf("=============================================\n");
	printf("=============================================\n");
	printf("COMMAND LOG\n");
	printf("=============================================\n");
	printf("Number of simple cmd: %d\n", cmd->_numSimpleCmds);
	for (i = 0; i <= cmd->_numSimpleCmds - 1; i++)
		printSimpleCommand(cmd->simpleCmds[i]);
	printf("=============================================\n");
	printf("INFILE: %s\n", cmd->_inFile);
	printf("=============================================\n");
	printf("OUTFILE: %s\n", cmd->_outFile);
	printf("=============================================\n");
	printf("ERRFILE: %s\n", cmd->_errFile);
	printf("=============================================\n");
	printf("BACKGROUND: %d\n", cmd->_backGround);
	printf("=============================================\n");
	printf("Command Stats: \n");
	printCommandStats(cmd);
	printf("=============================================\n");
}

void printCommandStats(struct Command* cmd) {
	
	printf("Cmd Stats: cmd->_numAvailableSimpleCmds = %d\n", cmd->_numAvailableSimpleCmds);
	printf("Cmd Stats: int _numSimpleCmds = %d\n", cmd->_numSimpleCmds);
	printf("Cmd Stats: char *_inFile = %s\n", cmd->_inFile);
	printf("Cmd Stats: char *_outFile = %s\n", cmd->_outFile);
	printf("Cmd Stats: char *_errFile = %s\n", cmd->_errFile);
	printf("Cmd Stats: int _backGround = %d\n", cmd->_backGround);
}
