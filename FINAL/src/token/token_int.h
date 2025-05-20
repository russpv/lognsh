#ifndef TOKEN_INT_H
# define TOKEN_INT_H

# include "token.h"

# define _MOD_ "Token"
# define GRP_TOK_RAW "(GROUP)"

/* For combiner prints */
# define SAYGOT 0
# define SAYCANCOMB 1
# define SAYDONE 2
# define SAYNULL 3
# define SAYCOMBINE 4
# define SAYTRIMMED 5
# define SAYCACHEONLY 6
# define SAYCANNOTCOMB 7

/* For combiner */
# define COMBINED 0
# define NOTCOMBINED 1

# define UNQUOTED 1
# define QUOTED 0
# define LTRL 2
# define INITVAL -1

# define FRONT 0
# define BACK 1

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

/* Combiner */
char					*do_trim(t_state *s, char *str, char *set, int side);
char					*do_join(t_state *s, char *left, char *right);
int						put_str_on_toklst(t_state *s, char **str);
bool					is_joinable(t_state *s, char *str);
bool					has_lagging_delims(char const *s, char const *set);
bool					has_leading_delims(char const *s, char const *set);
void					announce(const char *cllr, char *raw, char *str,
							int msg);

#endif
