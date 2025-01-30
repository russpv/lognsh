#include "state.h"


struct s_global_state {
	char **path;
	char *pwd; 
	int current_exit_code; 
	int error_code;
	char *input;
	t_parser *current_parser;
	t_lex *current_lexer;
	t_destroy_fn destroy_parser;
	t_destroy_fn destroy_lexer;

};

typedef struct s_global_state	t_state;

	// TODO track stdin, stdout, stderr fds to undo redirection
	// TODO add command history pointer
	// TODO 
	// no signals, too mcuh indirection	
	// t_ht *env_cache; TODO LATER, add hash table to env.h for faster lookups

t_state	*init_state(void)
{
 	t_state *s = malloc(sizeof(struct s_global_state));
	if (s)
	{
		s->path = NULL; // TODO remove?
		s->pwd = NULL; // TODO remove?
		s->current_exit_code = 0;
		s->error_code = 0;
		s->current_parser = NULL;
		s->current_lexer = NULL;
		s->input = NULL;
	}
	return (s);
}

void	destroy_state(t_state *s)
{
	if (s->current_parser)
		s->destroy_parser(s->current_parser);
	if (s->current_lexer)
		s->destroy_lexer(s->current_lexer);
	if (s->input)
		free(s->input);
}



