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

goal: command_line;

cmd_and_args: 
	cmd arg_list {
		if(!cmd) {
			//printf("Debug: cmd is NULL, initializing\n");
			cmd = cmdInit();
		} else {
			//printf("Debug: cmd is initialized\n");
		}
		insertSimpleCmd(cmd, simcmd);
	}
	;

cmd: WORD { //Add word here to command
  		//printf("Recieved Command: %s\n", yylval.string_val);
		simcmd = simpleCmdInit();
		insertArg(simcmd, yylval.string_val);
		}
	;

arg_list: 
	arg_list arg
	| /* Scenario: No arguements sepcified, can be empty */
	;

arg: 
	WORD { //Add word here to the arguementlist
		
		if(!simcmd)
			printf("Error: simcmd is uninitialized");

		//printf("Recieved Arguement: %s\n", yylval.string_val);
		insertArg(simcmd, yylval.string_val);
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
			//printf("Recieved outfile io modification: %s\n", yylval.string_val);
			setOutputRedir(cmd, yylval.string_val);
		}
	| LESS WORD { //Add word and modify io_flags 
			//printf("Recieved infile io modification: %s\n", yylval.string_val);
			setInputRedir(cmd, yylval.string_val);
		}
	| GREATGREAT WORD { //Add word and modify io_flags 
			//printf("Recieved append file io modification: %s\n", yylval.string_val);
		}
	| GREATAMPERSAND WORD { //Add word and modify io_flags 
			//printf("Recieved error outfile io modification: %s\n", yylval.string_val);
		}
	| GREATGREATAMPERSAND WORD { //Add word and modify io_flags 
			//printf("Recieved error append file io modification: %s\n", yylval.string_val);
		}
	;

background: 
	AMPERSAND { //Modify background flag here 
			//printf("Recieved background modification\n");
			setBackground(cmd);
		}
	|
	;

command_line: 
	pipe_list io_modifier_list background NEWLINE { //Execute command, referesh data structure for next command
			//printf("Executing with command\n");
			//if(!cmd)
			//	printf("cmd is NULL, thats an error\n");
			//else 
			//	printCommand(cmd);
			return 0;
		}
	| NEWLINE { //Execute command, referesh data structure for next command
			//printf("Executing without command, only newline\n");
			return 0;
		}
	| error NEWLINE {
		// Handle Error
		yyerrok;
		}
	;

%%

void yyerror (char *msg) {
	fprintf (stderr, "%s\n", msg);
	exit(1);
}
