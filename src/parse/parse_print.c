#include "parse_int.h"

static void	_print_log(t_ast_node *ast, int depth)
{
	char	**operators;
	char	*symbol;
	t_list	*op_node;
	t_list	*cmd_node;

	cprintf(CYAN, "Logical with %d commands, ", ast->data.log.cmdc);
	operators = (char **)ast->data.log.operators;
	if (operators)
	{
		cprintf(CYAN, "  Ops: ");
		op_node = ast->data.log.operators;
		while (op_node != NULL)
		{
			symbol = (char *)op_node->content;
			cprintf(CYAN, "%s", symbol);
			op_node = op_node->next;
		}
		printf("\n");
	}
	cmd_node = ast->data.log.cmds;
	while (cmd_node != NULL)
	{
		print_ast(cmd_node->content, depth + 1);
		cmd_node = cmd_node->next;
	}
}

static void	_print_cmd(t_ast_node *ast)
{
	t_redir_data	*redir;
	t_list			*redir_node;

	cprintf(CYAN, "Command: %s", ast->data.cmd.name);
	if (ast->data.cmd.argc > 0)
		print_ast_cmd_helper(ast);
	if (ast->data.cmd.redc > 0)
	{
		cprintf(CYAN, "  Redirects: ");
		redir_node = ast->data.cmd.redirs;
		while (redir_node != NULL)
		{
			redir = (t_redir_data *)redir_node->content;
			cprintf(CYAN, "%d|%s %s g_%d exp_%d", redir->type, redir->symbol,
				redir->filename, redir->do_globbing, redir->do_expansion);
			redir_node = redir_node->next;
		}
	}
	cprintf(CYAN, "\n");
}

static void	_print_proc(t_ast_node *ast, int depth)
{
	t_list	*cmd_node;

	cprintf(CYAN, "Proc () with %d commands %d redirs:\n", ast->data.proc.cmdc,
		ast->data.proc.redc);
	cmd_node = ast->data.proc.cmds;
	while (cmd_node != NULL)
	{
		print_ast(cmd_node->content, depth + 1);
		cmd_node = cmd_node->next;
	}
}

void	_print_pipe(t_ast_node *ast, int depth)
{
	t_list	*cmd_node;

	cprintf(CYAN, "Pipeline with %d commands:\n", ast->data.pipe.cmdc);
	cmd_node = ast->data.pipe.cmds;
	while (cmd_node != NULL)
	{
		print_ast(cmd_node->content, depth + 1);
		cmd_node = cmd_node->next;
	}
}

void	print_ast(t_ast_node *ast, int depth)
{
	int	i;

	if ((LOGGING || DEBUG) && ast)
	{
		i = -1;
		while (++i < depth)
			cprintf(CYAN, "  ");
		if (ast->type == AST_NODE_CMD)
			_print_cmd(ast);
		else if (ast->type == AST_NODE_PIPELINE)
			_print_pipe(ast, depth);
		else if (ast->type == AST_NODE_LOG)
			_print_log(ast, depth);
		else if (ast->type == AST_NODE_PROC)
			_print_proc(ast, depth);
		else
			cprintf(CYAN, "Unknown AST Node\n");
	}
}
