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


/* Needs to work with the arg llist 
 * and mind the flags
 */
char **c_do_arg_expansions(t_ast_node *a)
{
	t_list *args = p_get_args(a);
	(void)args;
	//ft_lstiter(args, p_do_expansion);
	char **tmp = p_get_argv(a);
	return (tmp);

}

/* 
 * Next builtins 
 * Next PATH
 * if no cmd name...
 */
int	cmd_execute_simple(t_ast_node *a)
{
	char **args;
	int exit_code;

	exit_code = -1;
	if (!a)
		return (-1);
	if (p_get_type(a) != AST_NODE_CMD)
		return (-1);
	const t_builtin_fn bi = get_builtin(p_get_cmd(a));
	args = c_do_arg_expansions(a); 
	//_do_redirections(a);
	if (bi)
		exit_code = bi(args);
	//else exit_code = _run_cmd(args);
	return (exit_code);
}
