#include "command_int.h"

#define EMSG_RUNC_STRJ "Path strjoin err\n"
#define EMSG_RUNC_EXEC "run_cmd ERR execve()\n"
#define LOGMSG_RUNC_GOT "Cmd: Found command! at %s\n"
#define LOGMSG_RUNC_SIGINT "Cmd: Received SIGINT before fork\n"

/* Note: command not found is thrown in caller */
static int	_check_access(const char *path)
{
	if (0 == access((const char *)path, F_OK))
	{
		if (0 != access((const char *)path, X_OK))
			return (print_perror(path), ERR_CMD_NOT_EXEC);
		return (0);
	}
	return (ERR_CMD_NOT_FOUND);
}

/* Returns 0 if absolute fullpath is found 
 * All heap allocs are temporary within this scope
 */
static int	_search_path(t_state *s, const char *cmd, char **fullpath)
{
	char	**paths;
	char	*tmp;
	int		i;

	i = -1;
	paths = get_sh_path(s);
	if (NULL == paths)
		return (1);
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (NULL == tmp)
			return (ft_freearr_mem(&get_mem(s)->list, get_mem(s)->dealloc, (void **)paths, -1), err(EMSG_RUNC_STRJ),
				ERR_MEM);
		*fullpath = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!*fullpath)
			return (ft_freearr_mem(&get_mem(s)->list, get_mem(s)->dealloc,  (void **)paths, -1), err(EMSG_RUNC_STRJ),
				ERR_MEM);
		if (0 == _check_access(*fullpath))
			return (ft_freearr_mem(&get_mem(s)->list, get_mem(s)->dealloc,  (void **)paths, -1), 0);
		free(*fullpath);
		*fullpath = NULL;
	}
	return (ft_freearr_mem(&get_mem(s)->list, get_mem(s)->dealloc,  (void **)paths, -1), ERR_CMD_NOT_FOUND);
}

/* Stores command path in fullpath, if valid.
 * Checks PATH, or absolute path if slash is in the name */
extern int	find_and_validate_cmd(t_state *s, const char *name, char **fullpath)
{
	if (NULL != name && '\0' != name[0])
	{
		if (ft_strchr(name, '/'))
		{
			if (0 == _check_access(name))
			{
				*fullpath = (char *)name; 
					return (0);
			}
		}
		else
		{
			if (0 == _search_path(s, name, fullpath))
				if (0 == _check_access(*fullpath))
					return (0);
		}
	}
	print_command_not_found(name);
	return (ERR_CMD_NOT_FOUND);
}

/* Forks depending on execution context
 * In the context of a proc or pipeline, forking would already
 * be done and be in a child process.
 * A null cmd name is valid, nothing runs.
 * Execute module handles redirects and forking in exec_fork_execve()
 */
int	run_cmd(t_state *s, t_ast_node *a)
{
	t_cmd	*c;
	int		exit_status;

	c = get_cmd(s);
	if (p_get_type(a) != AST_NODE_CMD || NULL == p_get_cmd(a))
		return (EINVAL);
	if (0 != find_and_validate_cmd(s, p_get_cmd(a), &c->fullpath))
		return (s_free_cmd(s), ERR_CMD_NOT_FOUND);
	if (c->fullpath)
		log_print(LOGMSG_RUNC_GOT, c->fullpath);
	if (CTXT_PIPELINE == st_int_peek(get_cmd(s)->st)
		|| CTXT_PROC == st_int_peek(get_cmd(s)->st))
	{
		if (-1 == execve(c->fullpath, c->argv, get_envp(s)))
			perror(EMSG_EXECVE);
	}
	else
	{
		exit_status = exec_fork_execve(s);
		if (SIGINT_BEFORE_FORK == exit_status)
			log_print(LOGMSG_RUNC_SIGINT);
	}
	s_free_cmd(s);
	return (0);
}
