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
		if(!cmd)
			cmd = cmdInit();
		
		insertSimpleCmd(cmd, simcmd);
	}
	;

cmd: WORD { //Add word here to command
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
			setOutputRedir(cmd, yylval.string_val);
		}
	| LESS WORD { //Add word and modify io_flags 
			setInputRedir(cmd, yylval.string_val);
		}
	| GREATGREAT WORD { //Add word and modify io_flags 
		}
	| GREATAMPERSAND WORD { //Add word and modify io_flags 
		}
	| GREATGREATAMPERSAND WORD { //Add word and modify io_flags 
		}
	;

background: 
	AMPERSAND { //Modify background flag here 
			setBackground(cmd);
		}
	|
	;

command_line: 
	pipe_list io_modifier_list background NEWLINE { //Execute command, referesh data structure for next command
			return cmd->_numSimpleCmds;
		}
	| NEWLINE { //Execute command, referesh data structure for next command
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
