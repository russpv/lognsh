#include "lex_int.h"

static inline void	_init_lexer_state(t_lex *lexer, int start_state)
{
	if (NORMAL == start_state)
		lexer->tokenizer = tokenize_normal;
	if (IN_SINGLE_QUOTES == start_state)
		lexer->tokenizer = tokenize_single_quotes;
	if (IN_DOUBLE_QUOTES == start_state)
		lexer->tokenizer = tokenize_double_quotes;
	lexer->state = start_state;
}

static inline bool	_allocate_buf_and_hasht(t_lex *lexer)
{
	lexer->buf = malloc(sizeof(char) * LEX_BUFSZ);
	if (!lexer->buf)
		return (false);
	ft_memset(lexer->buf, 0, LEX_BUFSZ);
	lexer->buf_idx = 0;
	lexer->hasht = ht_create();
	if (!lexer->hasht)
		free(lexer->buf);
	return (lexer->hasht != NULL);
}

void	init_token_masks(t_lex *l)
{
	l->glob_mask = malloc(sizeof(l->buf));
	if (NULL == l->glob_mask)
		err("ERR DO SOMETHING ABOUT MEM!!!");
	ft_memset(l->glob_mask, 0, sizeof(l->buf));
	l->expd_mask = malloc(sizeof(l->buf));
	if (NULL == l->expd_mask)
		err("ERR DO SOMETHING ABOUT MEM!!!");
	ft_memset(l->expd_mask, 0, sizeof(l->buf));
}

t_lex	*create_lexer(t_state *st, int start_state, const char *s)
{
	t_lex	*lexer;

	debug_print(_MOD_": %s\n", __FUNCTION__);
	lexer = malloc(sizeof(t_lex));
	if (lexer)
	{
		_init_lexer_state(lexer, start_state);
		lexer->raw_string = s;
		lexer->ptr = (char *)s;
		lexer->escape_mode = false;
		lexer->do_expansion = INITVAL;
		lexer->do_globbing = INITVAL;
		lexer->token_list = NULL;
		lexer->input_incomplete = false;
		lexer->is_subtoken = false;
		lexer->do_heredoc_expansion = true;
		lexer->eof_word = NULL;
		lexer->expd_mask = NULL;
		lexer->glob_mask = NULL;
		lexer->last_grp_tok = NULL;
		lexer->tokc = 0;
		lexer->keep_dollar = LEXERKEEP$;
		register_lexer_destroy(st, destroy_lexer);
		if (false == _allocate_buf_and_hasht(lexer))
		{
			free(lexer);
			return (NULL);
		}
		build_hasht(lexer);
	}
	return (lexer);
}

/* Doesn't free raw_string */
void	destroy_lexer(t_state *s, void *instance)
{
	t_lex	*lexer;

	lexer = (t_lex *)instance;
	debug_print(_MOD_": destroy_lexer...\n");
	if (!lexer)
		return ;
	if (lexer->buf)
	{
		free(lexer->buf);
	}
	if (lexer->eof_word)
	{
		free(lexer->eof_word);
	}
	if (lexer->hasht)
	{
		ht_destroy(lexer->hasht, lex_destroy_ht_node);
	}
	if (lexer->token_list)
	{
		ft_lstclear_tmp(s, &lexer->token_list, destroy_token);
	}
	free(lexer);
}

// assumes non-empty string
t_lex	*tokenize(t_state *s, const char *input)
{
	enum e_lex_state	state;
	t_lex				*lexer;

	if ((unsigned char)OP_SQUOTE == *input)
		state = IN_SINGLE_QUOTES;
	else if ((unsigned char)OP_DQUOTE == *input)
		state = IN_DOUBLE_QUOTES;
	else
		state = NORMAL;
	lexer = create_lexer(s, state, input);
	if (lexer)
	{
		while (DONE != lexer->state)
		{
			if (0 != lexer->tokenizer(s, lexer))
			{
				destroy_lexer(s, lexer);
				return (debug_print(_MOD_": tokenizer ERR\n"), NULL);
			}
			do_state_transition(lexer);
		}
	}
	debug_detect_cycle(lexer->token_list);
	lex_print(lexer);
	return (lexer);
}
