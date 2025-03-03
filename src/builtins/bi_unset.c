/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:27:21 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/16 15:15:53 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

// unset will always return '0'; remove_env_node will only work for valid keys
int	bi_unset(t_state *s, char **argv, int argc)
{
	int	i;

	if (!s || !argv)
		return (1);
	if (argc == 1)
		return (0);
	i = 1;
	while (argv[i])
	{
		remove_env_node(get_sh_env_list_add(s), argv[i]);
		i++;
	}
	return (0);
}
