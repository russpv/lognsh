/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_heredoc_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:46:57 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/06 00:47:24 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"

#define EMSG_MEM "Exec: ##### Error heredoc\n"

size_t	write_heredoc(int fd, t_lex *l)
{
	int	len;

	len = ft_strlen(l->buf);
	return (write(fd, l->buf, len + 1));
}

static void	dealloc_str(t_state *s, char *str_to_free)
{
	t_mem_mgr	*mem_mgr;

	mem_mgr = get_mem(s);
	mem_mgr->dealloc(&mem_mgr->list, str_to_free);
}

/* Uses readline() and handles Ctrl+D
 * Entire heredoc content loads into one token.
 * Any trailing words are skipped until an OP
 */
int	tokenize_heredoc(t_state *s, t_lex *lexer)
{
	t_tok	*token;
	int		res;

	res = get_eof_word(get_mem(s), lexer);
	if (0 != res)
		return (res);
	res = exec_heredoc(get_mem(s), lexer);
	if (0 != res)
		return (res);
	dealloc_str(s, lexer->eof_word);
	lexer->eof_word = NULL;
	token = lex_create_token(get_mem(s), lexer, TOK_HEREDOC_WORD);
	if (token)
	{
		if (0 != add_token(get_mem(s), lexer, token))
			return (ERR_GENERAL);
		set_got_heredoc(s);
	}
	else
		return (write(STDERR_FILENO, EMSG_MEM, 26), ERR_MEM);
	return (0);
}
