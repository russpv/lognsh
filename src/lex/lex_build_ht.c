/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_build_ht.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:50:12 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/06 20:19:37 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lex_int.h"
/*
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
		if (!ht_install(m, ht, (char *)cmd[i], lex_create_ht_node(m, false,
					TOK_BI), NULL))
			return (1);
	return (0);
}*/

static inline int	_insert_builtins(t_mem_mgr *m, t_ht ht)
{
	int					i;
	const char			*cmd[] = { 
		BI_ECHO,
		BI_CD,
		BI_PWD,
		BI_EXPORT,
		BI_UNSET,
		BI_ENV,
		BI_EXIT,
		BI_EXEC
	};
	t_ht_install_args	args;

	i = -1;
	args.m = m;
	args.ht = ht;
	while (++i < BI_COUNT)
	{
		args.name = (char *)cmd[i];
		args.data = lex_create_ht_node(m, false, TOK_BI);
		args.cpy_data = NULL;
		if (!ht_install(args))
			return (1);
	}
	return (0);
}

/* Currently not used. */
/*
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
}*/
/*
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
	ht_install(m, ht, OP_DGREAT, lex_create_ht_node(m, false,
			TOK_REDIRECT_APPEND), NULL);
	ht_install(m, ht, OP_DLESS, lex_create_ht_node(m, false, TOK_HEREDOC),
		NULL);
	ht_install(m, ht, "\'", lex_create_ht_node(m, false, TOK_SINGLE_QUOTE),
		NULL);
	ht_install(m, ht, "\"", lex_create_ht_node(m, false, TOK_DOUBLE_QUOTE),
		NULL);
}*/

/*
static inline void	_insert_operators(t_mem_mgr *m, t_ht ht)
{
	t_ht_install_args	args;

	args.m = m;
	args.ht = ht;
	args.cpy_data = NULL;
	args.name = "("; 
	args.data = lex_create_ht_node(m, false, TOK_OPEN_PAREN);
	ht_install(args);
	args.name = ")"; 
	args.data = lex_create_ht_node(m, false, TOK_CLOSE_PAREN); 
	ht_install(args);
	args.name = OP_ANDIF; 
	args.data = lex_create_ht_node(m, false, TOK_ANDIF); 
	ht_install(args);
	args.name = "&"; 
	args.data = lex_create_ht_node(m, true, TOK_OP_REF); 
	ht_install(args);
	args.name = "|"; 
	args.data = lex_create_ht_node(m, true, TOK_PIPE); 
	ht_install(args);
	args.name = OP_ORIF; 
	args.data = lex_create_ht_node(m, false, TOK_ORIF); 
	ht_install(args);
	args.name = ">"; 
	args.data = lex_create_ht_node(m, true, TOK_REDIRECT_OUT); 
	ht_install(args);
	args.name = "<"; 
	args.data = lex_create_ht_node(m, true, TOK_REDIRECT_IN); 
	ht_install(args);
	args.name = OP_DGREAT; 
	args.data = lex_create_ht_node(m, false, TOK_REDIRECT_APPEND); 
	ht_install(args);
	args.name = OP_DLESS; 
	args.data = lex_create_ht_node(m, false, TOK_HEREDOC); 
	ht_install(args);
	args.name = "\'"; 
	args.data = lex_create_ht_node(m, false, TOK_SINGLE_QUOTE); 
	ht_install(args);
	args.name = "\""; 
	args.data = lex_create_ht_node(m, false, TOK_DOUBLE_QUOTE); 
	ht_install(args);
}*/
static inline void	init_install_args(t_ht_install_args *args, \
				t_mem_mgr *m, t_ht ht)
{
	args->m = m;
	args->ht = ht;
	args->cpy_data = NULL;
}

static inline void	_insert_operators(t_mem_mgr *m, t_ht ht)
{
	static struct s_op_data	ops[] = {
	{"(", TOK_OPEN_PAREN, 0},
	{")", TOK_CLOSE_PAREN, 0},
	{OP_ANDIF, TOK_ANDIF, 0},
	{"&", TOK_OP_REF, 1},
	{"|", TOK_PIPE, 1},
	{OP_ORIF, TOK_ORIF, 0},
	{">", TOK_REDIRECT_OUT, 1},
	{"<", TOK_REDIRECT_IN, 1},
	{OP_DGREAT, TOK_REDIRECT_APPEND, 0},
	{OP_DLESS, TOK_HEREDOC, 0},
	{"\'", TOK_SINGLE_QUOTE, 0},
	{"\"", TOK_DOUBLE_QUOTE, 0}
	};
	t_ht_install_args		args;
	int						i;

	i = -1;
	init_install_args(&args, m, ht);
	while (++i < 12)
	{
		args.name = ops[i].name;
		args.data = lex_create_ht_node(m, ops[i].is_ref, ops[i].type);
		ht_install(args);
	}
}

/* Creates reference t_tok records, and t_ht_data records
 * Once ht is built no records are added
 * ht_install takes a copy constructor, but data isn't malloc'd
 */

void	build_hasht(t_mem_mgr *m, t_lex *lexer)
{
	const t_ht			ht = lexer->hasht;
	t_ht_install_args	args;

	assert(ht != NULL);
	args.m = m;
	args.ht = ht;
	args.cpy_data = NULL;
	args.name = "\0";
	args.data = lex_create_ht_node(m, false, TOK_EOF);
	ht_install(args);
	args.name = "$?";
	args.data = lex_create_ht_node(m, false, TOK_EXIT_STATUS);
	ht_install(args);
	args.name = "'\n'";
	args.data = lex_create_ht_node(m, false, TOK_NEWLINE);
	ht_install(args);
	_insert_operators(m, ht);
	_insert_builtins(m, ht);
}
