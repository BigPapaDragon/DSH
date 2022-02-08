// Forward declaration for currently implemented built-in commands
int dsh_cd (char **args);
int dsh_help (char **args);
int dsh_exit (char **args);

// Array of built-in commands
char *builtIn[] = {
	"cd",
	"help",
	"exit"	
};

// Array of corresponding built-in command functions
int (*builtInFunc[]) (char **args) = {
	&dsh_cd,
	&dsh_help,
	&dsh_exit
};

// Returns the number of current implement built-in commands
int dshNumBuiltIns () {
	return (sizeof(builtIn)/sizeof(char *));
}

// Always returns 1
// Changes directory using chdir() system call
// args is an initialized SimpleCommand 
int dsh_cd (char **args) {
	if(args[1] == NULL) {
		fprintf(stderr, "cd: incorrect usage\n");
	} else if (chdir(args[1]) != 0) {
		perror("dsh");
	}
	return 1;
}

// Always returns 1
// Dumps help message
// args is an initialized SimpleCommand 
int dsh_help (char **args) {
	printf("Dhillon Shell Help\n");
	printf("Following commands are built-in commands: \n");
	
	int i;
	
	for(i = 0; i < dshNumBuiltIns(); i++) {
		printf("	%d. %s\n", i+1, builtIn[i]);
	}

	printf("Rest all other commands are not built-in. Please use man page for information on them\n");

	return 1;
}

// returns 0 status
// exits shell
// args is an initialized SimpleCommand 
int dsh_exit (char **args)  {
	return 0;
}

// returns 1 if args is a built-in command
// returns 0 otherwise
// args is an initialized SimpleCommand 
int isBuiltInCmd(char **args) {
	int i;

	for(i = 0; i < dshNumBuiltIns(); i++) {
		if(strcmp(args[0], builtIn[i]) == 0) {
			return 1;
		}
	}

	return 0;
}
