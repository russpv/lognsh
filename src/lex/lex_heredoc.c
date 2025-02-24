#include "lex_int.h"

/* Captures eof token.
 * "If any part of word is quoted, the delimiter is the result 
 * of quote removal on word, and the lines in the here-document
 * are not expanded."
 */
static void	_lex_eof(t_lex *l)
{
	size_t	buf_idx;

	buf_idx = 0;
	while (*l->ptr && *l->ptr != '\n' && !ft_isspace(*l->ptr))
	{
		if (*l->ptr == '\'' || *l->ptr == '\"')
		{
			l->do_heredoc_expansion = false;
			l->escape_mode ^= true;
			*l->ptr++;
		}
		if (*l->ptr == '\\' && false == l->escape_mode)
			*l->ptr++;
		l->buf[buf_idx++] = *l->ptr++;
	}
}

/* Stores EOF word in lexer with lexer buf and ptr 
 * Spaces and newline delimit the EOF word
 * Handles double or single quote behavior toggles,
 * but ignores missing closing marks.
 */
static inline int	_get_eof_word(t_lex *l)
{
	if (!l->ptr || true == is_too_long(l->ptr))
		return (ERR_GENERAL);
	while (ft_isspace(*l->ptr))
		l->ptr++;
	_lex_eof(l);
	if (0 == ft_strlen(l->buf))
		return (err("ERR heredoc EOF not found.\n"), 0);
	l->eof_word = ft_strdup(l->buf);
	if (!l->eof_word)
	{
		ft_memset(l->buf, 0, LEX_BUFSZ);
		return (ERR_GENERAL);
	}
	debug_print(MOD": Captured Heredoc delimiter: %s\n", l->eof_word);
	ft_memset(l->buf, 0, LEX_BUFSZ);
	return (0);
}

/* Puts readline output onto buf */
static inline int	_load_line(t_lex *l, const char *line)
{
	size_t	buf_idx;

	buf_idx = 0;
	if (true == is_too_long(line))
		return (1);
	while (*line)
		l->buf[buf_idx++] = *line++;
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

/* Creates token up until EOF or exits if NULL (Ctrl+D)
 * Runs readline, loads buf until line return equals EOF
 * Note: Bash/Zsh do not trim the line when testing EOF (" EOF" != EOF)
 * only when capturing the EOF value
 */
static inline t_tok	*_match_heredoc(t_lex *l)
{
	char	*line;
	t_tok	*token;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (NULL == line)
		{
			debug_print(MOD": Input interrupted.\n");
			free(l->eof_word);
			l->eof_word = NULL;
			return (NULL);
		}
		if (true == _line_is_eof(l, line))
		{
			debug_print(MOD": Heredoc delimiter found,\
				exiting heredoc mode.\n");
			token = lex_create_token(l, TOK_HEREDOC_WORD);
			free(l->eof_word);
			l->eof_word = NULL;
			return (token);
		}
		_load_line(l, line);
		free(line);
	}
}

/* Uses readline() and handles Ctrl+D
 * Entire content loads into one token
 *
 * Note: Forces state transition to DONE
 * (probably not req'd).
 */
int	tokenize_heredoc(t_lex *lexer)
{
	t_tok	*token;

	debug_print(MOD": << tokenize_heredoc:\n");
	if (ERR_GENERAL == _get_eof_word(lexer))
		return (ERR_GENERAL);
	token = _match_heredoc(lexer);
	debug_print("Lexer: ptr at:_%c_\n", *lexer->ptr);
	if (token)
	{
		if (0 != add_token(lexer, token))
			return (1);
	}
	else
	{
		debug_print(MOD": ##### Error heredoc\n");
		return (1);
	}
	return (0);
}
