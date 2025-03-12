#include "lex_int.h"

static inline int	_insert_builtins(t_mem_mgr *m, t_ht ht)
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
		if (!ht_install(m, ht, (char *)cmd[i], lex_create_ht_node(m, false, TOK_BI),
				NULL))
			return (1);
	return (0);
}

/* Currently not used. */
static inline void	_insert_reserved_words(t_mem_mgr *m, t_ht ht)
{
	ht_install(m, ht, WD_IF, lex_create_ht_node(m, false, TOK_IF), NULL);
	ht_install(m, ht, WD_THEN, lex_create_ht_node(m, false, TOK_THEN), NULL);
	ht_install(m, ht, WD_ELSE, lex_create_ht_node(m, false, TOK_ELSE), NULL);
	ht_install(m, ht, WD_FI, lex_create_ht_node(m, false, TOK_FI), NULL);
	ht_install(m, ht, WD_DO, lex_create_ht_node(m, false, TOK_DO), NULL);
	ht_install(m, ht, WD_DONE, lex_create_ht_node(m, false, TOK_DONE), NULL);
	ht_install(m, ht, WD_CASE, lex_create_ht_node(m, false, TOK_CASE), NULL);
	ht_install(m, ht, WD_ESAC, lex_create_ht_node(m, false, TOK_ESAC), NULL);
	ht_install(m, ht, WD_WHIL, lex_create_ht_node(m, false, TOK_WHILE), NULL);
	ht_install(m, ht, WD_UNTL, lex_create_ht_node(m, false, TOK_UNTIL), NULL);
	ht_install(m, ht, WD_FOR, lex_create_ht_node(m, false, TOK_FOR), NULL);
	ht_install(m, ht, WD_IN, lex_create_ht_node(m, false, TOK_IN), NULL);
}

static inline void	_insert_operators(t_mem_mgr *m, t_ht ht)
{
	ht_install(m, ht, "(", lex_create_ht_node(m, false, TOK_OPEN_PAREN), NULL);
	ht_install(m, ht, ")", lex_create_ht_node(m, false, TOK_CLOSE_PAREN), NULL);
	ht_install(m, ht, OP_ANDIF, lex_create_ht_node(m, false, TOK_ANDIF), NULL);
	ht_install(m, ht, "&", lex_create_ht_node(m, true, TOK_OP_REF), NULL);
	ht_install(m, ht, "|", lex_create_ht_node(m, true, TOK_PIPE), NULL);
	ht_install(m, ht, OP_ORIF, lex_create_ht_node(m, false, TOK_ORIF), NULL);
	ht_install(m, ht, ">", lex_create_ht_node(m, true, TOK_REDIRECT_OUT), NULL);
	ht_install(m, ht, "<", lex_create_ht_node(m, true, TOK_REDIRECT_IN), NULL);
	ht_install(m, ht, OP_DGREAT, lex_create_ht_node(m, false, TOK_REDIRECT_APPEND),
		NULL);
	ht_install(m, ht, OP_DLESS, lex_create_ht_node(m, false, TOK_HEREDOC), NULL);
	ht_install(m, ht, "\'", lex_create_ht_node(m, false, TOK_SINGLE_QUOTE), NULL);
	ht_install(m, ht, "\"", lex_create_ht_node(m, false, TOK_DOUBLE_QUOTE), NULL);
}

/* Creates reference t_tok records, and t_ht_data records
 * Once ht is built no records are added
 * ht_install takes a copy constructor, but data isn't malloc'd
 */
void	build_hasht(t_mem_mgr *m, t_lex *lexer)
{
	const t_ht	ht = lexer->hasht;

	assert(NULL != ht);
	ht_install(m, ht, "\0", lex_create_ht_node(m, false, TOK_EOF), NULL);
	ht_install(m, ht, "$?", lex_create_ht_node(m, false, TOK_EXIT_STATUS), NULL);
	ht_install(m, ht, "'\n'", lex_create_ht_node(m, false, TOK_NEWLINE), NULL);
	_insert_operators(m, ht);
	_insert_builtins(m, ht);
}
