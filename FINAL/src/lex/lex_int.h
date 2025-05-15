/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_int.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:33 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:32:34 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEX_INT_H
# define LEX_INT_H

# include "lex.h"

# define _MOD_ "Lexer"
# define SKIPREST 1
# define NOGLOB 1
# define NOEXPD 1
# define FAIL_TOKEN 10
# define LEX_MAX_TOKC 1000

/* LEX
 *
 * Selectively loads input string into buf, marking
 * current section with processing properties
 * until a delimiter is found, then creates a token
 * and resets buf.
 *
 * Does these rules:
 * - white space skipped outside of quotes
 * - single quotes protect string literal
 * - double quotes allow shell var expansion
 * - escape sequences also validate inside double quotes
 * - tokenizes group/subtokens if a transition is reached
 * 		in dollar or double quote modes
 *
 * On separators:
 * "A cornerstone of UNIX wisdom is that “plaintext is the
 * universal interface.” Hence, it is not strongly typed;
 * it’s stringly typed."
 * - a grumpy programmer
 *
 * Note: no multi-line commands, so no line continuation or
 * unclosed quotes are recognized.
 */

/* chars that need to be quoted if meant literally */
# define NORMALDELIMS "()|<>\n\t &\0"
// no space needed to split singles: "()|<> \n\t"
// no space needed to split doubles: "<<,>>,&&,||"
// space needed to split (hence addressed above) "[ \'],[ \"]"
// doesn't split "\'\"{}[]=""
// not implemented ~^`;&
// $, \, # do not delimit tokens, and are skipped
// Removed '*' so that it is included in token raws
// and '?'?

/* ops that have no following delims */
# define NOTDELIMITED "()<>|"

# define NORMALTRANSITIONS "$\'\"<\0"
// the '\0' isn't tested, keep at end,
//	< for heredoc
// # handled in NORMAL
// $ must be followed by alphanum or _ to delim a (sub)token

# define LEX_BUFSZ 1024
# define MAX_HDOCSZ LEX_BUFSZ
# define INITVAL 0
# define MOD "Lexer"

enum						e_lex_state
{
	NORMAL,
	IN_DOLLAR,
	IN_DOUBLE_QUOTES,
	IN_SINGLE_QUOTES,
	IN_HEREDOC,
	ON_EOF,
	DONE
};

typedef int					(*t_tokenizer)(t_state *s, t_lex *l);

typedef struct s_lex
{
	const char				*raw_string;
	char					*ptr;

	enum e_lex_state		state;
	t_tokenizer				tokenizer;

	t_ht					hasht;
	char					*buf;
	size_t					buf_idx;
	t_tok					*last_grp_tok;
	t_int_stack				*stack;

	bool					escape_mode;
	int						do_expansion;
	int						do_globbing;
	int						do_heredoc;
	int						do_heredoc_expansion;
	bool					do_wordsplit;
	bool					is_subtoken;
	int						is_assignment;

	char					*eof_word;
	int						lines;

	bool					input_incomplete;
	int						lex_err;
	int						keep_dollar;

	t_list					*token_list;
	int						tokc;

	t_state					*global_state;
}							t_lex;

/* This gets inserted as s_ht_entry->data */
struct						s_ht_data
{
	bool					is_substring;
	enum e_tok_type			type;
};

/* Forwards */
typedef struct s_ht_data	*t_ht_data;
extern int					exec_heredoc(t_state *s, t_lex *l);

t_lex						*create_lexer(t_state *state, int start_state,
								const char *s);
void						destroy_lexer(t_mem_mgr *m, void **instance);

/* ht */
void						build_hasht(t_mem_mgr *m, t_lex *lexer);
t_ht_data					lex_create_ht_node(t_mem_mgr *m, bool is_substring,
								int type);
void						lex_destroy_ht_node(t_mem_node *n, void *node);
void						*lex_copy_ht_data(t_mem_node *n, void *data);
t_tok						*lex_ht_lookup(t_state *s, t_lex *lexer);

/* Strategies */
int							do_state_transition(t_lex *lexer);
int							tokenize_normal(t_state *s, t_lex *lexer);
int							tokenize_single_quotes(t_state *s, t_lex *lexer);
int							tokenize_double_quotes(t_state *s, t_lex *lexer);
int							tokenize_null(t_state *s, t_lex *lexer);
int							tokenize_heredoc(t_state *s, t_lex *lexer);
int							tokenize_dollar(t_state *s, t_lex *lexer);
bool						handle_quote_state(t_lex *lexer);

/* Tokenize ops */
t_tok						*lex_create_token(t_mem_mgr *st, t_lex *lexer,
								int type);
int							add_token(t_mem_mgr *s, t_lex *lexer, t_tok *token);
int							add_grptoken(t_mem_mgr *m, t_lex *lexer);

/* Char sequence checks */
bool						is_normal_delim(t_lex *lexer, int offset);
bool						is_transition_delim(t_lex *lexer);
bool						is_dollar_delim(t_lex *lexer);
bool						is_logicalop(unsigned char c, unsigned char next);
bool						is_dollar_question(t_lex *lexer);

int							word_or_name(const char *s);
bool						is_too_long(t_lex *l, const char *input);

void						process_escape_sequence(t_lex *l);
int							process_special_operators(t_lex *lexer);
struct s_ht_entry			*do_one_char_lookahead(t_lex *lexer,
								struct s_ht_entry *res);

/* heredoc */
int							get_eof_word(t_mem_mgr *m, t_lex *l);
bool						on_cmd_op(t_lex *l);

/* dollar subs */
bool						is_specialchar(unsigned char c);
bool						is_varnamechar(unsigned char c);

/* utils */
int							put_on_buf(t_lex *l);
unsigned char				is_valid_next(t_lex *l, int offset);
unsigned char				is_valid_prev(t_lex *l, int offset);
void						lex_set_err(t_lex *l);

#endif