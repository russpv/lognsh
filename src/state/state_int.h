#ifndef STATE_INT_H
# define STATE_INT_H

# include "state.h"
# include "../builtins/env/env.h"

struct							s_global_state
{
	char						**envp;
	t_env						*sh_env_list;
	char						*pwd;
	int							current_exit_code;
	int							error_code;
	char						*input;
	char						*tmp;

	t_parser					*current_parser;
	t_lex						*current_lexer;
	t_cmd						*current_cmd;

	t_destroy_fn				destroy_parser;
	t_destroy_fn				destroy_lexer;
	t_destroy_fn				destroy_command;

	t_mem_node					mem_list;
};

typedef struct s_global_state	t_state;

// TODO
// t_ht *env_cache; TODO LATER, add hash table to env.h for faster lookups

#endif
