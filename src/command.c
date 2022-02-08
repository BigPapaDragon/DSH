#define CMD_SIMPLE_CMD_LIM 5

// Command: command_line 
// See grammar in shell.l for more details
struct Command {
	// Number of available SimpleCommand Slots
	int _numAvailableSimpleCmds;

	// Number of Simple Command, always points to the next free location in Simple Command array
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


// Debug commands forward declaration
// ----------------------------START------------------------------
void printCommand(struct Command* cmd);
void printCommandStats(struct Command* cmd);
// -----------------------------END-------------------------------

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

	// cmd->simpleCmds is null terminated list of SimpleCommand objects
	cmd->simpleCmds[cmd->_numSimpleCmds] = NULL;

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

void printCommandLine (struct Command *cmd) {
	int i, j;

	for(i = 0; i < cmd->_numSimpleCmds; i++){
		for (j = 0; j < cmd->simpleCmds[i]->_numArgs; j++){
			printf(" %s", cmd->simpleCmds[i]->args[j]);
		}
		
		if(i != 0)
			printf(" |");
	}

	if(cmd->_inFile)
		printf(" < %s", cmd->_inFile);

	if(cmd->_outFile)
		printf(" > %s", cmd->_outFile);

	if(cmd->_errFile)
		printf(" >& %s", cmd->_errFile);

	if(cmd->_backGround)
		printf(" &");

	printf("\n");
}
