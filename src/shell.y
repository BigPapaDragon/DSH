%{ 
#include <stdio.h>
#include <stdlib.h>

// Specifying that the lexical analyzer will be provided externally
extern int yylex();
void yyerror (char *msg);

%}

%union {
	char *string_val;
}

%token WORD PIPE GREAT LESS GREATGREAT GREATAMPERSAND GREATGREATAMPERSAND NEWLINE AMPERSAND

%%

goal: command_list;

cmd_and_args: 
	cmd arg_list
	;

cmd: WORD { //Add word here to command
  		printf("Recieved Command\n");
		}
	;

arg_list: 
	arg_list arg
	| /* Scenario: No arguements sepcified, can be empty */
	;

arg: 
	WORD { //Add word here to the arguementlist
		printf("Recieved Arguement\n");
		}
	;

pipe_list: 
	pipe_list PIPE cmd_and_args
	| cmd_and_args
	;

io_modifier_list: 
	io_modifier_list io_modifier
	| /* Scenario: No io_modifier provided, can be empty */
	;

io_modifier: 
	GREAT WORD { //Add word and modify io_flags
			printf("Recieved outfile io modification\n");
		}
	| LESS WORD { //Add word and modify io_flags 
			printf("Recieved infile io modification\n");
		}
	| GREATGREAT WORD { //Add word and modify io_flags 
			printf("Recieved append file io modification\n");
		}
	| GREATAMPERSAND WORD { //Add word and modify io_flags 
			printf("Recieved error outfile io modification\n");
		}
	| GREATGREATAMPERSAND WORD { //Add word and modify io_flags 
			printf("Recieved error append file io modification\n");
		}
	;

background: 
	AMPERSAND { //Modify background flag here 
			printf("Recieved background modification\n");
		}
	|
	;

command_line: 
	pipe_list io_modifier_list background NEWLINE { //Execute command, referesh data structure for next command
			printf("Executing with command\n");
		}
	| NEWLINE { //Execute command, referesh data structure for next command
			printf("Executing without command, only newline\n");
		}
	| error NEWLINE {
		// Handle Error
		yyerrok;
		}
	;

command_list: 
	command_list command_line
	| command_line
	;

%%

void yyerror (char *msg) {
	fprintf (stderr, "%s\n", msg);
	exit(1);
}

int main() {
	yyparse();
	return 0;
}
