#include "lex_int.h"

void	lex_print(t_lex *lexer)
{
	if (NULL == lexer)
		return ;
	if (NULL == lexer->token_list)
		return ;
	ft_lstiter(lexer->token_list, tok_print);
	dprintdiv("\n--- Input Lexing Finished ---\n\n");
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

// Returns next char, if valid
unsigned char	is_valid_next(t_lex *l, int offset)
{
	if (!l || !l->ptr)
		return (0);
	if (offset < 0 || (size_t)(offset + 1) >= ft_strlen(l->ptr) || l->ptr[offset
		+ 1] == '\0')
		return (0);
	return (l->ptr[offset + 1]);
}

// Returns prev char, if valid
unsigned char	is_valid_prev(t_lex *l, int offset)
{
	char	*ptr;

	(void)offset;
	if (!l || !l->ptr)
		return (0);
	ptr = l->ptr;
	ptr--;
	if (NULL == ptr)
		return (0);
	if (0 == *ptr)
		return (0);
	return (*(l->ptr - 1));
}

/* -------------------------------------------------------------------------
 * Function Group: Lexer token actions
 * Description   : Handles creation and adding to token linked list
 * ------------------------------------------------------------------------- */

static int	_add_subtoken(t_mem_mgr *m, t_lex *lexer, t_tok *subtok)
{
	tok_add_subtok(m, lexer->last_grp_tok, subtok);
	tok_incr_tokc(lexer->last_grp_tok);
	return (0);
}

int	add_grptoken(t_mem_mgr *m, t_lex *lexer)
{
	dprint(_MOD_ ": %s adding token (grp) to list\n", __FUNCTION__);
	ft_lstadd_back(&lexer->token_list, ft_lstnew_tmp(m, lexer->last_grp_tok));
	lexer->is_subtoken = false;
	lexer->last_grp_tok = NULL;
	lexer->is_assignment = false;
	lexer->tokc++;
	return (0);
}

static void	_commit_token_or_subtoken(t_mem_mgr *m, t_lex *lexer, t_tok *token)
{
	if (lexer->last_grp_tok)
	{
		dprint(_MOD_ ": %s adding subtoken %s to grp\n", __FUNCTION__,
			tok_get_raw(token));
		_add_subtoken(m, lexer, token);
		dprint(_MOD_ ": %s checking delimiter, ptr:%c\n", __FUNCTION__,
			*lexer->ptr);
		if (NORMAL == lexer->state && is_normal_delim(lexer, 0))
		{
			dprint(_MOD_ ": %s adding grptoken\n", __FUNCTION__);
			add_grptoken(m, lexer);
		}
	}
	else
	{
		dprint(_MOD_ ": %s adding token to list\n", __FUNCTION__);
		if (ft_lstadd_back(&lexer->token_list, ft_lstnew_tmp(m,
					token)) > 0)
			lexer->tokc++;
		lexer->is_assignment = false;
	}
}

/* Adds to llist tail a new token, clears buf */
int	add_token(t_mem_mgr *m, t_lex *lexer, t_tok *token)
{
	dvprint(_MOD_ ": %s\n", __FUNCTION__);
	if (token && lexer)
	{
		if (lexer->lex_err)
			return (ERR_GENERAL);
		ft_memset(lexer->buf, 0, LEX_BUFSZ);
		lexer->buf_idx = 0;
		if (lexer->tokc < LEX_MAX_TOKC)
		{
			_commit_token_or_subtoken(m, lexer, token);
		}
		else
			return (ERR_BUFFLOW);
		return (0);
	}
	return (ERR_ARGS);
}

static void	_init_token(t_lex *lexer, t_tok *token)
{
	if (true == lexer->is_subtoken)
		tok_set_subtoken(token);
	if (false == lexer->do_wordsplit || true == lexer->is_assignment)
		tok_set_dquotes(token);
	if (true == lexer->do_globbing)
	{
		tok_set_globbing(token);
		tok_set_globbing(lexer->last_grp_tok);
	}
	if (true == lexer->do_expansion)
	{
		tok_set_expansion(token);
		tok_set_expansion(lexer->last_grp_tok);
	}
	lexer->do_globbing = INITVAL;
	lexer->do_expansion = INITVAL;
	dvprint(_MOD_ ": %s: Created token \n", __FUNCTION__);
}

/* Creates token based on current buf and ptr, does not add to llist
 * If buf is empty, creates grp token only.
 * Resets buf and buf_idx and tokenizing flags.
 * Looks ahead for normal delim to reset subtoken flag.
 * Note: skip any terminator that is not to be part of token (\")
 */
t_tok	*lex_create_token(t_mem_mgr *m, t_lex *l, int type)
{
	t_tok	*t;
	t_tok	*grp_token;

	dvprint(_MOD_ ": %s\n", __FUNCTION__);
	if (!l)
		return (NULL);
	if (true == l->is_subtoken && NULL == l->last_grp_tok)
	{
		grp_token = create_token(m, l->buf, TOK_GROUP_WORD,
				(size_t)(l->ptr - l->raw_string));
		if (!grp_token)
			exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		l->last_grp_tok = grp_token;
		dprint(_MOD_ ": %s: Created GROUP token\n", __FUNCTION__);
	}
	if (0 == ft_strlen(l->buf))
		(void)0;
	t = create_token(m, l->buf, type, (size_t)(l->ptr - l->raw_string));
	if (t)
		_init_token(l, t);
	return (t);
}
