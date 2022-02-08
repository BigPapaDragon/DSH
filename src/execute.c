// Launches non built-in commands
// cmd is an initialized Command object
// Returns the number of simple commands executed
int dsh_launch(struct Command *cmd) {
	// Temp file descriptors to store stdin and stdout
	int temp_in,temp_out;
	temp_in = dup(0);
	temp_out = dup(1);

	// File descriptors for the commands in the pipeline
	int fdin, fdout;
	int fdpipe[2];
	
	// Redirecting input
	if(cmd->_inFile)  // if _inFile exists, make fdin point to _inFile
		fdin = open(cmd->_inFile, O_RDONLY);
	else  // Else make fdin point to stdin
		fdin = dup(temp_in);
	
	// Process id and process state
	int pid, pid_state;

	int i;
	for (i = 0; i < cmd->_numSimpleCmds; i++) {
		
		// Make file descriptor 0 point to the correct input source
		dup2(fdin, 0);
		close(fdin);

		if( i == cmd->_numSimpleCmds - 1) { 
			// If last command, check for cmd->_outFile
			if(cmd->_outFile)
				fdout = open(cmd->_outFile, O_WRONLY);
			else // if NULL, set output to stdout
				fdout = dup(temp_out);
		} else {
			// If not last, make pipe 
			pipe(fdpipe);
			fdin = fdpipe[0];
			fdout = fdpipe[1];
		}
		
		// Reorganize output for the next SimpleCommand
		dup2(fdout, 1);
		close(fdout);

		// Create new process
		pid = fork();

		if(pid == 0)  {
			// Child process
			
			// Check whether the command is a built-In command
			// If yes, call built-in with execvp 
			if(isBuiltInCmd(cmd->simpleCmds[i]->args)) {
				for(i = 0; i < dshNumBuiltIns(); i++) {
					if(strcmp(cmd->simpleCmds[0]->args[0], builtIn[i]) == 0) {
						if(execvp(cmd->simpleCmds[i]->args[0], cmd->simpleCmds[i]->args) == -1) {
							perror("dsh");
						}
					}
				}
			}
		       	// If no, call normally with execvp
			else {
				
				if(execvp(cmd->simpleCmds[i]->args[0], cmd->simpleCmds[i]->args) == -1) {
					perror("dsh");
				}
			}
			exit(EXIT_FAILURE);
		} else if (pid < 0) {
			// Error forking
			perror("dsh");
		} else {
			// Parent process
			if(!cmd->_backGround) { // if cmd->_backGround is set, dont wait for child process to finish
				do {
					waitpid(pid, &pid_state, WUNTRACED);
				} while (!WIFEXITED(pid_state) && !WIFSIGNALED(pid_state));
			}
		}
	}
	
	dup2(temp_out, 1);
	dup2(temp_in, 0);
	close(temp_out);
	close(temp_in);

	return i;
}

// cmd is an initialized Command Object
// returns the command execution status code
// StackOverflow:
// the shell runs each command from a pipeline in a separate process, no matter if it's an external command or a built-in. So yes, history | ... is spawning a new process, even if it's just another instance of bash.
int dsh_execute (struct Command *cmd) {
	
	int i;

	// Single command, no pipeline
	// Check whether cmd is an builtin command
	if (cmd->_numSimpleCmds == 1) {
		for(i = 0; i < dshNumBuiltIns(); i++) {
			if(strcmp(cmd->simpleCmds[0]->args[0], builtIn[i]) == 0) {
				return builtInFunc[i](cmd->simpleCmds[0]->args);
			}
		}
	}
	
	// run dsh_launch if cmd is a pipeline or cmd is not a built-in command
	return dsh_launch(cmd);
}

// Main loop for the shell
void dsh_loop(void) {
	// Initialize Command Table
	cmdTab = cmdTableInit();
	
	// Execution Status, number of simple commands in Command Line
	int execStatus, numSimCmd;
	
	execStatus = 1;
	
	do {
		// Display Prompt
		prompt();

		// Parse input and return number of Simple Commands identified
		numSimCmd = yyparse();
		
		// If empty command, return to prompt
		if(!numSimCmd)
			continue;

		// Execute command and return execution status
		execStatus = dsh_execute(cmd);
		
		// Save the command in Command Table for history
		cmdTabEnt = cmdTabEntryInit(cmd);
		insertCommand(cmdTab, cmdTabEnt);

		// Clear data-structures for next command in the shell
		cmd = NULL;
		simcmd = NULL;
		cmdTabEnt = NULL;
	} while (execStatus);
}

