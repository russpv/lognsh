/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:08:43 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/13 13:57:52 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void	dprintdiv(const char *format, ...)
{
	va_list	args;

	if (DEBUG && isatty(STDERR_FILENO))
	{
		va_start(args, format);
		vfprintf(stderr, format, args);
		va_end(args);
		fflush(stderr);
	}
}

void	dprint(const char *format, ...)
{
	va_list	args;
	size_t	n;
	char	buf[DEBUG_BUF_SIZE];

	if (DEBUG && isatty(STDERR_FILENO))
	{
		va_start(args, format);
		n = vsnprintf(buf, sizeof(buf), format, args);
		va_end(args);
		fprintf(stderr, BLUE "[PID %d] [DEBUG] ", getpid());
		fputs(buf, stderr);
		if (n > DEBUG_BUF_SIZE)
			fprintf(stderr, " ...\n");
		fprintf(stderr, RESET);
		fflush(stderr);
	}
}

void	dvprint(const char *format, ...)
{
	va_list	args;

	if (DEBUGVERBOSE && isatty(STDERR_FILENO))
	{
		va_start(args, format);
		fprintf(stderr, BLUE "[PID %d] [DEBUG] ", getpid());
		vfprintf(stderr, format, args);
		fprintf(stderr, RESET);
		va_end(args);
		fflush(stderr);
	}
}
