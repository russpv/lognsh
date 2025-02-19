/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_do.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 10:47:28 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/18 18:16:22 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

# define CMD_NAME "export"
# define ERRMSG_BADMALLOC "memory allocation failed.\n"

// Returns 0 if error.
static int	_update_existing_var(t_env *existing_key, const char *value)
{
	if (!existing_key || !value)
		return (1);
	if (existing_key->value)
		free(existing_key->value);
	existing_key->value = ft_strdup(value);
	if (!existing_key->value)
	{
		print_custom_err(CMD_NAME, ERRMSG_BADMALLOC);
		return (0);
	}
	return (1);
}

static int	_add_new_var(t_env **sh_env_list, const char *key, const char *value)
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

static int	_process_arg_validation(const char *arg, char **equal_pos, \
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

static int	_process_arg_update_add(t_state *s, const char *arg, \
		char *equal_pos, int *error_occurred)
{
	t_env	*existing_key;

	existing_key = env_find_key(s->sh_env_list, arg);
	if (existing_key)
	{
		if (!_update_existing_var(existing_key, equal_pos +1))
		{
			*error_occurred = 1;
			*equal_pos = '=';
			return (0);
		}
	}
	else
	{
		if (!_add_new_var(&(s->sh_env_list), arg, equal_pos + 1))
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

	if (!_process_arg_validation(arg, &equal_pos, error_occurred))
		return (0);
	if (!_process_arg_update_add(s, arg, equal_pos, error_occurred))
		return (0);
	return (1);
}
