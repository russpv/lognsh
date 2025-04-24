
#include "lex_int.h"

#define EMSG_NOEOF "ERR heredoc EOF not found.\n"

// This should skip whitespace only
// Readline allows ' ' only
static inline void	_skip_to_next_op(t_lex *l)
{
	if ('\0' == *l->ptr)
		return ;
	while (*l->ptr && ft_isspace(*l->ptr))
	{
		l->ptr++;
		if ('\0' == *l->ptr)
			break ;
	}
}

// Taking a less expressive stance on delimiter content
static inline bool	_is_hdoc_eof_char(unsigned char c)
{
	if (ft_isalnum((int)c))
		return (true);
	if ('_' == c)
		return (true);
	if ('\'' == c)
		return (true);
	if ('\"' == c)
		return (true);
	return (false);
}

// Determines type of EOF, quoted or not
// Cannot be an expansion. If any part of the word is quoted,
// sets processing type. Accepts any chars within matching quotes
static inline void	_put_eof_in_buf(t_lex *l)
{
	char	quote;

	while (*l->ptr && *l->ptr != '\n' && !ft_isspace(*l->ptr)
		&& _is_hdoc_eof_char(*l->ptr))
	{
		if ('\'' == *l->ptr || '\"' == *l->ptr)
		{
			l->do_expansion = false;
			quote = *l->ptr++;
			while (*l->ptr && quote != *l->ptr)
			{
				l->buf[l->buf_idx++] = *l->ptr++;
				if ('\0' == *l->ptr)
					return ;
			}
			l->ptr++;
		}
		else
			l->buf[l->buf_idx++] = *l->ptr++;
		if ('\0' == *l->ptr)
			return ;
	}
}

// Returns eof delimiter
// Skips any spaces. Also flushes buf to prepare body token
// Sets expansion flags
int	get_eof_word(t_mem_mgr *m, t_lex *l)
{
	if (!l->ptr)
		return (ERR_ARGS);
	if (true == is_too_long(l, l->ptr))
		return (ERR_BUFFLOW);
	while (ft_isspace(*l->ptr))
		l->ptr++;
	l->do_expansion = true;
	_put_eof_in_buf(l);
	if (0 == ft_strlen(l->buf))
		return (print_lex_error(l, "<<"), err(EMSG_NOEOF), ERR_SYNTAX);
	_skip_to_next_op(l);
	l->eof_word = ft_strdup_tmp(m, l->buf);
	if (!l->eof_word)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	dprint(_MOD_ ": captured heredoc delimiter: %s\n", l->eof_word);
	ft_memset(l->buf, 0, LEX_BUFSZ);
	l->buf_idx = 0;
	return (0);
}
