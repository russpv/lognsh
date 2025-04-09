#include "parse_int.h"

void	print_ast_cmd_helper(t_ast_node *ast)
{
	t_arg_data	*arg;
	t_list		*arg_node;

	cprintf(CYAN, "  Arguments: ");
	arg_node = ast->data.cmd.args;
	while (arg_node != NULL)
	{
		arg = (t_arg_data *)arg_node->content;
		cprintf(CYAN, "%s", arg->raw);
		if (arg->option)
			cprintf(CYAN, " (option)");
		if (arg->do_globbing)
			cprintf(CYAN, " (glob)");
		if (arg->do_expansion)
			cprintf(CYAN, " (expand)");
		cprintf(CYAN, " ");
		arg_node = arg_node->next;
	}
}

int	print_arg(void *arg)
{
	t_arg_data	*node;

	node = (t_arg_data *)arg;
	if (node)
	{
		if (node->raw)
			cprintf(MAGENTA, "%s", node->raw);
		else
			cprintf(MAGENTA, "(null)");
	}
	return (0);
}

int	print_redir_tok(void *redir)
{
	t_redir_data	*node;

	node = (t_redir_data *)redir;
	if (node)
	{
		if (node->filename)
			cprintf(MAGENTA, "%s", node->filename);
		else
			cprintf(MAGENTA, "(null)");
	}
	return (0);
}
