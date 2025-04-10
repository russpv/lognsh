/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:05 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:33:06 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

inline bool	is_too_long(const char *input)
{
	if (NULL == input)
		return (true);
	if (ft_strlen(input) >= LEX_BUFSZ)
	{
		dprint(_MOD_ ": ERROR: Input exceeds buf size.\n");
		return (true);
	}
	return (false);
}

/* VARNAMELEN
** This returns length of the string pointed to by 'c'
** As long as it is a shell variable name
** NOT including '\0' (e.g. 'size')
** Will NOT SEGV if c is NULL (UNlike strlen())
*/
size_t	ft_varnamelen(const char *c)
{
	size_t	i;

	i = 0;
	if (!c)
		return (0);
	if (ft_isdigit(c[0]))
		return (0);
	while (c[i] && is_varnamechar(c[i]))
		i++;
	return (i);
}

/* Puts single input char on buf, and increments idx and ptr */
int	put_on_buf(t_lex *l)
{
	dprint(_MOD_ ": %s: _%c_\n", __FUNCTION__, *l->ptr);
	if (l->buf_idx < LEX_BUFSZ - 1)
		l->buf[(l->buf_idx)++] = *l->ptr++;
	else
		return (print_lex_buffer_overflow(), ERR_BUFFLOW);
	return (0);
}
