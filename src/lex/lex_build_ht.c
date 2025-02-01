#include "lex_int.h"

static inline int	_insert_builtins(t_ht ht)
{
	const char *cmd[] = {
		BI_ECHO,
		BI_CD,
		BI_PWD,
		BI_EXPORT,
		BI_UNSET,
		BI_ENV, 
		BI_EXIT,
		BI_EXEC,
	};
	int i;

	i = -1;
	while (++i < BI_COUNT)
		if (!ht_install(ht, (char *)cmd[i], lex_create_ht_node(false, TOK_BI), lex_copy_ht_data))
			return (1);
	return (0);
}

static inline void	_insert_reserved_words(t_ht ht)
{
	ht_install(ht, WD_IF, lex_create_ht_node(false, TOK_IF), lex_copy_ht_data);
	ht_install(ht, WD_THEN, lex_create_ht_node(false, TOK_THEN),
		lex_copy_ht_data);
	ht_install(ht, WD_ELSE, lex_create_ht_node(false, TOK_ELSE),
		lex_copy_ht_data);
	ht_install(ht, WD_FI, lex_create_ht_node(false, TOK_FI), lex_copy_ht_data);
	ht_install(ht, WD_DO, lex_create_ht_node(false, TOK_DO), lex_copy_ht_data);
	ht_install(ht, WD_DONE, lex_create_ht_node(false, TOK_DONE),
		lex_copy_ht_data);
	ht_install(ht, WD_CASE, lex_create_ht_node(false, TOK_CASE),
		lex_copy_ht_data);
	ht_install(ht, WD_ESAC, lex_create_ht_node(false, TOK_ESAC),
		lex_copy_ht_data);
	ht_install(ht, WD_WHIL, lex_create_ht_node(false, TOK_WHILE),
		lex_copy_ht_data);
	ht_install(ht, WD_UNTL, lex_create_ht_node(false, TOK_UNTIL),
		lex_copy_ht_data);
	ht_install(ht, WD_FOR, lex_create_ht_node(false, TOK_FOR),
		lex_copy_ht_data);
	ht_install(ht, WD_IN, lex_create_ht_node(false, TOK_IN), lex_copy_ht_data);
}

static inline void	_insert_operators(t_ht ht)
{
	ht_install(ht, "(", lex_create_ht_node(false, TOK_OPEN_PAREN),
		lex_copy_ht_data);
	ht_install(ht, ")", lex_create_ht_node(false, TOK_CLOSE_PAREN),
		lex_copy_ht_data);
	ht_install(ht, OP_ANDIF, lex_create_ht_node(false, TOK_ANDIF),
		lex_copy_ht_data);
	ht_install(ht, "&", lex_create_ht_node(true, TOK_OP_REF), lex_copy_ht_data);
	ht_install(ht, "|", lex_create_ht_node(true, TOK_PIPE), lex_copy_ht_data);
	ht_install(ht, OP_ORIF, lex_create_ht_node(false, TOK_ORIF),
		lex_copy_ht_data);
	ht_install(ht, ">", lex_create_ht_node(true, TOK_REDIRECT_OUT),
		lex_copy_ht_data);
	ht_install(ht, "<", lex_create_ht_node(true, TOK_REDIRECT_IN),
		lex_copy_ht_data);
	ht_install(ht, OP_DGREAT, lex_create_ht_node(false, TOK_REDIRECT_APPEND),
		lex_copy_ht_data);
	ht_install(ht, OP_DLESS, lex_create_ht_node(false, TOK_HEREDOC),
		lex_copy_ht_data);
	ht_install(ht, "\'", lex_create_ht_node(false, TOK_SINGLE_QUOTE),
		lex_copy_ht_data);
	ht_install(ht, "\"", lex_create_ht_node(false, TOK_DOUBLE_QUOTE),
		lex_copy_ht_data);
}

/* Creates reference t_tok records, and t_ht_data records
 * Once ht is built no records are added
 * ht_install takes a copy constructor, but data isn't malloc'd
 */
void	build_hasht(t_lex *lexer)
{
	const t_ht	ht = lexer->hasht;

	ht_install(ht, "\0", lex_create_ht_node(false, TOK_EOF), lex_copy_ht_data);
	ht_install(ht, "$?", lex_create_ht_node(false, TOK_EXIT_STATUS), \
		lex_copy_ht_data);
	ht_install(ht, "'\n'", lex_create_ht_node(false, TOK_NEWLINE), \
		lex_copy_ht_data);
	_insert_operators(ht);
	_insert_reserved_words(ht);
	_insert_builtins(ht);
}
