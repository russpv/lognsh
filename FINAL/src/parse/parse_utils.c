#include "parse_int.h"

static void	_destroy_arr(t_mem_mgr *m, char **arr)
{
	int	i;

	i = 0;
	if (!arr[i])
		return ;
	while (arr[i])
		m->dealloc(&m->list, arr[i++]);
	m->dealloc(&m->list, arr);
}

// Deep copies linked list of arguments to char **array
char	**list_to_array(t_mem_mgr *m, t_list *args, int argc)
{
	char	**array;
	int		i;
	char	*new_s;

	array = m->f(&m->list, sizeof(char *) * (size_t)(argc + 1));
	if (NULL == array)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	i = 0;
	while (i < argc && args)
	{
		new_s = NULL;
		array[i] = new_s;
		if (NULL == array[i])
			array[i] = ft_strdup_tmp(m, ((t_arg_data *)args->content)->raw);
		if (NULL == array[i])
		{
			_destroy_arr(m, array);
			return (NULL);
		}
		args = args->next;
		i++;
	}
	array[argc] = NULL;
	return (array);
}

int	ft_lstiter_redir(t_list *lst, t_redir_ctx *ctx, \
int (*f)(void *, t_redir_ctx *))
{
	int	res;

	res = 0;
	if (lst == NULL)
		return (-1);
	while (lst)
	{
		res = f((lst)->content, ctx);
		if (0 != res)
			return (res);
		lst = (lst)->next;
	}
	return (0);
}

/* Inserts easily into test function */
t_ast_node	*test_parse(t_state *s, t_parser *parser)
{
	t_ast_node	*ast;

	while (!is_at_end(parser) && !parser->parse_error)
		ast = parse_full_cmd(s, parser);
	parser->ast = ast;
	return (ast);
}


/* -------------------------------------------------------------------------
 * Function Group: Printers
 * Description   : Print utils for various objects
 * ------------------------------------------------------------------------- */

/* Arglist print */

static void	_print_bool(bool val)
{
	if (val)
		printf("true\n");
	else
		printf("false\n");
}

static void	_print_rest(t_arg_data *data)
{
	printf("  lst: ");
	if (data->lst_tokens)
		printf("%p\n", data->lst_tokens);
	else
		printf("0x0\n");
	printf("  option: ");
	_print_bool(data->option);
	printf("  do_globbing: ");
	_print_bool(data->do_globbing);
	printf("  do_expansion: ");
	_print_bool(data->do_expansion);
	printf("  in_dquotes: ");
	_print_bool(data->in_dquotes);
}

static void	_print_arg_data(t_arg_data *data)
{
	int	i;

	if (DEBUG && isatty(STDOUT_FILENO))
	{
		if (!data)
			return ((void)printf("  t_arg_data is NULL\n"));
		printf("  raw: _");
		if (data->raw)
			printf("%s_\n", data->raw);
		else
			printf("(null)_\n");
		printf("  tmp: ");
		if (data->tmp)
		{
			i = -1;
			while (data->tmp[++i] != NULL)
				printf("%s ", data->tmp[i]);
			printf("\n");
		}
		else
			printf("(null)\n");
		printf("  grouparg: ");
		_print_bool(data->is_grouparg);
		_print_rest(data);
	}
}

void	dprint_arglist(t_list *head)
{
	t_list	*current;
	int		i;

	current = head;
	i = 0;
	if (DEBUG && isatty(STDOUT_FILENO))
	{
		while (current)
		{
			dprint("Node %d:\n", i);
			_print_arg_data((t_arg_data *)current->content);
			current = current->next;
			i++;
		}
		dvprint("List printed.\n");
	}
}

/* Node printers */

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
		cprintf(RESET, "\n");
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

/* Misc */

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
