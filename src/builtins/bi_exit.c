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

#define CMD_NAME "exit"
#define ERRMSG_ARG_NONNUM "numeric argument required\n"

static void	_exit_no_arg(t_state *s)
{
	int	exit_code;

	exit_code = *(get_status(s));
	destroy_state(s);
	exit(exit_code);
}

static void	_exit_nonnumeric_arg(t_state *s)
{
	destroy_state(s);
	print_custom_err(CMD_NAME, ERRMSG_ARG_NONNUM);
	exit(255);
}

static void	_exit_too_many_args(t_state *s)
{
	destroy_state(s);
	print_too_many_args(CMD_NAME);
	exit(ERR_GENERAL);
}

static void	_exit_valid_arg(t_state *s, char *arg)
{
	int	exit_code;

	exit_code = ft_atoi(arg) % 256;
	if (exit_code < 0)
		exit_code = exit_code + 256;
	destroy_state(s);
	exit(exit_code);
}

/* EXIT
 * Exits with the last exit status, or
 * the numeric argument given. 
 */
int	bi_exit(t_state *s, char **args, int argc)
{
	(void)argc;
	write(STDOUT_FILENO, "exit\n", sizeof("exit\n"));
	if (!args[1])
		_exit_no_arg(s);
	else if (!is_numeric(args[1]))
		_exit_nonnumeric_arg(s);
	else if (args[2])
		_exit_too_many_args(s);
	else
		_exit_valid_arg(s, args[1]);
	return (0);
}
