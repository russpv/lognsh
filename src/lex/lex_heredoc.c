#include "lex_int.h"

#define DBGMSG_ANNOUNCE "%s: << tokenize_heredoc:\n"
#define DBGMSG_PTRAT "%s: ptr at:_%c_\n"
#define EMSG_MEM "%s: ##### Error heredoc\n"


/* -------------------------------------------------------------------------
 * Function Group: Lexer heredoc reader
 * Description   : Tokenizes heredoc body string from readline input
 * ------------------------------------------------------------------------- */

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
			return (free(line), ERR_RL_ABORTED);
		free(line);
	}
}


/* -------------------------------------------------------------------------
 * Function Group: Lexer heredoc EOF word
 * Description   : Parses EOF word and stores in lexer object
 * ------------------------------------------------------------------------- */

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
	l->eof_word = ft_strdup_mem(m, l->buf);
	if (!l->eof_word)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	dprint(_MOD_ ": captured heredoc delimiter: %s\n", l->eof_word);
	ft_memset(l->buf, 0, LEX_BUFSZ);
	l->buf_idx = 0;
	return (0);
}





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
	size_t	sz;
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
	if (sz > sizeof(l->buf))
	{
	    dprint("[WARN] Heredoc input truncated at %d bytes\n", sizeof(l->buf));
		sz = sizeof(l->buf);
	}
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
