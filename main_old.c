#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void dsh_loop(void);

int main() {
	// Load config files, if any.
	
	// Run command loop.
	dsh_loop();

	//Perform any shutdown/cleanup.
	
	return EXIT_SUCCESS;
}

char *dsh_read_line(void);
char **dsh_split_line(char *);
int dsh_execute(char **);

void dsh_loop(void) {
	char *line; 						/* CLI entered command */
	char **args;						/* Tokenized command input */
	int status; 						/* Execution status */

	do {
		printf("> ");
		line = dsh_read_line();
		args = dsh_split_line(line);
		status = dsh_execute(args);

		free(line);
		free(args);
	} while (status);
}

#define DSH_RL_BUFSIZE 1024					/* Initial buffer size */

char *dsh_read_line() {
	int bufsize = DSH_RL_BUFSIZE;
	int position = 0;
	char *buffer;

	buffer = (char *) malloc(sizeof(char) * bufsize);		/* DMA of bufsize, returns pointer */
	int c;							/* Temp var to hold read char */

	if(buffer == NULL) {					
		printf("dsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while(1) {
		c = getchar();

		if(c == EOF || c == '\n') {			/* If c==EOF|'\n', current line read */
			buffer[position] = '\0';
			return buffer;
		}

		buffer[position] = c;
		position++;

		if(position >= DSH_RL_BUFSIZE) {		/* Reallc buffer if current command size > buffer size */
			bufsize += DSH_RL_BUFSIZE;
			if ((buffer = realloc(buffer, bufsize)) == NULL){
				printf("dsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

#define DSH_TOK_BUFSIZE 64
#define DSH_TOK_DELIM " \t\r\n\a"

char **dsh_split_line(char *line) {
	int bufsize = DSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if(!tokens) {
		fprintf(stderr, "dsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, DSH_TOK_DELIM);
	while(token != NULL) {
		tokens[position] = token;
		position++;
		
		if(position >= bufsize) {
			bufsize += DSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if(!tokens) {
				fprintf(stderr, "dsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		
		token = strtok(NULL, DSH_TOK_DELIM);
	}

	tokens[position] = NULL;
	return tokens;
}

int dsh_launch (char **args) {
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		// Child process
		if (execvp(args[0], args) == -1) {
			perror("dsh");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// Error forking
		perror("dsh");
	} else {
		// Parent process
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

int dsh_cd(char **args);
int dsh_help(char **args);
int dsh_exit(char **args);

char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};

int (*builtin_func[]) (char **) = {
	dsh_cd,
	dsh_help,
	dsh_exit
};

int dsh_num_builtins() {
	return sizeof(builtin_str)/sizeof(char *);
}

int dsh_cd(char **args) {
	if(args[1] == NULL) {
		fprintf(stderr, "dsh: expected arguement to \"cd\"\n");
	} else {
		if (chdir(args[1]) != 0 ){
			perror("dsh");
		}
	}
	return 1;
}

int dsh_help(char **args) {
	int i;
	printf("Dhillon Shell\n");
	printf("Type program names and arguements, and hit enter.\n");
	printf("The following are built in:\n");

	for (i = 0; i < dsh_num_builtins(); i++) {
		printf("	%s\n", builtin_str[i]);
	}

	printf("Use the man command for information on other programs.\n");
	return 1;
}

int dsh_exit(char **args) {
	return 0;
}

int dsh_execute(char **args) {
	int i;

	if(args[0] == NULL) {
		return 1;
	}

	for (i = 0; i < dsh_num_builtins(); i++) {
		if(strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}
	return dsh_launch(args);
}
