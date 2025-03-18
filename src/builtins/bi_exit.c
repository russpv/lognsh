/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:05:10 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/07 16:57:22 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define CMD_NAME "exit"
#define EMSG_NONNUM "numeric argument required."

static int	is_numeric(const char *str)
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

static void	_exit_no_arg(t_state *s)
{
	int	exit_code;

	exit_code = *(get_status(s));
	destroy_state(s);
	exit(exit_code);
}

static void	_exit_nonnumeric_arg(t_state *s)
{
	print_custom_err(CMD_NAME, EMSG_NONNUM);
	destroy_state(s);
	exit(2);
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
 * the numeric argument given (within one byte).
 */
 int	bi_exit(t_state *s, char **argv, int argc)
 {
 	//int	exit_code;
 	
 	if (isatty(STDOUT_FILENO))
 		write(STDOUT_FILENO, "exit\n", 5);
 	if (argc < 2)
 		_exit_no_arg(s);
 	else if (!is_numeric(argv[1]))
 		_exit_nonnumeric_arg(s);  //Return, don't exit (but bash DOES exit)
 	else if (argv[2])
	{
		print_too_many_args(CMD_NAME);
 		return (1);
 	}
 	else
 		_exit_valid_arg(s, argv[1]);
 	return (0);
}
