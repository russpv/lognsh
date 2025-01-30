#include "parse_int.h"

/* Returns the cmd name of an AST_NODE_CMD node */
char *p_get_cmd(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (NULL);
	debug_print("p_get_cmd got: _%s_\n", a->data.cmd.name);
	return (a->data.cmd.name);
}

int p_get_type(t_ast_node *a)
{
	return (a->type);
}

/* Returns the llist of t_arg_data node content */
t_list *p_get_args(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (NULL);
	debug_print("p_get_args got: _%s_\n", a->data.cmd.name);
	return (a->data.cmd.args);
}

void	destroy_arr(char **arr)
{
	int i = 0;
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

	array = malloc(sizeof(char *) * (argc + 1));
	if (!array)
		return (NULL);
	current = args;
	i = 0;
	while (i < argc && current)
	{
		array[i] = ft_strdup(((t_arg_data *)current->content)->raw);
		if (!array[i])
		{
			destroy_arr(array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	array[argc] = NULL;
	return (array);
}
// TODO change to actual argv
char **p_get_argv(t_ast_node *a)
{
	char **args;

	args = NULL;
	if (!a)
		return (NULL);
	if (p_get_type(a) != AST_NODE_CMD)
		return (NULL);
	if (a->data.cmd.argc > 0)
	{
		args = list_to_array(a->data.cmd.args, a->data.cmd.argc);
	}
	return (args);
}

/* Needs to work with the arg llist 
 * and mind the flags
 */
char **p_do_arg_expansions(t_ast_node *a)
{
	if (a->type != AST_NODE_CMD)
		return (NULL);
	t_list *args = p_get_args(a);
	ft_lstiter(args, p_do_expansion);
	//char **tmp = p_get_argv(a);
	char **tmp = list_to_array(args, a->data.cmd.argc);
	return (tmp);
}
