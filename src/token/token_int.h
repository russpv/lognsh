#ifndef TOKEN_INT_H
# define TOKEN_INT_H

# include "token.h"

# define _MOD_ "Token"

enum					e_token_class
{
	NORMAL,
	GROUP
};

typedef struct s_normal_tok
{
	enum e_tok_type		type;
	char				*raw;
	size_t				raw_len;
	size_t				pos;
	bool				do_globbing;
	bool				do_expansion;
	bool				is_subtoken;
	bool				in_dquotes;
	
	bool				is_combinable;
}						t_normal_tok;

typedef struct s_meta_tok
{
	t_list				*tokens;
	bool				do_globbing;
	bool				do_expansion;
	bool				in_dquotes;
	size_t				pos;
	size_t				tokc;
}						t_meta_tok;

union					u_token_data
{
	t_normal_tok		tok;
	t_meta_tok			meta;
};

typedef struct s_tok
{
	enum e_token_class	class;
	union u_token_data	t;
}						t_tok;

extern unsigned int		lex_get_keepd(t_lex *lexer);
extern int				check_special_expansions(t_state *s, const char *buf,
							char **value);

#endif