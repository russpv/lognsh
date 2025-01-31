#ifndef STATE_INT_H
# define STATE_INT_H

#include "state.h"

struct s_global_state {
	char **envp;
	char *pwd; 
	int current_exit_code; 
	int error_code;
	char *input;
	
	t_parser *current_parser;
	t_lex *current_lexer;
	t_cmd *current_cmd;

	t_destroy_fn destroy_parser;
	t_destroy_fn destroy_lexer;
	t_destroy_fn destroy_command;
};

typedef struct s_global_state	t_state;

	// TODO track stdin, stdout, stderr fds to undo redirection
	// TODO add command history pointer
	// TODO 
	// no signals, too mcuh indirection	
	// t_ht *env_cache; TODO LATER, add hash table to env.h for faster lookups

#endif