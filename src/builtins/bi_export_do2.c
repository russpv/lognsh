/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_do2.c                                     :+:      :+:    :+:  */
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

// Returns 0 if error.
/* Returns 1 if key is alphanumeric */
static int	validate_key(const char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
	{
		print_custom_err(CMD_NAME, ERRMSG_KEY_NOTVALID);
		return (0);
	}
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
		{
			print_custom_err(CMD_NAME, ERRMSG_KEY_NOTVALID);
			return (0);
		}
		i++;
	}
	return (1);
}

// validates the argument format and key syntax
static int	_process_arg_validation(const char *arg, \
			char **equal_pos, int *error_occurred)
{
	if (!arg || !equal_pos || !error_occurred)
		return (0);
	*equal_pos = ft_strchr(arg, '=');
	if (*equal_pos)
	{
		**equal_pos = '\0';
		if (!validate_key(arg))
		{
			*error_occurred = 1;
			**equal_pos = '=';
			return (0);
		}
		**equal_pos = '=';
	}
	else if (!validate_key(arg))
	{
		*error_occurred = 1;
		return (0);
	}
	return (1);
}

// main function  to process a single export argument
int	process_arg(t_state *s, const char *arg, int *error_occurred)
{
	char	*equal_pos;

	if (!s || !arg || !error_occurred)
		return (0);
	if (!_process_arg_validation(arg, &equal_pos, error_occurred))
		return (0);
	if (!_process_arg_update_add(s, arg, equal_pos, error_occurred))
		return (0);
	return (1);
}
