/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:27:21 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/16 15:15:53 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

void	env_remove_key(t_env **env_list, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
		}
		prev = current;
		current = current->next;
	}
}

int	bi_unset(t_state *s, char **argv, int argc)
{
	int	i;

	if (argc == 1)
		return (0);
	i = 1;
	while (argv[i])
	{
		if (!is_valid_key(argv[i]))
		{
			i++;
			continue ;
		}
		env_remove_key(&(s->sh_env_list), argv[i]);
		i++;
	}
	return (0);
}
