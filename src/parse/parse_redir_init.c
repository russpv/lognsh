#include "parse_int.h"

#define ERRMSG_REDIR_DATA_MALLOC "Memory allocation failed for redirection data\n"

t_redir_data	*init_redir(t_ast_node *target, enum e_tok_type type)
{
	t_redir_data	*red;

	if (!target)
		return (NULL);
	red = malloc(sizeof(t_redir_data));
	if (!red)
	{
		err(ERRMSG_REDIR_DATA_MALLOC);
		return (NULL);
	}
	red->type = type;
	red->target_ptr = target;
	red->do_globbing = false;
	red->do_expansion = false;
	red->heredoc_body = NULL;
	red->symbol = NULL;
	red->filename = NULL;
	red->global_state = NULL;
	return (red);
}
