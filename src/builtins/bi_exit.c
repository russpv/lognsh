/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:05:10 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/28 17:55:00 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define CMD_NAME "exit"
#define EMSG_NONNUM "Numeric argument required.\n"
#define MAX_ARGSZ 20
#define NOARG 1
#define NONNUM 2

static int	_load_buf(char buf[], const char *str)
{
	int	i;
	int buf_idx;

	buf_idx = 0;
	i = -1;
	if (str[0] == '-' || str[0] == '+')
		buf[buf_idx++] = str[++i];
	while (str[++i] && buf_idx < MAX_ARGSZ)
	{
		if (str[i] != '\"' && (str[i] < '0' || str[i] > '9'))
			return (-1);
		if (str[i] != '\"')
			buf[buf_idx++] = str[i];
	}
	return (0);
}

static char	*get_numeric(t_state *s, const char *str)
{
	char *res;
	char buf[MAX_ARGSZ];

	res = NULL;
	ft_memset(buf, 0, MAX_ARGSZ);
	if (!str || str[0] == '\0')
		return (NULL);
	if (0 != _load_buf(buf, str))
		return (res);
	if (*buf)
	{
		res = ft_strdup_tmp(get_mem(s), buf);
		if (!res)
			exit_clean(&get_mem(s)->list, ENOMEM, CMD_NAME, EMSG_MALLOC);
	}
	return (res);
}

static void	_exit_arg_cond(t_state *s, int cond)
{
	int	exit_code;

	if (NOARG == cond)
		exit_code = *(get_status(s));
	else
		print_custom_err(CMD_NAME, EMSG_NONNUM);
	destroy_state(&s);
	if (NOARG == cond)
		exit(exit_code);
	else
		exit(2);
}

static void	_exit_valid_arg(t_state *s, char *arg)
{
	int	exit_code;

	exit_code = ft_atoi(arg) % 256;
	if (exit_code < 0)
		exit_code = exit_code + 256;
	destroy_state(&s);
	exit(exit_code);
}

/* EXIT
 * Exits with the last exit status, or
 * the numeric argument given (within one byte).
 */
int	bi_exit(t_state *s, char **argv, int argc)
{
	char *arg;
	
	if (isatty(STDOUT_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
	if (argc < 2)
		_exit_arg_cond(s, NOARG); 
	if ((ft_strlen(argv[1]) > 19))
		_exit_arg_cond(s, NONNUM); 
	arg = get_numeric(s, argv[1]);
	if (NULL == arg)
		_exit_arg_cond(s, NONNUM);
	if (argv[2])
	{
		print_too_many_args(CMD_NAME);
		return (1);
	}
	else
		_exit_valid_arg(s, arg);
	return (0);
}
