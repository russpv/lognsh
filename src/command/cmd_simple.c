#include "command_int.h"

static int	_handle_no_command(t_ast_node *a, char **args)
{
	if (true == p_get_expansion(a) && !p_get_cmd(a))
		p_set_cmd(a, args[0]);
	if (false == p_get_expansion(a) && !p_get_cmd(a))
		return (0);
	return (-1);
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
	char				**args;
	int					exit_code;
	const t_cmd			*c = (const t_cmd *)get_cmd(s);
	const t_builtin_fn	bi = get_builtin(p_get_cmd(a));

	debug_print("Cmd: \t### cmd_exec_simple ###\n");
	if (!c || !a)
		return (-1);
	if (p_get_type(a) != AST_NODE_CMD)
		return (-1);
	((t_cmd *)c)->curr_node = a;
	exit_code = -1;
	//TODO expansion on redirs, then
	//TODO globbing on args and redirs (throw err ambiguous redir)
	args = p_do_arg_processing(a); // this needs to be freed
	exit_code = _handle_no_command(a, args);
	if (0 == exit_code)
		return (exit_code);
	((t_cmd *)c)->argv = c_argstoargv(args, p_get_cmd(a), p_get_argc(a));
	((t_cmd *)c)->argc = p_get_argc(a) + 1;
	if (0 != p_do_redir_processing(a))
		return (ERR_AMBIGUOUS_REDIR);
	if (bi)
		exit_code = exec_bi_call(s, bi);
	else
	{
		log_command_info((t_cmd *)c, a);
		exit_code = run_cmd(s, a);
	}
	debug_print("Cmd: Finished cmd_exec_simple.\n");
	return (exit_code);
}
