#include "execute_int.h"

#define EMSG_NULLARGV "%s: ERR null argv parameters\n"
#define EMSG_NULLPATH "%s: ERR null fullpath parameters\n"
#define EMSG_NULLTCMD "%s: ERR null t_cmd parameters\n"
#define EMSG_NULL "%s: ERR null parameters\n"
#define DMSG_DOEXEC _MOD_ ": %s: Child exec'g %s\n"
#define DMSG_GOT _MOD_ ": %s: Got %s, of %p\n"
#define EMSG_CHILD "ERR child ops\n"
#define DMSG_REDIRSV _MOD_ ": %s: saving redir fns...\n"
#define DMSG_REDIRDO _MOD_ ": %s: doing redirs ...\n"
#define DMSG_DOBLTIN _MOD_ ": %s: Shell exec'g builtin\n"
#define DMSG_ERRBLTIN _MOD_ ": %s: ERR bi()\n"

/* Executes redirects, if any, and calls built-in */
int	exec_bi_call(t_state *s, t_builtin_fn bi)
{
	int					exit_code;
	const t_cmd			*c = (const t_cmd *)get_cmd(s);
	const char			**argv = (const char **)c_get_argv((t_cmd *)c);
	const int			argvc = (const int)c_get_argvc((t_cmd *)c);
	const t_ast_node	*a = (const t_ast_node *)c_get_node((t_cmd *)c);

	debug_print(DMSG_GOT, __FUNCTION__, argv[0], a);
	if (!argv || !bi || !c || !a)
		return (err(EMSG_NULL), ERR_ARGS);
	save_redirs((t_cmd *)c);
	if (0 != p_do_redirections((t_ast_node *)a))
    {
        restore_redirs((t_cmd *)c);
		return (ERR_REDIR);
    }
	debug_print(DMSG_DOBLTIN, __FUNCTION__);
	exit_code = bi(s, (char **)argv, argvc);
	if (0 != exit_code)
		debug_print(DMSG_ERRBLTIN, __FUNCTION__);
	restore_redirs((t_cmd *)c);
	set_exit_status(s, exit_code);
	s_free_cmd(s);
	return (exit_code);
}
