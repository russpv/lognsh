#include "command_int.h"

#define LMSG_IN _MOD_ ": ######## %s ########\n"
#define DMSG_CEXEC_NOTPROC _MOD_ ": node not a proc...\n"
#define DMSG_CEXEC_NOTCMD _MOD_ ": node not a cmd...\n"
#define DMSG_CEXEC_NOTPIP _MOD_ ": node not a pipe...\n"
#define EMSG_CEXEC_UNK _MOD_ ": ERR unknown node...\n"

void	destroy_cmd(t_state *s, void *c)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)c;
	if (cmd->st)
		st_int_destroy(cmd->st);
//	if (cmd->fullpath)
//		free(cmd->fullpath);
	if (cmd->redirs)
		ft_lstclear(&cmd->redirs, destroy_redir);
	if (cmd->fildes)
		ft_freearr((void **)cmd->fildes, -1);
	if (cmd->argv)
		ft_freearr((void **)cmd->argv, -1);
	free(cmd);
}

t_cmd	*init_cmd(t_state *s, t_ast_node *a)
{
	t_cmd	*c;

	c = malloc(sizeof(struct s_cmd));
	if (c)
	{
		c->fildes = NULL;
		c->st = st_int_create();
		c->argv = NULL;
		c->argc = 0;
		c->redirs = NULL;
		c->redc = 0;
		c->fullpath = NULL;
		c->curr_node = a;
		c->saved_stderr = -1;
		c->saved_stdin = -1;
		c->saved_stdout = -1;
		c->curr_cmdc = -1;
		c->argvc = 0;
		s_free_tmp(s);
		set_command(s, c);
		register_command_destroy(s, destroy_cmd);
	}
	return (c);
}

/* Determines which parser to call given current token
 * note: cmd_exec_simple is atomic and does not recurse
 */
int	cmd_execute_full(t_state *s, t_ast_node *a)
{
	log_print(LMSG_IN, __FUNCTION__);
	if (AST_NODE_PROC == p_get_type(a))
		return (cmd_exec_proc(s, a));
	debug_print(DMSG_CEXEC_NOTPROC);
	if (AST_NODE_CMD == p_get_type(a))
		return (cmd_exec_simple(s, a));
	debug_print(DMSG_CEXEC_NOTCMD);
	if (AST_NODE_PIPELINE == p_get_type(a))
		return (cmd_exec_pipe(s, a));
	debug_print(DMSG_CEXEC_NOTPIP);
	if (AST_NODE_LOG == p_get_type(a))
		return (cmd_exec_log(s, a));
	err(EMSG_CEXEC_UNK);
	return (ERR_SYNTAX);
}

/* CMD EXECUTE
 * Recursively executes the AST from Parser
 * Sets current_cmd in State then calls
 * top 'switchboard' func
 * Returns last exit_code from input
 *
 * Setting cmd in State allows multiple accesses
 * for expansions, globbing, etc.
 *
 * The Command ADT stack tracks current AST depth
 * When a sub-method is called, the current node is
 * pushed. And when that sub-method returns, it is
 * popped. Why?
 */
int	cmd_execute(t_state *s, t_ast_node *a)
{
	if (NULL == init_cmd(s, a))
		return (ERR_MEM);
	print_ast(a, 10);
	return (cmd_execute_full(s, a));
}
