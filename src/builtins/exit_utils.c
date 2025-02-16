/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:05:10 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/07 19:25:09 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (i > 0);
}

void	handle_no_arg(t_state *s)
{
	int	exit_code;

	exit_code = *(get_status(s));
	destroy_state(s);
	exit(exit_code);
}

void	handle_invalid_arg(t_state *s)
{
	destroy_state(s);
	write(STDERR_FILENO, "minishell: exit: numeric argument required\n", 44);
	exit(255);
}

void	handle_too_many_args(t_state *s)
{
	destroy_state(s);
	write(STDERR_FILENO, "minishell: exit: too many arguments\n", 36);
	exit (1);
}

void	handle_valid_arg(t_state *s, char *arg)
{
	int	exit_code;

	exit_code = ft_atoi(arg) % 256;
	if (exit_code < 0)
		exit_code = exit_code + 256;
	destroy_state(s);
	exit(exit_code);
}
