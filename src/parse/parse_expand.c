#include "parse_int.h"
/*
void	do_expansion(void *node)
{
	t_arg_data *arg_node = (t_arg_data *)node;
	if (!arg_node->do_expansion)
		return ;
	// lookup env for value
	char *value = getenv(arg_node->raw);
	// split the value if not in dquotes
	if (!arg_node->in_dquotes)
		char **arr = _split_words(value);
	// remove outer quotes, already done by lexer
	// insert into list

} */