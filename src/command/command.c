#include "command.h"

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
	args = p_do_arg_expansions(a); 
	//_do_redirections(a);
	if (bi)
		exit_code = bi(args);
	//else exit_code = _run_cmd(args);
	return (exit_code);
}
