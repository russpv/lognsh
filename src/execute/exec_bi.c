#include "execute_int.h"

/* Executes redirects, if any, and calls built-in */
int	exec_bi_call(t_state *s, t_builtin_fn bi, t_cmd_fns *cf)
{
	int					exit_code;
	const t_cmd			*c = (const t_cmd *)get_cmd(s);
	const char			**argv = (const char **)cf->c_get_argv((t_cmd *)c);
	const int			argvc = (const int)cf->c_get_argvc((t_cmd *)c);
	const t_ast_node	*a = (const t_ast_node *)cf->c_get_node((t_cmd *)c);

	dprint("Exec: %s got cmd:%s node:%p", __FUNCTION__, argv[0], a);
	if (!argv || !bi || !c || !a)
		return (err("ERR null input.\n"), ERR_ARGS);
	cf->save_redirs((t_cmd *)c);
	if (0 != (get_parse_fns(s))->p_do_redirections((t_ast_node *)a))
	{
		cf->restore_redirs((t_cmd *)c);
		return (EX_EREDIR);
	}
	dprint("Exec: %s: Exec'g builtin\n", __FUNCTION__);
	exit_code = bi(s, (char **)argv, argvc);
	if (0 != exit_code)
		dprint("Exec: %s: ERR bi()\n", __FUNCTION__);
	cf->restore_redirs((t_cmd *)c);
	set_exit_status(s, exit_code);
	s_free_cmd(s);
	return (exit_code);
}
