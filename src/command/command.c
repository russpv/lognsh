#include "command.h"
/*
t_cmd	init_command(char *input)
{
	//TODO
}

void	destroy_command(t_cmd c)
{
	//TODO
}

int	tokenize_command(t_cmd c)
{
	//TODO
}

int	parse_command(t_state s, t_cmd c)
{
	//TODO
}

void	exec_command(t_cmd c)
{
	//TODO, interface with EXECUTE?
}
*/
/* We've checked for shell funcs ... 
 * Next builtins 
 * Next PATH
 */
int	cmd_execute_simple(t_ast_node *a)
{
	t_builtin_fn bi;
	char **args;
	int exit_code;

	if (!a)
		return (-1);
	if (p_get_type(a) != AST_NODE_CMD)
		return (-1);
	bi = get_builtin(p_get_cmd(a));
	args = p_get_argv(a);
	exit_code = bi(args);
	return (exit_code);
}

