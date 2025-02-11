/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:27:21 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/28 09:27:24 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

extern	char	**environ;

int	bi_unset(t_state *s, char **argv, int argc)
{
	
	int	i;
	int	j;
	
	(void)s;
	if (argc < 2)
		return (0);
	i = 1;
	while (argv[i])
	{
		j = 0;
		while (environ[j])
		{
			if (ft_strncmp(environ[j], argv[i], ft_strlen(argv[i])) == 0 \
				&& environ[j][ft_strlen(argv[i])] == '=')
			{
				while (environ[j])
				{
					environ[j] = environ[j + 1];
					j++;
				}
				break ;
			}
			j++;
		}
		i++;
	}
	return (0);
}

		
	
