#include "command_int.h"

#define NO_CMD 0

/* In case of no command name string, determines if 
 * the cmd is not an expansion, or there is an expansion to do
 */
static int	_handle_no_command(t_ast_node *a, char **args)
{
	if (true == p_get_expansion(a) && !p_get_cmd(a))
		p_set_cmd(a, args[0]);
	if (false == p_get_expansion(a) && !p_get_cmd(a))
		return (NO_CMD);
	return (-1);
}

static int	_do_ops(t_state *s, t_ast_node *a, const t_cmd *c)
{
	char	**args;
	int		exit_code;
	int		res;

	((t_cmd *)c)->curr_node = a;
	args = NULL;
	res = p_do_arg_processing(s, a, &args);
	if (0 != res)
		return (res);
	exit_code = _handle_no_command(a, args);
	if (NO_CMD == exit_code)
		return (exit_code);
	((t_cmd *)c)->argv = c_argstoargv(args, p_get_cmd(a), p_get_argc(a));
	((t_cmd *)c)->argc = p_get_argc(a) + 1;
	if (0 != p_do_redir_processing(s, a))
		return (ERR_AMBIGUOUS_REDIR);
	return (exit_code);
}

/* CMD EXEC SIMPLE
 *
 * For an AST COMMAND node only
 * Checks for no command, builtin, then PATH
 * Prints diagnostic info.
 * Runs expansion then globbing and stores
 * 	results into t_cmd not t_ast_node.
 * Throws ambiguous redirect error (1) prior to exec'g.
 * Stores argv and argc into s->curr_cmd.
 */
int	cmd_exec_simple(t_state *s, t_ast_node *a)
{
	int					exit_code;
	const t_cmd			*c = (const t_cmd *)get_cmd(s);
	const t_builtin_fn	bi = get_builtin(p_get_cmd(a));

	log_print("Cmd: \t### cmd_exec_simple ###\n");
	if (!c || !a)
		return (ERR_ARGS);
	if (p_get_type(a) != AST_NODE_CMD)
		return (-1);
	exit_code = _do_ops(s, a, c);
	if (ERR_AMBIGUOUS_REDIR == exit_code || NO_CMD == exit_code)
		return (exit_code);
	if (bi)
		exit_code = exec_bi_call(s, bi);
	else
	{
		log_command_info((t_cmd *)c, a);
		exit_code = run_cmd(s, a);
	}
	log_print("Cmd: \tFinished cmd_exec_simple.\n");
	return (exit_code);
}
