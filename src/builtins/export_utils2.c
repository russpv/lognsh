/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 10:47:28 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/16 15:16:00 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

int	update_existing_var(t_env *existing_key, const char *value)
{
	free(existing_key->value);
	existing_key->value = ft_strdup(value);
	if (!existing_key->value)
	{
		perror("minishell: export: memory allocation failed.");
		return (0);
	}
	return (1);
}

int	add_new_var(t_env **sh_env_list, const char *key, const char *value)
{
	t_env	*new_node;

	new_node = create_env_node(key, value);
	if (!new_node->key || !new_node->value)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (0);
	}
	add_env_node(sh_env_list, new_node);
	return (1);
}

int	process_arg_validation(const char *arg, char **equal_pos, \
		int *error_occurred)
{
	*equal_pos = ft_strchr(arg, '=');
	if (!*equal_pos)
		return (handle_no_equal(arg, error_occurred));
	**equal_pos = '\0';
	if (!validate_key(arg) || !handle_underscore(arg))
	{
		*error_occurred = 1;
		**equal_pos = '=';
		return (0);
	}
	return (1);
}

int	process_arg_update_add(t_state *s, const char *arg, \
		char *equal_pos, int *error_occurred)
{
	t_env	*existing_key;

	existing_key = env_find_key(s->sh_env_list, arg);
	if (existing_key)
	{
		if (!update_existing_var(existing_key, equal_pos +1))
		{
			*error_occurred = 1;
			*equal_pos = '=';
			return (0);
		}
	}
	else
	{
		if (!add_new_var(&(s->sh_env_list), arg, equal_pos + 1))
		{
			*error_occurred = 1;
			*equal_pos = '=';
			return (0);
		}
	}
	*equal_pos = '=';
	return (1);
}

int	process_arg(t_state *s, const char *arg, int *error_occurred)
{
	char	*equal_pos;

	if (!process_arg_validation(arg, &equal_pos, error_occurred))
		return (0);
	if (!process_arg_update_add(s, arg, equal_pos, error_occurred))
		return (0);
	return (1);
}
