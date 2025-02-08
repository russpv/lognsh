#include "command_int.h"

/* Can accept null args and cmd
 * If no cmd, copies first arg as cmd
 * Does not need to handle no-command situation
 */
char	**c_argstoargv(char **args, char *cmd, int argc)
{
	char	**argv;
	int		i;

	i = -1;
	argv = malloc(sizeof(char *) * ((size_t)argc + 2));
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
				ft_freearr((void **)args, -1);
				ft_freearr((void **)argv, -1);
				return (NULL);
			}
		}
		argv[i] = NULL;
		ft_freearr((void **)args, -1);

	}
	return (argv);
}
