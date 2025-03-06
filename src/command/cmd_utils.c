#include "command_int.h"

/* Builds argv from args and stores in command
 * If no cmd, copies first arg as cmd
 * If no args, ...
 */
int	c_argstoargv(t_state *s, t_cmd *cmd, t_ast_node *a, char **args)
{
	const char	*cmdname = p_get_cmd(a);
	const int	argc = p_get_argc(a);
	char		**argv;
	int			i;

	i = -1;
	if (!s || !a || !cmd)
		return (ERR_ARGS);
	argv = malloc(sizeof(char *) * ((size_t)argc + 2));
	if (argv)
	{
		debug_print(_MOD_ ": %s: got cmd %s argc:%d args:%p \n",__FUNCTION__, cmdname, argc, args);
		while (++i <= argc)
		{
			if (cmdname)
			{
				if (i == 0)
					argv[0] = ft_strdup(cmdname);
				else if (i != 0 && args && argc > 0)
					argv[i] = ft_strdup(args[i - 1]);
			}
			else if (args && argc > 0)
			{
				if (i == 0)
					argv[0] = ft_strdup(args[0]);
				else
					argv[i] = ft_strdup(args[i]);
			}
			else
				return (ERR_ARGS);
			if (!argv[i])
				return (err(EMSG_MALLOC), ((void **)args, -1), \
						ft_freearr((void **)argv, -1), ERR_MEM);
		}
		argv[i] = NULL;
		cmd->argvc = i; 
		cmd->argv = argv;
		if (NULL == cmdname)
			p_set_cmd(a, argv[0]);
		ft_freearr((void **)args, -1);
	}
	return (0);
}

void print_pipes(t_cmd *c)
{
    int i;

    if (c == NULL || c->fildes == NULL) 
	{
        debug_print("Error: fildes is NULL\n");
        return;
    }
    i = -1;
    while (++i < c->curr_cmdc - 1)
    {
        debug_print("Cmd: \tPipe %d: read fd=%d, write fd=%d", \
		i, c->fildes[i][0], c->fildes[i][1]);
    }
}
