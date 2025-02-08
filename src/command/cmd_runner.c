#include "command_int.h"

static int	_check_access(const char *path)
{
	if (0 == access((const char *)path, F_OK))
	{
		if (0 != access((const char *)path, X_OK))
			return (print_permission_denied(path), ERR_CMD_NOT_EXEC);
		return (0);
	}
	return (-1);
}

/* Returns 0 if absolute fullpath is found */
static int	_search_path(const char *cmd, char **fullpath)
{
	char	**paths;
	char	*tmp;
	int		i;

	i = -1;
	if (!(paths = s_getenv()))
		return (1);
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (NULL == tmp)
			return (ft_freearr((void **)paths, -1), err("Path strjoin err\n"), \
			1);
		*fullpath = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!*fullpath)
			return (ft_freearr((void **)paths, -1), err("Path strjoin err\n"), \
				1);
		if (0 == _check_access(*fullpath))
			return (ft_freearr((void **)paths, -1), 0);
		free(*fullpath);
		*fullpath = NULL;
	}
	return (ft_freearr((void **)paths, -1), 1);
}

/* Stores command path in fullpath, if valid.
 * Checks PATH, or absolute path if slash is in the name */
extern int	find_and_validate_cmd(const char *name, char **fullpath, \
		const char *caller)
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
			if (0 == _search_path(name, fullpath))
				if (0 == _check_access(*fullpath))
					return (0);
		}
	}
	print_command_not_found(name, ft_strdup(caller));
	return (ERR_CMD_NOT_FOUND);
}

/* Forks depending on execution context
 * In the context of a proc or pipeline,
 * forking would already be done and be in
 * a child process.
 * a null cmd name is valid, nothing runs
 * Execute module handles redirects and forking in exec_fork_execve()
 * 
 */
int	run_cmd(t_state *s, t_ast_node *a)
{
	t_cmd	*c;
	int	exit_status;

	c = get_cmd(s);
	if (p_get_type(a) != AST_NODE_CMD || NULL == p_get_cmd(a))
		return (EINVAL);
	if (0 != find_and_validate_cmd(p_get_cmd(a), &c->fullpath, NULL))
		return (s_free_cmd(s), ERR_CMD_NOT_FOUND);
	if (c->fullpath)
		debug_print("Cmd: Found command! at %s\n", c->fullpath);
	if (CTXT_PIPELINE == st_peek(get_cmd(s)->st)
		|| CTXT_PROC == st_peek(get_cmd(s)->st))
	{
		if (-1 == execve(c->fullpath, c->argv, get_envp(s)))
			err("run_cmd ERR execve()\n");
	}
	else 
	{
		exit_status = exec_fork_execve(s);
		if (SIGINT_BEFORE_FORK == exit_status)
			debug_print("Cmd: Received SIGINT before fork\n");
	}
	s_free_cmd(s);
	return (0);
}
