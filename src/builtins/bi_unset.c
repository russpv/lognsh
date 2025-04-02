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

static int	is_valid_var_name(char *name)
{
	if (!name || !*name || ft_isdigit(*name))
		return (0);
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

// unset will always return ('0'); remove_env_node will only work for valid keys
int	bi_unset(t_state *s, char **argv, int argc)
{
	int	i;
	int	ret;

	if (!s || !argv)
		return (1);
	if (argc == 1)
		return (0);
	i = 1;
	while (argv[i])
	{
		ret = 0;
		if (!is_valid_var_name(argv[i]))
		{
			write(STDERR_FILENO, "unset: ", 7);
			write(STDERR_FILENO, argv[i], ft_strlen(argv[i]));
			write(STDERR_FILENO, ": invalid parameter name\n", 25);
			ret = 1;
		}
		else
			env_remove_node(get_mem(s), get_env_list_add(s), argv[i]);
		i++;
	}
	return (ret);
}
