#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* default memblock size for CLI input*/
#define CHARBLOCK 128
#define ARGBLOCK 10

/* Flags for read commands */
struct cmdFlags {
	unsigned int inredir	: 1;			/* Input redirection */
	unsigned int outredir	: 1;			/* Output redirection */
	unsigned int pipe	: 1;			/* Pipe usage in command */
	unsigned int bkgnd	: 1;			/* execute in background */
};

/* Input command structure */
struct cmdCLI {
	char *cmd;				/* command name */
	char **args;				/* arguements */
	int numArgs;				/* Number of arguements */
	struct cmdFlags flags;			/* Flags */
};

/* Forward declaration for read_line */
int isterm(char c);
int getch();
void ungetch(char c);

/* read line from CLI and save in line */
/* Returns pointer to the read line */
/* Returns NULL if error encountered */
char *read_line () {
	int c, position = 0;			/* c to read char, position to track curr position in line */
	int linesize = CHARBLOCK;		/* size of line to be allocated */
	char *line;

	line = (char *) malloc(sizeof(char) * linesize);	/* Allocate char array of size linesize */
	
	if(!line)						/* Allocation error */
		return NULL;

	while (isspace(c = getch()))				/* Remove all leading white space */
		;

	if(c == EOF) {						/* if first char is EOF, return NULL */ 
		printf("DEBUG: Looping\n");
		return NULL;
	}
	
	ungetch(c);						/* Unget first non EOF char */

	while(!isterm(c = getch())) {				/* loop till c = termination char */
		line[position++] = c;				

		if(position >= linesize) {			/* Realloc line char memory if position > linesize */
			linesize += CHARBLOCK;
			line = realloc(line, linesize);

			if(!line)				/* Allocation error */
				return NULL;
		}
	}
	
	line[position] = '\0';

	return line;
}

/* Forward declaration for parse() */
int isspecial(char c);

struct cmdCLI *parse(char *line) {
	struct cmdCLI *currcmd = malloc(sizeof(struct cmdCLI));		/* Current command struct */
	
	/* Initialize currcmd instance */
	currcmd->flags.inredir = 
		currcmd->flags.outredir = 
		currcmd->flags.pipe = 
		currcmd->flags.bkgnd = 0;

	currcmd->numArgs = 0;

	int c;								/* Temp int to read char */
	char *p = line;							/* Char pointer to traverse the string */
	
	//--------------------------------------------Reading CMD----------------------------------------------------
	/* No leading white space, since leading white spaces were removed by read_line*/
	while (!isspecial(c = *p) && c != '\0')				/* Cmd is till first special character */
		p++;
	int n = 0;							/* int to iterate over currcmd->cmd */
	currcmd->cmd = (char *) malloc(sizeof(char) * (p - line + 1));	/* Allocate space to store cmd name */
	
	while (*line != *p) 						/* Copy command name from line to currcmd.cmd */
		currcmd->cmd[n++] = *line++;
	
	currcmd->cmd[n] = '\0';						/* Terminating NULL for cmd name */
	//-----------------------------------------------------------------------------------------------------------
	
	//--------------------------------------------Reading Args---------------------------------------------------
	char **args;							/* Pointer to Array of arguements */
	int args_size = ARGBLOCK;					/* Array of Arguements size */
	int args_p = 0;							/* Array of arguements count */
	int args_str_n = 0;						/* int to iterate over individual arg str */

	args = (char **) malloc(sizeof(char *) * args_size);		/* Allocate space to store arguements */
	
	while((c = *p) != '\0') {					
		if(isspecial(c)) {					/* Check whether c == '>' or '<' or '|' or ' ' */
			if ((p - line) > 1) {				/* is (p - line) > 1, that means no consecutive special characters, save the argument */
				args[args_p] = (char *) malloc (sizeof(char) * (p - line));	/* Request space of the required lenght */
				++line;					/* line points to the last read special character, inrementing to pointo next char */
				
				while ((p - line) != 0)
					(args[args_p])[args_str_n++] = *line++;	/* Save arguement */
				
				(args[args_p])[args_str_n] = '\0';	/* Add terminating '\0' */
				
				args_p++;				/* increment num args */
				args_str_n = 0;				/* reset args string counter */
				
				if(args_p >= args_size) {		/* is num args > args_size, reallocate bigger memory block */
					args_size += ARGBLOCK;
					args = realloc(args, args_size);

					if(args)			/* Allocation error */
						return NULL;
				}
			}

			switch(c) {					/* check special character and update flags */
				case '<':
					if(!currcmd->flags.inredir)
						currcmd->flags.inredir = 1;
					else
						printf("Error\n");
					break;
				case '>':
					if(!currcmd->flags.outredir)
						currcmd->flags.outredir = 1;
					else
						printf("Error\n");
					break;
				case '|':
					currcmd->flags.pipe = 1;
					break;
				case ' ':
					break;
				default:
					printf("Error");
					break;
			}
			line = p;					/*  Prep line for the next iteration of the while loop */
		}
		p++;							/* Increment p to point to next char */
	}

	currcmd->numArgs = args_p;
	currcmd->args = args;
	
	return currcmd;							/* Return tokenized command */
}

void printCmd (struct cmdCLI *currcmd) {
	printf("\n");
	printf("==========================================================\n");
	printf("###### Command: %s\n", currcmd->cmd);	
	printf("==========================================================\n");
	printf("==========================================================\n");
	printf("###### Arguements\n");	
	printf("==========================================================\n");
	int i = 0;
	printf("DEBUG: Num args %d\n", currcmd->numArgs);
	for ( ; i < currcmd->numArgs ; i++)
		printf("ARGS: %s\n", currcmd->args[i]);
	printf("==========================================================\n");
	printf("==========================================================\n");
	printf("###### Flags\n");	
	printf("==========================================================\n");
	printf("Input redirection flag: %d\n", currcmd->flags.inredir);
	printf("Output redirection flag: %d\n", currcmd->flags.outredir);
	printf("Pipe flag: %d\n", currcmd->flags.pipe);
	printf("Background flag: %d\n", currcmd->flags.bkgnd);
	printf("==========================================================\n");
	printf("\n");
}

int main () {
	char *line;
	struct cmdCLI *currcmd;

	while((line = read_line())) {
		currcmd = parse(line);
		printCmd(currcmd);
	}
}

/* CLI input special characters */
#define CLISPC "><| "

/* Check whether c is in CLISPC */
int isspecial(char c) {

	char *termlist = CLISPC;

	for(;*termlist != '\0';termlist++)
		if(c == *termlist)
			return 1;
	
	return 0;
}

/* CLI cmd termination characters */
#define CLITERM "EOF\n;"

/* Check whether c is in CLITERM */
int isterm(char c) {

	char *termlist = CLITERM;

	for(;*termlist != '\0';termlist++)
		if(c == *termlist)
			return 1;
	
	return 0;
}

/* Char buffer to get and put back characters */

#define CHBUFSIZE 10

char chbuf[CHBUFSIZE];			/* Buffer of size CHBUFSIZE */
int chbufp = 0;				/* Char buffer pointer, always points to the next empty space */

int getch() {				/* Get char from buffer, if buffer empty then through stdin */
	return (chbufp > 0) ? chbuf[--chbufp] : getchar();
}

void ungetch(char c) {			/* Put char in buffer, print error if buffer full */
	if (chbufp < CHBUFSIZE)
		chbuf[chbufp++] = c;
	else
		printf("dsh:Buffer full");
}
