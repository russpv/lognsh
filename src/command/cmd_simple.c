#include "command_int.h"

#define NO_CMD -10
#define LMSG_IN _MOD_ ": \t### %s ###\n"
#define LMSG_OUT _MOD_ ": \tFinished %s.\n"

/* Adds arg[0] as command name if no name string present
 * if node expansion flag is set. Removes arg[0] then.
 * TODO: what circumstances result in no command?
 */
static int	_handle_no_command(t_ast_node *a, char **args)
{
	if (NULL == p_get_cmd(a))
	{
		if (true == p_get_expansion(a) || true == p_get_grouptok(a))
		{
			p_set_cmd(a, args[0]);
			p_update_argc(a, -1);
		}
		else
			return (NO_CMD);
	}
	return (0);
}

/* Processes args and redirs. Stores several command parameters. */
static int	_do_ops(t_state *s, t_ast_node *a, t_cmd *c)
{
	char	**args;
	int		exit_code;
	int		res;

	c->curr_node = a;
	args = NULL;
	res = p_do_arg_processing(s, a, &args);
	if (0 != res)
		return (res);
	exit_code = _handle_no_command(a, args);
	if (NO_CMD == exit_code)
		return (exit_code);
	c->argv = c_argstoargv(args, p_get_cmd(a), p_get_argc(a));
	c->argc = p_get_argc(a) + 1;
	c->redc = p_get_redc(a); 
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
 * Silently returns if non-expansion string 
 */
int	cmd_exec_simple(t_state *s, t_ast_node *a)
{
	int					exit_code;
	t_cmd				*c;
	t_builtin_fn		bi;

	c = get_cmd(s);
	log_print(LMSG_IN, __FUNCTION__);
	if (!c || !a)
		return (ERR_ARGS);
	if (p_get_type(a) != AST_NODE_CMD)
		return (ERR_INVALID_CMD_TYPE);
	exit_code = _do_ops(s, a, c);
	if (ERR_AMBIGUOUS_REDIR == exit_code || NO_CMD == exit_code)
		return (exit_code);
	log_command_info((t_cmd *)c, a);
	bi = get_builtin(p_get_cmd(a));
	if (bi)
		exit_code = exec_bi_call(s, bi);
	else
		exit_code = run_cmd(s, a);
	log_print(LMSG_OUT, __FUNCTION__);
	return (exit_code);
}
