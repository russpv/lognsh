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

/* Returns 0 if absolute path is found */
int	_search_path(const char *cmd, char **fullpath)
{
    char **paths;
	char *tmp;
	int i;

	i = -1;
    if (!(paths = s_getenv()))
		return (1);
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		*fullpath = ft_strjoin(tmp, cmd);
		free(tmp);
        if (!*fullpath)
            return (ft_freearr((void **)paths, -1), err("Path strjoin err\n"), 1);
        if (0 == _check_access(*fullpath))
            return (ft_freearr((void **)paths, -1), 0);
        free(*fullpath);
		*fullpath = NULL;
	}
	return (ft_freearr((void **)paths, -1), 1);
}

/* Checks PATH, or absolute path if slash is in the name */
static int	_find_and_validate_cmd(const char *cmd, char **fullpath)
{
	if (!(NULL == cmd || '\0' == cmd[0]))
	{
		if (ft_strchr(cmd, '/'))
		{
			if (0 == _check_access(cmd))
				return (0); 
		}
		else
		{
			if (0 == _search_path(cmd, fullpath))
				if (0 == _check_access(*fullpath))
					return (0);
		}
	}
	print_command_not_found(cmd);
	return (ERR_CMD_NOT_FOUND);
}

/* Forks depending on execution context
 * a null cmd name is valid, nothing runs
 */
static int	_run_cmd(t_state *s, t_ast_node *a)
{
	t_cmd *c = get_cmd(s);

	if (p_get_type(a) != AST_NODE_CMD || NULL == p_get_cmd(a))
		return (EINVAL);
	if ( 0 != _find_and_validate_cmd(p_get_cmd(a), &c->fullpath))
		return (ERR_CMD_NOT_FOUND);
	if (c->fullpath)
		debug_print("Found command! at %s\n", c->fullpath);
	if (CTXT_PIPELINE == st_peek(get_cmd(s)->st) \
		|| CTXT_PROC == st_peek(get_cmd(s)->st))
	{
		if (execve(c->fullpath, c->argv, get_envp(s)) == -1)
			err("ERR execve()\n");
	}
	else
		if (0 != fork_and_run(s))
			err("ERR fork and run\n");
	return (0);
}

/* Checks for no command, builtin (1), then PATH (2)
 */
int	cmd_execute_simple(t_state *s, t_ast_node *a)
{
	char **args;
	int exit_code;
	t_cmd *c = get_cmd(s);

	exit_code = -1;
	if (!a)
		return (-1);
	if (p_get_type(a) != AST_NODE_CMD)
		return (-1);
	args = p_do_arg_expansions(a);
	if (true == p_get_expansion(a) && !p_get_cmd(a)) // handle no command
		p_set_cmd(a, args[0]); // deep copies 
	if (false == p_get_expansion(a) && !p_get_cmd(a))
		return (0); // is this the right code?
	const t_builtin_fn bi = get_builtin(p_get_cmd(a));
	if (bi)
	// TODO handle redirects for builtins
		exit_code = bi(args);
	else 
	{
		if (!c)
			return (-1);
		c->argv = c_argstoargv(args, p_get_cmd(a), p_get_argc(a));

		// Print debug statement before calling _run_cmd
		printf("Executing command: %s\n", p_get_cmd(a));
		printf("Arguments:\n");
		if (c->argv)
		{
			for (int i = 0; c->argv[i] != NULL; i++)
				printf("  argv[%d]: %s\n", i, c->argv[i]);
		}
		printf("  argv[%d]: (NULL)\n", p_get_argc(a) + 1);  // Ensure NULL terminator

		exit_code = _run_cmd(s, a);
	}
	return (exit_code);
}
