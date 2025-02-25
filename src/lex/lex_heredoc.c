#include "lex_int.h"

#define DBGMSG_ANNOUNCE _MOD_": << tokenize_heredoc:\n"
#define DBGMSG_FOUNDDEL _MOD_": Heredoc delimiter found, exiting heredoc mode.\n"
#define DBGMSG_PTRAT _MOD_": ptr at:_%c_\n"
#define ERRMSG_MEM _MOD_": ##### Error heredoc\n"
#define LOGMSG_SIG _MOD_": "LOGMSG_SIGINT

/* Puts readline line onto buf. Omits '\0' from readline. */
static inline int	_load_line(t_lex *l, const char *line, size_t *buf_idx)
{
	if (true == is_too_long(line))
		return (ERR_BUFFLOW);
	while (*line)
		l->buf[(*buf_idx)++] = *line++;
	l->buf[(*buf_idx)++] = '\n';
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
 */
static inline int	_match_heredoc(t_lex *l)
{
	char	*line;
	size_t	buf_idx;

	buf_idx = 0;
	while (1)
	{
		line = readline("> ");
		if (NULL == line)
		{
			debug_print(LOGMSG_SIG);
			free(l->eof_word);
			l->eof_word = NULL;
			return (ERR_CMD_SIGINTD);
		}
		if (true == _line_is_eof(l, line))
		{
			debug_print(DBGMSG_FOUNDDEL);
			free(l->eof_word);
			l->eof_word = NULL;
			return (0);
		}
		_load_line(l, line, &buf_idx);
		free(line);
	}
}

/* Uses readline() and handles Ctrl+D
 * Entire heredoc content loads into one token.
 * Any trailing words are skipped until an OP 
 */
int	tokenize_heredoc(t_lex *lexer)
{
	t_tok	*token;
	int		res;

	debug_print(DBGMSG_ANNOUNCE);
	res = get_eof_word(lexer);
	if (0 != res)
		return (res);
	lexer->do_expansion = true;
	res = _match_heredoc(lexer);
	if (0 != res)
		return (res);
	debug_print(DBGMSG_PTRAT, *lexer->ptr);
	token = lex_create_token(lexer, TOK_HEREDOC_WORD);
	if (token)
	{
		assert(NULL != lexer && NULL != token);
		if (0 != add_token(lexer, token))
			return (ERR_GENERAL);
	}
	else
		return (err(ERRMSG_MEM), ERR_MEM);
	return (0);
}
