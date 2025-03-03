/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_do1.c                                     :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 10:47:28 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/18 18:16:22 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define CMD_NAME "export"
#define ERRMSG_BADMALLOC "memory allocation failed.\n"
#define ERRMSG_KEY_NOTVALID "not a valid identifier\n"

// adds a new environment variable to the list and syncs s->pwd if its PWD
static int	_add_new_var(t_env **sh_env_list, const char *key, \
			const char *value)
{
	t_env	*new_node;
	char	*new_value;

	if (!sh_env_list || !key)
		return (0);
	if (value)
		new_value = ft_strdup(value);
	else
		new_value = ft_strdup("");
	if (!new_value)
		return (0);
	new_node = create_env_node(key, new_value);
	if (!new_node)
	{
		free(new_value);
		return (0);
	}
	add_env_node(sh_env_list, new_node);
	free(new_value);
	return (1);
}

static char	*_extract_key(const char *arg, char *equal_pos)
{
	char	*key;
	int		key_len;

	key = NULL;
	if (!equal_pos)
		key = ft_strdup(arg);
	else if (*equal_pos)
	{
		key_len = equal_pos - arg;
		key = ft_strndup(arg, key_len);
	}
	return (key);
}

static void	_set_value(char *equal_pos, const char **value_ptr)
{
	if (equal_pos)
		*value_ptr = equal_pos + 1;
	else
		*value_ptr = NULL;
}

static int	_update_or_add_env(t_state *s, t_env *existing_key, \
			const char *key, const char *value)
{
	if (existing_key)
		return (_update_existing_var(existing_key, value));
	return (_add_new_var(get_sh_env_list_add(s), key, value));
}

// updates or adds key-value pair in s->sh_env_list
int	_process_arg_update_add(t_state *s, const char *arg, \
		char *equal_pos, int *error_occurred)
{
	t_env		*existing_key;
	const char	*value;
	char		*key;
	int			exit_code;

	if (!s || !arg || !error_occurred)
		return (0);
	key = _extract_key(arg, equal_pos);
	if (!key)
		return (0);
	_set_value(equal_pos, &value);
	if (!equal_pos)
	{
		free(key);
		return (1);
	}
	existing_key = env_find_key(get_sh_env_list(s), key);
	exit_code = _update_or_add_env(s, existing_key, key, value);
	free(key);
	if (!exit_code && equal_pos)
		*equal_pos = '=';
	if (!exit_code)
		*error_occurred = 1;
	return (exit_code);
}
