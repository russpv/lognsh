#include "command_int.h"

static int	_handle_no_command(t_ast_node *a, char **args)
{
	if (true == p_get_expansion(a) && !p_get_cmd(a))
		p_set_cmd(a, args[0]);
	if (false == p_get_expansion(a) && !p_get_cmd(a))
		return (0);
	return (-1);
}

static void	_print_command_info(t_cmd *c, t_ast_node *a)
{
	colored_printf(YELLOW, "\tExecuting command: %s\n", p_get_cmd(a));
	colored_printf(YELLOW, "\tArguments:\n");
	if (c->argv)
	{
		for (int i = 0; c->argv[i] != NULL; i++)
			colored_printf(YELLOW, "\t  argv[%d]: %s\n", i, c->argv[i]);
	}
	colored_printf(YELLOW, "\t  argv[%d]: (NULL)\n", p_get_argc(a) + 1);
}

/* For an AST COMMAND node only
 * Checks for no command, builtin, then PATH
 * Prints diagnostic info.
 * Stores argv and argc into s->curr_cmd.
 */
int	cmd_exec_simple(t_state *s, t_ast_node *a)
{
	char				**args;
	int					exit_code;
	const t_cmd			*c = (const t_cmd *)get_cmd(s);
	const t_builtin_fn	bi = get_builtin(p_get_cmd(a));

	debug_print("\t### cmd_exec_simple ###\n");
	if (!c || !a)
		return (-1);
	if (p_get_type(a) != AST_NODE_CMD)
		return (-1);
	((t_cmd *)c)->curr_node = a;
	exit_code = -1;
	args = p_do_arg_expansions(a);
	exit_code = _handle_no_command(a, args);
	if (0 == exit_code)
		return (exit_code);
	((t_cmd *)c)->argv = c_argstoargv(args, p_get_cmd(a), p_get_argc(a));
	((t_cmd *)c)->argc = p_get_argc(a) + 1;
	if (bi)
		exit_code = exec_bi_call(s, bi);
	else
	{
		_print_command_info((t_cmd *)c, a);
		exit_code = run_cmd(s, a);
	}
	return (exit_code);
}
