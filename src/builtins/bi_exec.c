#include "bi_int.h"

int	find_and_validate_cmd(const char *name, char **fullpath);

/* Takes first argument and
 */
int	bi_exec(t_state *s, char **argv, int argc)
{
	char	*fullpath;
	int		ret;
	char	**envp;

	if (!argv || !argv[1])
	{
		fprintf(stderr, "Usage: exec <command>\n");
		return (-1);
	}
	fullpath = NULL;
	for (int i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }
	ret = find_and_validate_cmd(argv[1], &fullpath);
	if (ret != 0)
	{
		fprintf(stderr, "Command not found: %s\n", argv[1]);
		return (-1);
	}
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
