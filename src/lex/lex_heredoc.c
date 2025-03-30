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

	while (1)
	{
		line = readline(HDOC_PROMPT);
		if (NULL == line)
		{
			debug_print(LOGMSG_SIG);
			m->dealloc(&m->list, l->eof_word);
			l->eof_word = NULL;
			return (ERR_CMD_SIGINTD);
		}
		if (true == _line_is_eof(l, line))
		{
			debug_print(DBGMSG_FOUNDDEL);
			m->dealloc(&m->list, l->eof_word);
			l->eof_word = NULL;
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
	debug_print("Received doc:%s\n", lexer->buf);
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
