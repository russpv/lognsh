#include "lex_int.h"

/* Doesn't free raw_string */
void	destroy_lexer(t_mem_mgr *m, void **instance)
{
	t_lex	*lexer;

	lexer = (t_lex *)(*instance);
	dprint(_MOD_ ": %s...\n", __FUNCTION__);
	if (!lexer)
		return ;
	if (lexer->buf)
		m->dealloc(&m->list, lexer->buf);
	if (lexer->eof_word)
		m->dealloc(&m->list, lexer->eof_word);
	if (lexer->hasht)
		ht_destroy(m, lexer->hasht, lex_destroy_ht_node);
	if (lexer->token_list)
		ft_lstclear_tmp(m, &lexer->token_list, destroy_token);
	m->dealloc(&m->list, lexer);
	*instance = NULL;
	dprint(_MOD_ ": %s...DONE\n", __FUNCTION__);
}

t_lex_fns	*init_lex_fns(t_state *s)
{
	const t_mem_mgr	*m = get_mem(s);
	t_lex_fns		*fns;

	fns = m->f(&((t_mem_mgr *)m)->list, sizeof(t_lex_fns));
	if (!fns)
		exit_clean(&((t_mem_mgr *)m)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	fns->lex_get_eof = lex_get_eof;
	fns->lex_get_lines = lex_get_lines;
	fns->lex_match_heredoc = match_heredoc;
	fns->write_heredoc = write_heredoc;
	fns->read_heredoc = read_heredoc;
	return (fns);
}

static inline void	_init_lexer_state(t_state *state, t_lex *lexer,
		int start_state)
{
	lexer->global_state = state;
	if (NORMAL == start_state)
		lexer->tokenizer = tokenize_normal;
	if (IN_SINGLE_QUOTES == start_state)
		lexer->tokenizer = tokenize_single_quotes;
	if (IN_DOUBLE_QUOTES == start_state)
	{
		lexer->tokenizer = tokenize_double_quotes;
		lexer->do_wordsplit = false;
		st_int_push(lexer->stack, IN_DOUBLE_QUOTES);
	}
	lexer->state = start_state;
}

static inline bool	_allocate_buf_and_hasht(t_mem_mgr *m, t_lex *lexer)
{
	lexer->buf = myalloc(&m->list, sizeof(char) * LEX_BUFSZ);
	if (!lexer->buf)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	ft_memset(lexer->buf, 0, LEX_BUFSZ);
	lexer->buf_idx = 0;
	lexer->hasht = ht_create(m);
	if (!lexer->hasht)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	return (lexer->hasht != NULL);
}

static inline void	_init_lexer_flags(t_lex *lexer)
{
	lexer->do_heredoc_expansion = true;
	lexer->escape_mode = false;
	lexer->do_expansion = INITVAL;
	lexer->do_globbing = INITVAL;
	lexer->do_wordsplit = true;
	lexer->input_incomplete = false;
	lexer->is_subtoken = false;
	lexer->is_assignment = false;
	lexer->lex_err = 0;
}

t_lex	*create_lexer(t_state *state, int start_state, const char *s)
{
	t_lex	*lexer;

	dprint(_MOD_ ": %s\n", __FUNCTION__);
	lexer = myalloc(&(get_mem(state)->list), sizeof(t_lex));
	if (lexer)
	{
		_init_lexer_flags(lexer);
		lexer->stack = st_int_create(get_mem(state));
		lexer->raw_string = s;
		lexer->ptr = (char *)s;
		lexer->token_list = NULL;
		lexer->eof_word = NULL;
		lexer->lines = 0;
		lexer->last_grp_tok = NULL;
		lexer->tokc = 0;
		lexer->keep_dollar = LEXKEEPDOLLR;
		register_lexer_destroy(state, destroy_lexer);
		register_token_destroy(state, destroy_token);
		if (false == _allocate_buf_and_hasht(get_mem(state), lexer))
			exit_clean(&get_mem(state)->list, ENOMEM, __FUNCTION__,
				EMSG_MALLOC);
		build_hasht(get_mem(state), lexer);
		_init_lexer_state(state, lexer, start_state);
	}
	return (lexer);
}


/* -------------------------------------------------------------------------
 * Function Group: Lexer hashtable
 * Description   : Handles creation and adding to ht
 * ------------------------------------------------------------------------- */

/* Returns a new data node */
t_ht_data	lex_create_ht_node(t_mem_mgr *m, bool is_substring, int type)
{
	t_ht_data	data;

	data = myalloc(&m->list, sizeof(struct s_ht_data));
	if (data)
	{
		data->is_substring = is_substring;
		data->type = type;
	}
	else
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	return (data);
}

/* Destroys t_ht_data data, defined in this module */
void	lex_destroy_ht_node(t_mem_node *n, void *node)
{
	myfree(n, node);
}

/* for ht_install this will return a new copy of the argument
 * But we don't need to use this
 */
void	*lex_copy_ht_data(t_mem_node *head, void *data)
{
	t_ht_data	newdata;

	newdata = myalloc(head, sizeof(struct s_ht_data));
	if (newdata)
	{
		newdata->is_substring = ((t_ht_data)data)->is_substring;
		newdata->type = ((t_ht_data)data)->type;
	}
	else
		exit_clean(head, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	return (newdata);
}

static inline int	_insert_builtins(t_mem_mgr *m, t_ht_utils *u)
{
	int			i;
	const char	*cmd[] = {
		BI_ECHO,
		BI_CD,
		BI_PWD,
		BI_EXPORT,
		BI_UNSET,
		BI_ENV,
		BI_EXIT,
		BI_EXEC,
	};

	i = -1;
	while (++i < BI_COUNT)
		if (!ht_install(m, u, (char *)cmd[i], lex_create_ht_node(m, false,
					TOK_BI)))
			return (1);
	return (0);
}

static inline void	_insert_operators(t_mem_mgr *m, t_ht_utils *u)
{
	ht_install(m, u, "(", lex_create_ht_node(m, false, TOK_OPEN_PAREN));
	ht_install(m, u, ")", lex_create_ht_node(m, false, TOK_CLOSE_PAREN));
	ht_install(m, u, OP_ANDIF, lex_create_ht_node(m, false, TOK_ANDIF));
	ht_install(m, u, "&", lex_create_ht_node(m, true, TOK_OP_REF));
	ht_install(m, u, "|", lex_create_ht_node(m, true, TOK_PIPE));
	ht_install(m, u, OP_ORIF, lex_create_ht_node(m, false, TOK_ORIF));
	ht_install(m, u, ">", lex_create_ht_node(m, true, TOK_REDIRECT_OUT));
	ht_install(m, u, "<", lex_create_ht_node(m, true, TOK_REDIRECT_IN));
	ht_install(m, u, OP_DGREAT, lex_create_ht_node(m, false,
			TOK_REDIRECT_APPEND));
	ht_install(m, u, OP_DLESS, lex_create_ht_node(m, false, TOK_HEREDOC));
	ht_install(m, u, "\'", lex_create_ht_node(m, false, TOK_SINGLE_QUOTE));
	ht_install(m, u, "\"", lex_create_ht_node(m, false, TOK_DOUBLE_QUOTE));
}

/* Creates reference t_tok records, and t_ht_data records
 * Once ht is built no records are added
 * ht_install takes a copy constructor, but data isn't malloc'd
 */
void	build_hasht(t_mem_mgr *m, t_lex *lexer)
{
	const t_ht	ht = lexer->hasht;
	t_ht_utils	u;

	ht_init_utils(&u, ht, NULL);
	ht_install(m, &u, "\0", lex_create_ht_node(m, false, TOK_EOF));
	ht_install(m, &u, "$?", lex_create_ht_node(m, false, TOK_EXIT_STATUS));
	ht_install(m, &u, "'\n'", lex_create_ht_node(m, false, TOK_NEWLINE));
	_insert_operators(m, &u);
	_insert_builtins(m, &u);
}
