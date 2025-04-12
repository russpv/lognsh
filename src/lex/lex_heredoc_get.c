/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_heredoc_get.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:30 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:32:31 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

#define DBGMSG_FOUNDDEL "%s: Heredoc delimiter found, exiting heredoc mode.\n"
#define LOGMSG_SIG "%s: Interrupted.\n"

/* Puts readline line onto buf. Omits '\0' from readline. */
static inline int	_load_line(t_lex *l, const char *line)
{
	if (true == is_too_long(l, line))
		return (ERR_BUFFLOW);
	while (*line)
		l->buf[(l->buf_idx)++] = *line++;
	l->buf[(l->buf_idx)++] = '\n';
	l->lines++;
	return (0);
}

static inline bool	_line_is_eof(t_lex *l, const char *line)
{
	if (!*line)
		return (false);
	if (0 == ft_strncmp(line, l->eof_word, ft_strlen(line)))
		return (true);
	return (false);
}

/* Creates heredoc_body token up until EOF or exits if NULL (Ctrl+D)
 * Runs readline, loads buf until line return equals EOF
 *
 * Note: Bash/Zsh do not trim the line when testing EOF (" EOF" != EOF)
 * only when capturing the EOF value
 * Memory allocations are local.
 */
int	match_heredoc(t_mem_mgr *m, t_lex *l)
{
	char	*line;

	(void)m;
	while (1)
	{
		line = readline(HDOC_PROMPT);
		if (NULL == line)
		{
			dprint(LOGMSG_SIG, _MOD_);
			free(line);
			return (ERR_RL_ABORTED);
		}
		if (true == _line_is_eof(l, line))
		{
			dprint(DBGMSG_FOUNDDEL, _MOD_);
			free(line);
			return (0);
		}
		if (0 != _load_line(l, line))
			return(free(line), ERR_RL_ABORTED);
		free(line);
	}
}
