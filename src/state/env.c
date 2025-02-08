#include "env.h"

/* returns new paths array 
 * getenv does not rerurn heap memory */
char **s_getenv(void)
{
	char **res;
	const char *path_env = getenv("PATH");

	if (!path_env)
	{
		perror("getenv error\n");
		return (NULL);
	}
	res = ft_split(path_env, ':');
	if (!res)
	{
		perror("ft_split error\n");
		return (NULL);
	}
	return (res);
}

/*
get_env_var(const char *key)
{
	//TODO
}

// add key value pair to ENV
set_env_var(char *key, char *value)
{
	//TODO
}

char	*search_path(const char *command)
{
	//TODO
}

char	**expand_glob(const char *pattern)
{
	//TODO
}

ht	*build_ht(void)
{
	//TODO
}
*/