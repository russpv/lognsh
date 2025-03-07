#include "parse_int.h"

# define EMSG_REDIR_HANDLER "Redirection handler issue\n"
# define EMSG_REDIR_NULLNODE "ERR no node given\n"
# define DBGMSG_REDIR_ANNOUNCE2 _MOD_ ": _p_do_redirection iterating...\n"
# define DBGMSG_REDIR_GOTNULL _MOD_ ": _p_do_redirection got NULL\n"
# define DBGMSG_REDIR_GOT _MOD_ ": _p_do_redirection got redir, executing...\n"
# define DBGMSG_REDIR_ANNOUNCE _MOD_ ": p_do_redirections, doing redirs...\n"

/* Executes redirection of t_redir_data llist
 * Accepts a t_redir_data.
 * Assumes a non-null set of redirections exists.
 * Note: parser does not node-rize TOK_HEREDOC
 * Note: no error code returns
 */
static int	_p_do_redirection(void *content)
{
	const t_redir_data	*redir = (t_redir_data *)content;
	const t_redir_fn	handlers[] = {
	[TOK_REDIRECT_IN] = handle_redirect_in,
	[TOK_REDIRECT_OUT] = handle_redirect_out,
	[TOK_REDIRECT_APPEND] = handle_redirect_append,
	[TOK_HEREDOC_WORD] = handle_heredoc
	};

	if (NULL == content)
		return (ERR_ARGS);
	debug_print(DBGMSG_REDIR_ANNOUNCE2);
	if (!redir->type)
		return (debug_print(DBGMSG_REDIR_GOTNULL), ERR_ARGS);
	debug_print(DBGMSG_REDIR_GOT);
	//TODO do expansion
	if (handlers[redir->type])
	{
		if (0 != handlers[redir->type](redir))
			return (err(EMSG_REDIR_HANDLER), ERR_GENERAL);
	}
	else
		return (err(EMSG_REDIR_HANDLER), ERR_GENERAL);
	return (0);
}

/* Executes redirections of a cmd node in order.
 * If no redirections to do, returns ...
 * Expansions and globbing are to have been done earlier.
 * If error during redirection due to...
 * 	- multiple filenames, aborts command only
 *  - nonexistent input file, aborts command only
 *  - nonexistent output file, creates file and proceeds (no error)
 */
int	p_do_redirections(t_ast_node *a)
{
	if (!a)
		return (err(EMSG_REDIR_NULLNODE), ERR_ARGS);
	if (a->type != AST_NODE_CMD)
		return (EINVAL);
	if (false == node_has_redirects(a))
		return (0);
	debug_print(DBGMSG_REDIR_ANNOUNCE);
	if (0 != ft_lstiter(a->data.cmd.redirs, _p_do_redirection))
		return (ERR_REDIR);
	return (0);
}
