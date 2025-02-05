#include "bi_int.h"

int	find_and_validate_cmd(const char *name, char **fullpath, const char *caller);

/* Takes first argument and
 */
int	bi_exec(t_state *s, char **argv, int argc)
{
	char	*fullpath;
	int		ret;
	char	**envp;

	if (!argv || !argv[1])
	{
		write(STDERR_FILENO, "Usage: exec <command>\n", sizeof("Usage: exec <command>\n"));
		return (-1);
	}
	fullpath = NULL;
	for (int i = 0; i < argc; i++) { //debugging
        //printf("Argument %d: %s\n", i, argv[i]);
    }
	ret = find_and_validate_cmd(argv[1], &fullpath, "exec");
	if (ret != 0)
		return (-1);
	envp = get_envp(s);
	if (execve(fullpath, argv + 1, envp) == -1)
	{
		perror("execve failed");
		free(fullpath);
		exit(1);
	}
	free(fullpath);
	return (0);
}
