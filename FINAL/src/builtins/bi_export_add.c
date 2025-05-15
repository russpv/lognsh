/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_add.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:26:44 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/12 20:22:37 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define CMD_NAME "export"
#define EMSG_BADMALLOC "memory allocation failed.\n"

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
		key_len = ft_strnlen(arg, MAX_NAME_LEN);
		ft_strscpy(key, arg, key_len + 1);
	}
	else if (*equal_pos)
	{
		key_len = equal_pos - arg;
		if (key_len < MAX_NAME_LEN)
			ft_strscpy(key, arg, key_len + 1);
		else
			return (E2BIG);
	}
	return (0);
}

// Returns ptr to the start of value
static void	_reset_value_ptr(char *equal_pos, const char **value_ptr)
{
	if (equal_pos)
		*value_ptr = equal_pos + 1;
	else
		*value_ptr = NULL;
}

static void	_update_special_state_vars(t_state *s, char *key, const char *value)
{
	if (0 == ft_strncmp(PATH_KEY, key, MAX_NAME_LEN))
		set_path(s, value);
}

// updates or adds key-value pair in s->sh_env_list
// assumes equal_pos is within arg
int	process_arg_update_add(t_state *s, const char *arg, char *equal_pos,
		int *error_occurred)
{
	const char	*value;
	char		key[MAX_NAME_LEN];
	int			exit_code;

	if (!s || !arg || !error_occurred || !equal_pos)
		return (ERR_ARGS);
	ft_memset(key, 0, MAX_NAME_LEN);
	if (0 != _extract_key(key, arg, equal_pos))
		return (ERR_ARGS);
	if (0 == ft_strnlen(key, MAX_NAME_LEN))
		return (ERR_ARGS);
	_reset_value_ptr(equal_pos, &value);
	if (MAX_NAME_LEN == ft_strnlen(value, MAX_NAME_LEN))
		return (print_value_toolong(), ERR_ARGS);
	exit_code = env_upsert_value(get_mem(s), get_env_list(s), key, value);
	if (0 != exit_code)
		*error_occurred = 1;
	if (0 == exit_code)
		_update_special_state_vars(s, key, value);
	return (exit_code);
}
