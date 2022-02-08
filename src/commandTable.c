#define CMD_HIS_TABLE_LIM 1000

long unsigned CURR_CMD_ID = 0;

// Command table entry/Row
// Saves the id of command and corresponding command
struct CmdTableEntry {
	// ID of the command in the shell
	int _id;
	// Pointer to the command
	struct Command *_cmd;
};

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

// Debug commands forward declaration
// ----------------------------START------------------------------
// -----------------------------END-------------------------------

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
		cmdTab->tableEntry = realloc(cmdTab->tableEntry, cmdTab->_numAvailableCommand * sizeof(struct CmdTableEntry*));

		if(!cmdTab->tableEntry) // realloc: allocation error
			return -1;
	}

	return cmdTab->_numCommands;
}

// Debug functions, not to be used in production and hence dont need error checking
void printCommands (struct CommandTable *cmdTab) {
	int i;

	for(i = 0; i < cmdTab->_numCommands; i++) {
		printf("ID: %4d,", cmdTab->tableEntry[i]->_id);
		printCommandLine(cmdTab->tableEntry[i]->_cmd);
	}
}
