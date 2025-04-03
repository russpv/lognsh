/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_add.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 10:47:28 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 11:33:16 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define CMD_NAME "export"
#define EMSG_BADMALLOC "memory allocation failed.\n"
#define EMSG_KEY_NOTVALID "not a valid identifier\n"

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

// updates or adds key-value pair in s->sh_env_list
// assumes equal_pos is within arg
int	process_arg_update_add(t_state *s, const char *arg, char *equal_pos,
		int *error_occurred)
{
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
	exit_code = env_upsert_value(get_mem(s), get_env_list(s), key, value);
	return (exit_code);
}
