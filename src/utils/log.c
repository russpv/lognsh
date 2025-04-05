/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:58:26 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:58:29 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parse/parse.h"
#include "log.h"

void	log_command_info(t_cmd *c, t_ast_node *a)
{
	int	i;

	i = -1;
	if (LOGGING)
	{
		if (p_get_cmd(a))
			colored_printf(YELLOW, "\tExecuting command: %s\n", p_get_cmd(a));
		else
			colored_printf(RED, "\tExecuting command: (NULL)\n");
		colored_printf(YELLOW, "\tArguments:\n");
		if (c_get_argv(c))
		{
			while (++i < c_get_argvc(c))
				colored_printf(YELLOW, "\t  argv[%d]: _%s_\n", i,
					c_get_argv(c)[i]);
		}
		if (p_get_argc(a) > 0 && c_get_argv(c)
			&& c_get_argv(c)[p_get_argc(a)] == NULL)
			colored_printf(YELLOW, "\t  argv[%d]: (NULL)\n", p_get_argc(a));
	}
	(void)i;
}

void	log_print(const char *s, ...)
{
	va_list	args;

	if (LOGGING && isatty(STDERR_FILENO))
	{
		va_start(args, s);
		fprintf(stderr, CYAN "[LOG] ");
		vfprintf(stderr, s, args);
		fprintf(stderr, CYAN RESET);
		va_end(args);
		fflush(stderr);
	}
}

void	colored_printf(const char *color, const char *format, ...)
{
	va_list	args;

	if (LOGGING && isatty(STDERR_FILENO))
	{
		va_start(args, format);
		fprintf(stderr, "%s", color);
		vfprintf(stderr, format, args);
		fprintf(stderr, "%s", RESET);
		va_end(args);
		fflush(stderr);
	}
}

// alternate debug message format
void	err(const char *s)
{
	if (LOGGING)
		fprintf(stderr, RED "%s" RESET, s);
	else
		(void)s;
}
