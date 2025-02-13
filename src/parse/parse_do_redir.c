#include "parse_int.h"

/* Executes redirection of t_redir_data llist
 * Accepts a t_redir_data.
 * Note: parser does not node-rize TOK_HEREDOC
 */
static void	_p_do_redirection(void *content)
{
	const t_redir_data	*node = (t_redir_data *)content;
	const t_redir_fn	handlers[] = {
	[TOK_REDIRECT_IN] = handle_redirect_in,
	[TOK_REDIRECT_OUT] = handle_redirect_out,
	[TOK_REDIRECT_APPEND] = handle_redirect_append,
	[TOK_HEREDOC_WORD] = handle_heredoc
	};

	if (NULL == content)
		return ;
	debug_print("Parser: _p_do_redirection...\n");
	if (!node->type)
		return (debug_print("Parser: _p_do_redirection got NULL\n"));
	debug_print("Parser: _p_do_redirection got smthg and executing...\n");
	if (handlers[node->type])
		handlers[node->type](node);
	else
		err("redirection handler issue\n");
}

/* Executes redirections of a cmd node in order.
 * If no redirections to do, returns ...
 * Expansions and globbing are to have been done earlier.
 */
int	p_do_redirections(t_ast_node *a)
{
	if (!a)
		return (err("ERR no node given\n"), ERR_ARGS);
	if (a->type != AST_NODE_CMD)
		return (EINVAL);
	if (!node_has_redirects(a))
		return (0);
	debug_print("Parser: p_do_redirections, doing redirs...\n");
	ft_lstiter(a->data.cmd.redirs, _p_do_redirection);
	return (0);
}
