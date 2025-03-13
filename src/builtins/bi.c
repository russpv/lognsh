/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:17:24 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/13 14:13:41 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "bi_int.h"

// Fn to map command names to corresponding built-ins
t_builtin_fn	get_builtin(char *command)
{
	debug_print(_MOD_": %s...\n", __FUNCTION__);
	if (NULL != command)
	{
		if (ft_strcmp(command, BI_ECHO) == 0)
			return (&bi_echo);
		if (ft_strcmp(command, BI_PWD) == 0)
			return (&bi_pwd);
		if (ft_strcmp(command, BI_EXIT) == 0)
			return (&bi_exit);
		if (ft_strcmp(command, BI_EXEC) == 0)
			return (&bi_exec);
		if (ft_strcmp(command, BI_ENV) == 0)
			return (&bi_env);
		if (ft_strcmp(command, BI_CD) == 0)
			return (&bi_cd);
		if (ft_strcmp(command, BI_UNSET) == 0)
			return (&bi_unset);
		if (ft_strcmp(command, BI_EXPORT) == 0)
			return (&bi_export);
	}
	debug_print(_MOD_ ": %s found nothing\n", __FUNCTION__);
	return (NULL);
}

void	flushbuf(void)
{
	if (0 != TESTFLAG)
		if (0 == isatty(STDOUT_FILENO))
			fflush(stdout);
}
