/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:27:21 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/13 13:13:01 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define _CMD_NAME_ "unset"

static bool	_is_valid_var_name(char *name)
{
	if (!name || !*name || ft_isdigit(*name))
		return (false);
	while (*name)
	{
		if (false == ft_isalnum(*name) && *name != '_')
			return (false);
		name++;
	}
	return (true);
}

// unset will always return ('0'); remove_env_node will only work for valid keys
/*int	bi_unset(t_state *s, char **argv, int argc)
{
	int	i;
	int	ret;

	if (!s || !argv)
		return (1);
	if (argc == 1)
		return (0);
	i = 1;
	ret = 0;
	while (argv[i])
	{
		ret = 0;
		if (false == _is_valid_var_name(argv[i]))
		{
			print_invalid_name(_CMD_NAME_, argv[i]);
			ret = 1;
		}
		else
		{
			if (0 == ft_strncmp(PATH_KEY, argv[i], MAX_ENVVAR_LEN))
				set_path(s, NULL);
			env_remove_node(get_mem(s), get_env_list_ptr(s), argv[i]);
		}
		i++;
	}
	return (ret);
}*/

static int	_process_var(t_state *s, char *var_name, int ret)
{
	if (!_is_valid_var_name(var_name))
	{
		print_invalid_name(_CMD_NAME_, var_name);
		ret = 1;
	}
	else
	{
		if (ft_strncmp(PATH_KEY, var_name, MAX_ENVVAR_LEN) == 0)
			set_path(s, NULL);
		env_remove_node(get_mem(s), get_env_list_ptr(s), var_name);
	}
	return (ret);
}

int	bi_unset(t_state *s, char **argv, int argc)
{
	int		i;
	int		ret;

	if (!s || !argv)
		return (1);
	if (argc == 1)
		return (0);
	i = 1;
	ret = 0;
	while (argv[i])
	{
		ret = _process_var(s, argv[i], ret);
		i++;
	}
	return (ret);
}
