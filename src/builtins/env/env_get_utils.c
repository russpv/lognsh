#include "env_int.h"

// this returns a system-managed string array. Do not modify!
char	**env_getenv_path(t_mem_mgr *m)
{
	char				**res;
	const char			*path_env = getenv("PATH");
	struct s_mem_utils	utils;

	if (!path_env)
	{
		perror("getenv\n");
		return (NULL);
	}
	mem_struct_init(m, &utils);
	res = ft_split_mem(&utils, path_env, ':');
	if (!res)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	return (res);
}

/* Returns env value for key if key exists, or NULL */
char	*env_find_value(const char *key, t_env *head)
{
	if (!key || !head)
		return (NULL);
	while (head)
	{
		if (ft_strcmp(head->key, key) == 0)
			return (head->value);
		head = head->next;
	}
	return (NULL);
}

char	*env_get_node_key(t_env *node)
{
	if (!node)
		return (NULL);
	return (node->key);
}

char	*env_get_node_value(t_env *node)
{
	if (!node)
		return (NULL);
	return (node->value);
}

t_env	*env_get_node_next(t_env *node)
{
	if (!node)
		return (NULL);
	return (node->next);
}
