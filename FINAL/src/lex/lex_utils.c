/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:05 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/13 16:01:31 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

bool	is_too_long(t_lex *l, const char *input)
{
	if (NULL == input)
		return (true);
	if (ft_strnlen(input, LEX_BUFSZ) >= \
LEX_BUFSZ - ft_strnlen(l->buf, LEX_BUFSZ))
	{
		dprint(_MOD_ ": ERROR: Input exceeds buf size.\n");
		print_lex_buffer_overflow();
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
	dvprint(_MOD_ ": %s: putting _%c_\n", __FUNCTION__, *l->ptr);
	if (l->buf_idx < LEX_BUFSZ - 1)
		l->buf[(l->buf_idx)++] = *l->ptr++;
	else if (!l->lex_err)
		return (lex_set_err(l), pbufflow(NULL), ERR_BUFFLOW);
	else
		return (ERR_BUFFLOW);
	return (0);
}

void	lex_set_err(t_lex *l)
{
	if (!l)
		return ;
	l->lex_err = 1;
}
