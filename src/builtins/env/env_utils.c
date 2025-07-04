#include "env_int.h"

// Destroys llist
void	env_free_list(t_mem_mgr *m, t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		m->dealloc(&m->list, tmp->key);
		if (tmp->value)
			m->dealloc(&m->list, tmp->value);
		m->dealloc(&m->list, tmp);
	}
}

// print all env variables
void	env_print(t_env *env)
{
	while (env)
	{
		printf("%s=", env->key);
		if (env->value)
			printf("%s\n", env->value);
		else
			printf("\n");
		env = env->next;
	}
}

// get list size of env in linked list
static int	_get_env_list_size(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

// Returns "[key]=[val]" string
static char	*_lst_join_env(t_mem_mgr *m, char *key, char *val)
{
	char	*tmp;
	char	*res;

	if (!key)
		return (NULL);
	tmp = ft_strjoin_mem(&m->list, m->f, key, "=");
	if (!tmp)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, "Malloc");
	if (!val)
		return (tmp);
	res = ft_strjoin_mem(&m->list, m->f, tmp, val);
	if (!res)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, "Malloc");
	return (res);
}

// Converts t_env* to char**
char	**env_to_array(t_mem_mgr *m, t_env *env_list)
{
	char	**env_array;
	t_env	*tmp;
	int		i;

	if (!env_list)
		return (NULL);
	i = _get_env_list_size(env_list);
	env_array = m->f(&m->list, sizeof(char *) * (i + 1));
	if (!env_array)
		return (NULL);
	tmp = env_list;
	i = 0;
	while (tmp)
	{
		env_array[i] = _lst_join_env(m, env_get_node_key(tmp), \
env_get_node_value(tmp));
		if (!env_array[i])
			return (NULL);
		i++;
		tmp = tmp->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
