/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:26:52 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:26:53 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"
#define CMD_NAME "export"
#define EMSG_INVLD "invalid state or arguments\n"
#define MSG_NOARGS "results are not specified with no arguments(refer to man)\n"

/* EXPORT
 * export [name[=value]]
 * Marks each name to be passed to child processes in the environment,
 * as long as equal sign is present.
 * Sets error exit code if nothing was exported.
 * Maximum name length is _
 * Maximum value length is _
 */
int	bi_export(t_state *s, char **argv, int argc)
{
	int	error_occurred;
	int	i;

	if (!s || !argv)
		return (print_custom_err(CMD_NAME, EMSG_INVLD), 1);
	if (argc == 1)
		return (print_custom_err(CMD_NAME, EMSG_INVLD), 0);
	error_occurred = 0;
	i = 1;
	while (i < argc)
	{
		if (!argv[i])
		{
			error_occurred = 1;
			i++;
			continue ;
		}
		if (0 != parse_arg(s, argv[i], &error_occurred))
			return (ERR_GENERAL);
		i++;
	}
	if (error_occurred)
		return (ERR_GENERAL);
	return (0);
}
