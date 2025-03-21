/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_add.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 10:47:28 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/19 15:00:37 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define CMD_NAME "export"
#define EMSG_BADMALLOC "memory allocation failed.\n"
#define EMSG_KEY_NOTVALID "not a valid identifier\n"

// adds a new environment variable to the list and syncs s->pwd if its PWD
static int	_add_new_var(t_mem_mgr *m, t_env **sh_env_list, const char *key,
		const char *value)
{
	t_env	*new_node;
	char	*new_value;

	new_value = NULL;
	if (!sh_env_list || !key)
		return (ERR_ARGS);
	if (value)
		new_value = ft_strdup(value);
	else
		new_value = ft_strdup("");
	if (!new_value)
		return (ERR_GENERAL);
	new_node = create_env_node(m, key, new_value);
	if (!new_node)
	{
		free(new_value);
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	}
	env_add_node(sh_env_list, new_node);
	free(new_value);
	return (0);
}

// Returns key string, or copy of arg if no equal sign found
static int	_extract_key(char key[], const char *arg, char *equal_pos)
{
	size_t	key_len;

	if (!arg)
		return (ERR_ARGS);
	if (!*arg)
		return (ERR_ARGS);
	if (!equal_pos)
	{
		key_len = ft_strnlen(arg, MAX_RAW_INPUT_LEN);
		ft_strscpy(key, arg, key_len + 1);
	}
	else if (*equal_pos)
	{
		key_len = equal_pos - arg;
		if (key_len < MAX_ENVVAR_LEN)
			ft_strscpy(key, arg, key_len + 1);
		else
			return (E2BIG);
	}
	return (0);
}

// Returns ptr to the start of value
static void	_set_value(char *equal_pos, const char **value_ptr)
{
	if (equal_pos)
		*value_ptr = equal_pos + 1;
	else
		*value_ptr = NULL;
}

// Commits change
static int	_update_or_add_env(t_state *s, t_env *existing_key, const char *key,
		const char *value)
{
	if (existing_key)
		return (update_existing_var(get_mem(s), existing_key, value));
	return (_add_new_var(get_mem(s), get_env_list_add(s), key, value));
}

// updates or adds key-value pair in s->sh_env_list
// assumes equal_pos is within arg
int	process_arg_update_add(t_state *s, const char *arg, char *equal_pos,
		int *error_occurred)
{
	t_env		*existing_key;
	const char	*value;
	char		key[MAX_ENVVAR_LEN];
	int			exit_code;

	if (!s || !arg || !error_occurred || !equal_pos)
		return (ERR_ARGS);
	ft_memset(key, 0, MAX_ENVVAR_LEN);
	_extract_key(key, arg, equal_pos);
	if (0 == ft_strnlen(key, MAX_ENVVAR_LEN))
		return (ERR_ARGS);
	_set_value(equal_pos, &value);
	existing_key = find_env_key(get_env_list(s), key);
	exit_code = _update_or_add_env(s, existing_key, key, value);
	if (0 != exit_code)
		*error_occurred = 1; // TODO, check what this flag does
	return (exit_code);
}
