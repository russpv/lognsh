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

int	bi_exit(t_state *s, char **args, int argc)
{
	(void)argc;
	write(STDOUT_FILENO, "exit\n", 5);
	if (!args[1])
		handle_no_arg(s);
	else if (!is_numeric(args[1]))
		handle_invalid_arg(s);
	else if (args[2])
		handle_too_many_args(s);
	else
		handle_valid_arg(s, args[1]);
	return (0);
}
