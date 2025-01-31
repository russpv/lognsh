# include "parse_int.h"

bool	node_has_redirects(t_ast_node *n)
{
    return (n->type == AST_NODE_CMD && n->data.cmd.redc > 0);
}

void handle_redirect_in(const t_redir_data *node)
{
	if (!node->filename)
		err("No filename for input redirection.\n");
	//handle expansions
	const int append = false;
	if (-1 == redirect(NULL, node->filename, STDIN_FILENO, append))
		err("Input redirection issue\n");
}

void handle_heredoc(const t_redir_data *node)
{
	if (!node->doc)
		err("No heredoc for input redirection.\n");
	const int append = false;
	int fildes[2];

	if (pipe(fildes) < 0)
		err("heredoc pipe()");
	write(fildes[1], node->doc, ft_strlen(node->doc));
	close(fildes[1]);
	if (-1 == redirect(&fildes[0], NULL, STDIN_FILENO, append))
		err("Heredoc redirection issue\n");
}

void handle_redirect_out(const t_redir_data *node)
{
	if (!node->filename)
		err("No filename for output redirection.\n");
	//handle expansions
	const int append = false;
	if (-1 == redirect(NULL, node->filename, STDOUT_FILENO, append))
		err("Output redirection issue\n");
}

void handle_redirect_append(const t_redir_data *node)
{
	if (!node->filename)
		err("No filename for output redirection.\n");
	//handle expansions
	const int append = true;
	if (-1 == redirect(NULL, node->filename, STDOUT_FILENO, append))
		err("Append redirection issue\n");
}

/* Executes redirection of t_redir_data llist */
void	p_do_redirection(void *content)
{
	const t_redir_data *node = (t_redir_data *)content;
    redir_fn handlers[] = {
        [TOK_REDIRECT_IN] = handle_redirect_in,
        [TOK_REDIRECT_OUT] = handle_redirect_out,
        [TOK_REDIRECT_APPEND] = handle_redirect_append,
        [TOK_HEREDOC] = handle_heredoc
    };

    if (handlers[node->type]) 
        handlers[node->type](node);
	else 
		err("redirection handler issue\n");
}

/* Processes redirections of a cmd node in order.
 * If no redirections to do, returns ...
 */
int	p_do_redirections(t_ast_node *a)
{
	if (!a)
		return (err("ERR no node given\n"), -1);
	if (a->type != AST_NODE_CMD)
		return (EINVAL);
	if (!node_has_redirects(a))
		return (0);
	ft_lstiter(a->data.cmd.redirs, p_do_redirection);
	return (0);
}
