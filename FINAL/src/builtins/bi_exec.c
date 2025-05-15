/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:26:29 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/13 15:59:25 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

extern int	find_and_validate_cmd(t_state *s, const char *name, char **fullpath,
				const char *caller);
extern char	**environ;

/* Takes first argument
 * Note: retains state, does not do cleanup or pass shell vars.
 */
int	bi_exec(t_state *s, char **argv, int argc)
{
	char	*fullpath;
	int		ret;

	(void)argc;
	if (!argv || !argv[1])
	{
		write(STDERR_FILENO, "Usage: exec <command>\n", \
sizeof("Usage: exec <command>\n"));
		return (-1);
	}
	fullpath = NULL;
	ret = find_and_validate_cmd(s, argv[1], &fullpath, "exec");
	if (ret != 0)
		return (-1);
	if (execve(fullpath, argv + 1, environ) == -1)
	{
		perror(EMSG_EXECVE);
		free(fullpath);
		exit(ERR_GENERAL);
	}
	free(fullpath);
	return (0);
}
