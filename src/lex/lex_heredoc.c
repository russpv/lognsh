#include "lex_int.h"

#define DBGMSG_ANNOUNCE "%s: << tokenize_heredoc:\n"
#define DBGMSG_PTRAT "%s: ptr at:_%c_\n"
#define EMSG_MEM "%s: ##### Error heredoc\n"

static int	_getncount(char buf[], unsigned char c, size_t n)
{
	size_t	i;
	int		res;

	i = 0;
	res = 0;
	if (!buf || !n)
		return (-1);
	while (buf[i] && i < n)
	{
		if (buf[i++] == c)
			res++;
	}
	return ((int)res);
}

size_t	read_heredoc(int fd, t_lex *l)
{
	int		sz;
	int		bytes;
	char	buf[MAX_HDOCSZ];

	bytes = 1;
	sz = 0;
	ft_memset(buf, 0, MAX_HDOCSZ);
	while (bytes > 0 && sz < MAX_HDOCSZ)
	{
		bytes = (int)read(fd, buf, MAX_HDOCSZ - sz);
		if (bytes < 0)
			perror("Pipe read");
		sz += bytes;
	}
	if (sz > LEX_BUFSZ)
		sz = LEX_BUFSZ;
	l->lines = _getncount(buf, '\n', sz);
	if (0 != buf[0] && sz > 0)
		ft_memmove(l->buf, buf, sz);
	return (sz);
}

size_t	write_heredoc(int fd, t_lex *l)
{
	int	len;

	len = ft_strlen(l->buf);
	return (write(fd, l->buf, len + 1));
}

/* Uses readline() and handles Ctrl+D
 * Entire heredoc content loads into one token.
 * Any trailing words are skipped until an OP
 */
int	tokenize_heredoc(t_state *s, t_lex *lexer)
{
	t_tok	*token;
	int		res;

	dprint(DBGMSG_ANNOUNCE, _MOD_);
	res = get_eof_word(get_mem(s), lexer);
	if (0 != res)
		return (res);
	res = exec_heredoc(s, lexer);
	if (0 != res)
		return (res);
	(get_mem(s))->dealloc(&get_mem(s)->list, lexer->eof_word);
	lexer->eof_word = NULL;
	dprint("Received doc:%s lines:%d\n", lexer->buf, lexer->lines);
	dprint(DBGMSG_PTRAT, _MOD_, *lexer->ptr);
	token = lex_create_token(get_mem(s), lexer, TOK_HEREDOC_WORD);
	if (token)
	{
		if (0 != add_token(get_mem(s), lexer, token))
			return (ERR_GENERAL);
		set_got_heredoc(s);
	}
	else
		return (err(EMSG_MEM), ERR_MEM);
	return (0);
}
