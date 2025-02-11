#include "parse_int.h"

void	print_ast(t_ast_node *ast, int depth)
{
	t_arg_data		*arg;
	t_redir_data	*redir;
	char			**operators;
	char			*symbol;

	if (LOGGING || DEBUG)
	{
		// You can implement an AST printer here to print the nodes for debugging.
		if (!ast)
			return ;
		// Indentation based on depth for readability
		for (int i = 0; i < depth; i++)
		{
			colored_printf(CYAN, "  ");
		}
		switch (ast->type)
		{
		case AST_NODE_CMD:
			colored_printf(CYAN, "Command: %s", ast->data.cmd.name);
			// Print arguments (if any)
			if (ast->data.cmd.argc > 0)
			{
				colored_printf(CYAN, "  Arguments: ");
				for (t_list *arg_node = ast->data.cmd.args; arg_node != NULL; arg_node = arg_node->next)
				{
					arg = (t_arg_data *)arg_node->content;
					colored_printf(CYAN, "%s", arg->raw);
					// Print additional information about the argument
					if (arg->option)
					{
						colored_printf(CYAN, " (option)");
					}
					if (arg->do_globbing)
					{
						colored_printf(CYAN, " (glob)");
					}
					if (arg->do_expansion)
					{
						colored_printf(CYAN, " (expand)");
					}
					colored_printf(CYAN, " ");
				}
			}
			// Print redirects (if any)
			if (ast->data.cmd.redc > 0)
			{
				colored_printf(CYAN, "  Redirects: ");
				for (t_list *redir_node = ast->data.cmd.redirs; redir_node != NULL; redir_node = redir_node->next)
				{
					redir = (t_redir_data *)redir_node->content;
					colored_printf(CYAN, "%d|%s %s ", redir->type,
						redir->symbol, redir->filename);
				}
			}
			colored_printf(CYAN, "\n");
			break ;
		case AST_NODE_PIPELINE:
			colored_printf(CYAN, "Pipeline with %d commands:\n",
				ast->data.pipe.cmdc);
			for (t_list *cmd_node = ast->data.pipe.cmds; cmd_node != NULL; cmd_node = cmd_node->next)
			{
				print_ast(cmd_node->content, depth + 1);
			}
			break ;
		case AST_NODE_LOG:
			colored_printf(CYAN, "Logical with %d commands, ",
				ast->data.log.cmdc);
			operators = (char **)ast->data.log.operators;
			if (operators)
			{
				colored_printf(CYAN, "  Ops: ");
				for (t_list *op_node = ast->data.log.operators; op_node != NULL; op_node = op_node->next)
				{
					symbol = (char *)op_node->content;
					colored_printf(CYAN, "%s", symbol);
				}
				printf("\n");
			}
			for (t_list *cmd_node = ast->data.log.cmds; cmd_node != NULL; cmd_node = cmd_node->next)
			{
				print_ast(cmd_node->content, depth + 1);
			}
			break ;
		case AST_NODE_PROC:
			colored_printf(CYAN, "Proc () with %d commands:\n",
				ast->data.proc.cmdc);
			for (t_list *cmd_node = ast->data.proc.cmds; cmd_node != NULL; cmd_node = cmd_node->next)
			{
				print_ast(cmd_node->content, depth + 1);
			}
			break ;
		// Add cases for other node types
		default:
			colored_printf(CYAN, "Unknown AST Node\n");
			break ;
		}
	}
}

static void	_destroy_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr[i])
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

// Deep copies linked list of arguments to char **array
char	**list_to_array(t_list *args, int argc)
{
	char	**array;
	t_list	*current;
	int		i;

	array = malloc(sizeof(char *) * (size_t)(argc + 1));
	if (!array)
		return (NULL);
	current = args;
	i = 0;
	while (i < argc && current)
	{
		array[i] = ft_strdup(((t_arg_data *)current->content)->raw);
		if (!array[i])
		{
			_destroy_arr(array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	array[argc] = NULL;
	return (array);
}

/* t_arg_data llist copy constructor using a llist of char*.
 * Returns new void *content for llist construction/duplication.
 */
void	*create_arg_data_node(void *content)
{
	t_arg_data	*arg_data;

	content = (char *)content;
	arg_data = malloc(sizeof(t_arg_data));
	if (arg_data)
	{
		arg_data->do_expansion = false;
		arg_data->do_globbing = false;
		arg_data->global_state = NULL;
		arg_data->in_dquotes = false;
		arg_data->option = false;
		arg_data->tmp = NULL;
		arg_data->raw = content;
	}
	return ((void *)arg_data);
}
