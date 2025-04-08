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
