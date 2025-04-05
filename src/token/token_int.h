/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_int.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:17:43 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 09:17:56 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// token_utils_combine.c: core combine logic
bool					_is_joinable(t_state *s, char *str);
int						tok_do_grp_combine(t_state *s, void *c);

// token_utils_combine_utils_1: trimming/joining helpers
char					*_do_join(t_state *s, char *left, char *right);
char					*_do_trim(t_state *s, char *str, char *set, int side);
bool					has_leading_delims(char const *s, char const *set);
bool					has_lagging_delims(char const *s, char const *set);

// token_utils_combine_utils_2: loading helpers
int						_put_str_on_toklst(t_state *s, char **str);
int						_load_tmp_normal(t_state *s, char *raw, char **str);
int						_load_str_expanded_quoted(t_state *s, \
							char *raw, char **str);
int						_load_str_expanded(t_state *s, char *raw, \
							char **str);

// token_utils_split.c:  core splitting utils and entry point
char					*_skip_delims_post(char const *s, char const *ref);
char					*_skip_delims(char const *s, char const *ref);
int						tok_do_wordsplits(t_mem_mgr *mgr, \
							t_list **lst_pos, void *lst_c);

// token_utils_split_utils.c: support insertion and word splitting
int						do_tok_inserts(t_mem_mgr *mgr, \
							t_list **lst_pos, t_list **ins_lst, \
							void *(*createf)(t_mem_mgr *, const void *));
t_list					*split_word(t_mem_mgr *mgr, const char *word);

// memory management
void					dealloc_str(t_state *s, char *str_to_free);

#endif
