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
