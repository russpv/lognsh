#include "command_int.h"

void	destroy_cmd(void *c)
{
	t_cmd *cmd = (t_cmd *)c;
	st_destroy(cmd->st);
	free(cmd);
}

t_cmd *init_cmd(t_state *s, t_ast_node *a)
{
	t_cmd *c = malloc(sizeof(struct s_cmd));
	c->st = st_create();
	c->argv = NULL;
	c->fullpath = NULL;
	c->curr_node = a;
	c->saved_stderr = -1;
	c->saved_stdin = -1;
	c->saved_stdout = -1;
	set_command(s, c);
	register_command_destroy(s, destroy_cmd);
	return (c);
}

/* Can accept null args and cmd
 * If no cmd, copies first arg as cmd
 * Does not need to handle no-command situation
 */
char **c_argstoargv(char **args, char *cmd, int argc)
{
	char **argv;
	int i;

	i = -1;
	argv = malloc(sizeof(char *) * (argc + 2));
	if (argv)
	{
		while (++i <= argc)
		{
			if (cmd && i == 0)
				argv[i] = ft_strdup(cmd);
			else if (!cmd && i == 0 && args && argc > 0)
				argv[i] = ft_strdup(args[i]);
			else if (cmd && i != 0 && args && argc > 0)
				argv[i] = ft_strdup(args[i - 1]);
			else if (!cmd && i != 0 && args && argc > 0)
				argv[i] = ft_strdup(args[i]);
			if (!argv[i])
			{
				ft_freearr((void **)argv, -1);
				return (NULL);
			}
		}
		argv[i] = NULL;
	}
	return (argv);
}

int cmd_execute_full(t_state *s, t_ast_node *a)
{
	int exit_code = -1;

	init_cmd(s, a);
	print_ast(a, 10);
	exit_code = cmd_execute_simple(s, a);
	return (exit_code);
}
