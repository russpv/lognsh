/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_parse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:26:48 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 17:15:09 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define CMD_NAME "export"
#define EMPTY_KEY 77
#define EMSG_BADMALLOC "memory allocation failed.\n"
#define EMSG_KEY_NOTVALID "not a valid identifier\n"

/* Tests key is of correct format */
static int	_validate_key(const char *key, int got_good_arg)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
	{
		if (ft_isspace(key[0]))
			if (1 == got_good_arg)
				return (EMPTY_KEY);
		print_custom_err(CMD_NAME, EMSG_KEY_NOTVALID);
		return (ERR_GENERAL);
	}
	i = 1;
	while (key[i])
	{
		fprintf(stderr, "testing key[i] got= %s_\n", key);
		if (!ft_isalnum(key[i]) && key[i] != '_')
			print_custom_err(CMD_NAME, EMSG_KEY_NOTVALID);
		i++;
	}
	return (0);
}

// validates the argument format and key syntax
static int	_process_arg_validation(const char *arg, char **equal_pos,
		int *error_occurred, int got_good_arg)
{
	if (!arg || !equal_pos || !error_occurred)
		return (0);
	*equal_pos = ft_strchr(arg, '=');
	if (*equal_pos)
	{
		**equal_pos = '\0';
		if (0 != _validate_key(arg, got_good_arg))
		{
			*error_occurred = 1;
			**equal_pos = '=';
			return (ERR_GENERAL);
		}
		**equal_pos = '=';
	}
	else if (0 != _validate_key(arg, got_good_arg))
	{
		*error_occurred = 1;
		return (ERR_GENERAL);
	}
	return (0);
}

// Processes a single export argument
int	parse_arg(t_state *s, const char *arg, int *error_occurred, \
	int got_good_arg)
{
	char	*equal_pos;
	int		res;

	if (!s || !arg || !error_occurred)
		return (ERR_ARGS);
	res = _process_arg_validation(arg, &equal_pos, error_occurred, \
		got_good_arg);
	if (EMPTY_KEY == res)
		return (0);
	else if (0 != res)
		return (ERR_GENERAL);
	if (0 != process_arg_update_add(s, arg, equal_pos, error_occurred))
		return (ERR_GENERAL);
	return (0);
}
