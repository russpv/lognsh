#include "lex_int.h"

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
