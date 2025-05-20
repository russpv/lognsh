#include "command_int.h"

static void	_fill_argv_nocmd(t_mem_mgr *m, int i, struct s_context *ctxt)
{
	if (i == 0)
	{
		ctxt->argv[0] = ft_strdup_tmp(m, ctxt->args[0]);
		if (!ctxt->argv[0])
			exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	}
	else if (ctxt->args && i < ctxt->argc)
	{
		ctxt->argv[i] = ft_strdup_tmp(m, ctxt->args[i]);
		if (!ctxt->argv[i])
			exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	}
	else if (ctxt->args && i >= ctxt->argc)
	{
		ctxt->argv[i] = NULL;
		ctxt->count--;
	}
	ctxt->count++;
}

static void	_fill_argv_cmdname(t_mem_mgr *m, int i, struct s_context *ctxt)
{
	if (i == 0)
	{
		ctxt->argv[0] = ft_strdup_tmp(m, ctxt->cmdname);
		if (!ctxt->argv[0])
			exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	}
	else if (ctxt->args)
	{
		ctxt->argv[i] = ft_strdup_tmp(m, ctxt->args[i - 1]);
		if (!ctxt->argv[i])
			exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	}
	ctxt->count++;
}

static int	_do_loop(t_state *s, struct s_context *ctxt)
{
	int	i;

	i = -1;
	ctxt->count = 0;
	while (++i <= ctxt->argc)
	{
		if (ctxt->cmdname)
			_fill_argv_cmdname(get_mem(s), i, ctxt);
		else if (ctxt->args && ctxt->argc > 0)
			_fill_argv_nocmd(get_mem(s), i, ctxt);
		else
			return (-1);
	}
	return (i);
}

int	arrlen(char *arr[])
{
	int	len;

	len = 0;
	while (arr[len] != NULL)
		len++;
	return (len);
}

/* Builds argv from args and stores in command
 * If no cmd, copies first arg as cmd
 * If no args, ...
 * Note: len(ctxt.argv) == ctxt.count
 */
int	c_argstoargv(t_state *s, t_cmd *cmd, t_ast_node *a, char **args)
{
	struct s_context	ctxt;
	int					i;
	t_mem_mgr			*m;

	if (!s || !a || !cmd)
		return (err("c_argstoargv NULL args\n"), ERR_ARGS);
	m = get_mem(s);
	ctxt.args = args;
	ctxt.argc = p_get_argc(a);
	ctxt.cmdname = p_get_cmd(a);
	ctxt.argv = m->f(&m->list, (sizeof(char *) * ((size_t)ctxt.argc + 2)));
	if (!ctxt.argv)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	dprint(_MOD_ ": %s: got cmd[%s] argc:%d args:%p \n", __FUNCTION__,
		ctxt.cmdname, ctxt.argc, args);
	i = _do_loop(s, &ctxt);
	if (i < 0)
		return (ERR_GENERAL);
	ctxt.argv[i] = NULL;
	cmd->argvc = ctxt.count;
	dprint(_MOD_ ": %s: adjusted argc:%d\n", __FUNCTION__, cmd->argvc);
	cmd->argv = ctxt.argv;
	if (NULL == ctxt.cmdname)
		p_set_cmd(get_mem(s), a, ctxt.argv[0]);
	return (ft_freearr_mem(&m->list, m->dealloc, (void **)args, -1), 0);
}

void	print_pipes(t_cmd *c)
{
	int	i;

	if (c == NULL || c->fildes == NULL)
		return (dprint("Error: fildes is NULL\n"));
	i = -1;
	while (++i < c->curr_cmdc - 1)
		dprint("Cmd: \tPipe %d: read fd=%d, write fd=%d\n", i, c->fildes[i][0],
			c->fildes[i][1]);
}
