#include "lex_int.h"

#define DBGMSG_ANNOUNCE _MOD_ ": << tokenize_heredoc:\n"
#define DBGMSG_FOUNDDEL _MOD_ ": Heredoc delimiter found,\
	exiting heredoc mode.\n"
#define DBGMSG_PTRAT _MOD_ ": ptr at:_%c_\n"
#define EMSG_MEM _MOD_ ": ##### Error heredoc\n"
#define LOGMSG_SIG _MOD_ ": " LOGMSG_SIGINT

/* Puts readline line onto buf. Omits '\0' from readline. */
static inline int	_load_line(t_lex *l, const char *line)
{
	if (true == is_too_long(line))
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

static int	_getncount(char buf[], unsigned char c, size_t n)
{
	size_t i;
	int res;

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
	int sz;
	int bytes;
	char buf[MAX_HDOCSZ];

	bytes = 1;
	sz = 0;
	ft_memset(buf, 0, MAX_HDOCSZ);
	while (bytes > 0 && sz < MAX_HDOCSZ)
	{
		bytes = (int)read(fd, buf, MAX_HDOCSZ - sz);
		if (bytes < 0)
			perror("Pipe read");
		sz+=bytes;
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
	int len;

	len = ft_strlen(l->buf);
	return (write(fd, l->buf, len + 1));
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
			debug_print(LOGMSG_SIG);
			free(line);
			return (ERR_RL_ABORTED);
		}
		if (true == _line_is_eof(l, line))
		{
			debug_print(DBGMSG_FOUNDDEL);
			free(line);
			return (0);
		}
		_load_line(l, line);
		free(line);
	}
}

/* Uses readline() and handles Ctrl+D
 * Entire heredoc content loads into one token.
 * Any trailing words are skipped until an OP
 */
int	tokenize_heredoc(t_state *s, t_lex *lexer)
{
	t_tok	*token;
	int		res;

	debug_print(DBGMSG_ANNOUNCE);
	res = get_eof_word(get_mem(s), lexer);
	if (0 != res)
		return (res);
	res = exec_heredoc(get_mem(s), lexer);
	if (0 != res)
		return (res);
	get_mem(s)->dealloc(&get_mem(s)->list, lexer->eof_word);
	lexer->eof_word = NULL;
	debug_print("Received doc:%s lines:%d\n", lexer->buf, lexer->lines);
	debug_print(DBGMSG_PTRAT, *lexer->ptr);
	token = lex_create_token(get_mem(s), lexer, TOK_HEREDOC_WORD);
	if (token)
	{
		assert(NULL != lexer && NULL != token);
		if (0 != add_token(get_mem(s), lexer, token))
			return (ERR_GENERAL);
		set_got_heredoc(s);
	}
	else
		return (err(EMSG_MEM), ERR_MEM);
	return (0);
}
