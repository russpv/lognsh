#include "lex_int.h"

t_lex	*create_lexer(int start_state, const char *s)
{
	debug_print( "create_lexer\n");
	t_lex *lexer = malloc(sizeof(t_lex));
	if (lexer)
	{
		lexer->state = start_state;
		if (NORMAL == start_state)
			lexer->tokenizer = tokenize_normal;
		if (IN_SINGLE_QUOTES == start_state)
			lexer->tokenizer = tokenize_single_quotes;
		if (IN_DOUBLE_QUOTES == start_state)
			lexer->tokenizer = tokenize_double_quotes;
		lexer->raw_string = s;
		lexer->ptr = (char *)s;
		lexer->escape_mode = false;
		lexer->do_expansion = RESET;
		lexer->do_globbing = RESET;
		lexer->token_list = NULL;
		lexer->buf = malloc(sizeof(char) * LEX_BUFSZ);
		ft_memset(lexer->buf, 0 , LEX_BUFSZ);
		if (!lexer->buf)
		{
			free(lexer);
			return (NULL);
		}
		lexer->hasht = ht_create();
		if (!lexer->hasht)
		{
			free(lexer);
			free(lexer->buf);
			return (NULL);
		}
		build_hasht(lexer);
	}
	return (lexer);
}

void	destroy_lexer(t_lex *lexer)
{
	if (!lexer)
		return ;
	if (lexer->buf)
		free(lexer->buf);
	if (lexer->hasht)
		ht_destroy(lexer->hasht, lex_destroy_ht_node);
	if (lexer->token_list)
		ft_lstclear(&lexer->token_list, destroy_token);
	free(lexer);
}

/* Changes state based on current ptr position */
static int16_t	_do_state_transition(t_lex *lexer)
{
	debug_print( "_do_state_transition\n");
	if (!lexer->ptr)
		return (0);
	if (ON_EOF == lexer->state)
		return (lexer->state = DONE);
	if (OP_DQUOTE == *lexer->ptr)
	{
		lexer->state = IN_DOUBLE_QUOTES;
		lexer->tokenizer = tokenize_double_quotes;
	}
	else if ((unsigned char)*lexer->ptr == OP_SQUOTE)
	{
		lexer->state = IN_SINGLE_QUOTES;
		lexer->tokenizer = tokenize_single_quotes;
	}
	else if ((unsigned char)*lexer->ptr == OP_NULL)
	{
		debug_print("FOUND NULL TOKEN\n");
		lexer->state = ON_EOF;
		lexer->tokenizer = tokenize_null;
	}
	else
	{
		lexer->state = NORMAL;
		lexer->tokenizer = tokenize_normal;
	}
	return (0);
}

// assumes non-empty string
t_lex	*tokenize(const char *input)
{
	debug_print( "tokenize\n");
	e_lex_state state;

	if ((unsigned char)OP_SQUOTE == *input)
		state = IN_SINGLE_QUOTES;
	else if ((unsigned char)OP_DQUOTE == *input)
		state = IN_DOUBLE_QUOTES;
	else
		state = NORMAL;
	t_lex *lexer = create_lexer(state, input);
	if (lexer)
	{
		while (DONE != lexer->state)
		{
			if (1 == lexer->tokenizer(lexer))
			{
				debug_print( "tokenizer ERR\n");
				destroy_lexer(lexer);
				return (NULL);
			}
			_do_state_transition(lexer);
		}
	}
	debug_detect_cycle(lexer->token_list);
	return (lexer);
}
/* Creates reference t_tok records, and t_ht_data records 
 * Once ht is built no records are added 
 * ht_install takes a copy constructor, but data isn't malloc'd
 */
void	build_hasht(t_lex *lexer)
{
	const t_ht ht = lexer->hasht;

	ht_install(ht, "\0", lex_create_ht_node(false, TOK_EOF), lex_copy_ht_data);
    //ht_install(ht, "$", lex_create_ht_node(true, TOK_ENV_VAR), lex_copy_ht_data);
    ht_install(ht, "$?", lex_create_ht_node(false, TOK_EXIT_STATUS), lex_copy_ht_data);
    ht_install(ht, "'\n'", lex_create_ht_node(false, TOK_NEWLINE), lex_copy_ht_data);

    // Insert operator definitions
    ht_install(ht, OP_ANDIF, lex_create_ht_node(false, TOK_ANDIF), lex_copy_ht_data);
    ht_install(ht, "&", lex_create_ht_node(true, TOK_OP_REF), lex_copy_ht_data);
    ht_install(ht, "|", lex_create_ht_node(true, TOK_PIPE), lex_copy_ht_data);
    ht_install(ht, OP_ORIF, lex_create_ht_node(false, TOK_ORIF), lex_copy_ht_data);
    ht_install(ht, ">", lex_create_ht_node(true, TOK_REDIRECT_OUT), lex_copy_ht_data);
    ht_install(ht, "<", lex_create_ht_node(true, TOK_REDIRECT_IN), lex_copy_ht_data);
    ht_install(ht, OP_DGREAT, lex_create_ht_node(false, TOK_REDIRECT_APPEND), lex_copy_ht_data);
    ht_install(ht, OP_DLESS, lex_create_ht_node(false, TOK_HEREDOC), lex_copy_ht_data);
    ht_install(ht, "\'", lex_create_ht_node(false, TOK_SINGLE_QUOTE), lex_copy_ht_data);
    ht_install(ht, "\"", lex_create_ht_node(false, TOK_DOUBLE_QUOTE), lex_copy_ht_data);

    // Insert reserved words
    ht_install(ht, WD_IF, lex_create_ht_node(false, TOK_IF), lex_copy_ht_data);
    ht_install(ht, WD_THEN, lex_create_ht_node(false, TOK_THEN), lex_copy_ht_data);
    ht_install(ht, WD_ELSE, lex_create_ht_node(false, TOK_ELSE), lex_copy_ht_data);
    ht_install(ht, WD_FI, lex_create_ht_node(false, TOK_FI), lex_copy_ht_data);
    ht_install(ht, WD_DO, lex_create_ht_node(false, TOK_DO), lex_copy_ht_data);
    ht_install(ht, WD_DONE, lex_create_ht_node(false, TOK_DONE), lex_copy_ht_data);
    ht_install(ht, WD_CASE, lex_create_ht_node(false, TOK_CASE), lex_copy_ht_data);
    ht_install(ht, WD_ESAC, lex_create_ht_node(false, TOK_ESAC), lex_copy_ht_data);
    ht_install(ht, WD_WHIL, lex_create_ht_node(false, TOK_WHILE), lex_copy_ht_data);
    ht_install(ht, WD_UNTL, lex_create_ht_node(false, TOK_UNTIL), lex_copy_ht_data);
    ht_install(ht, WD_FOR, lex_create_ht_node(false, TOK_FOR), lex_copy_ht_data);
    ht_install(ht, WD_IN, lex_create_ht_node(false, TOK_IN), lex_copy_ht_data);

}
