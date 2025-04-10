/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_int.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:33:54 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:33:55 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_INT_H
# define PARSE_INT_H

# include "parse.h"
# include <stdbool.h>
# include <stdio.h>

# define _MOD_ "Parser"
# define MAX_CMD_ARGS 10
# define MAX_STACK_DEPTH 100
# define PDMSG_IN "%s: %s: got: %s\n"
# define PDMSG_OUT "%s: %s: found: %s\n"
# define LMSG_IN "%s: \t###### %s ####### \n"
# define PDMSG_GOT "%s: %s: got tok of %s \n"

/* Grammar
**
** See Noyce, Robert. Crafting Interpreters.
** Attempting a hand-written LL(1) recursive descent parser.
**
** full_command		→ logical_command*
** logical_command	→ (proc | simple | pipeline) [op (proc | simple | pipeline)]*
** pipeline			→ (proc | simple) ['|' (proc | simple) ]*
** simple_command	→ (redirects)* command_name (args* | redirects)*
**
** proc				→ '(' full_command ')'
** redirect  		→ [('<' | '>' | '>>' | '<<')][proc | literal]) (no space)
** args 			→ (flags)* word*
** word 			→ literal | variable | command_substitution
** variable 		→ '$' (alphanumeric | '_')+
**
** Output redirections cause the file to be created or truncated.
** Any split words resulting from a filename expansion throw an error
*/

typedef struct s_pstack
{
	int						depth;
}							t_pstack;

typedef struct s_cmd
{
	char					*name;
	t_list					*args;
	int						argc;
	t_list					*redirs;
	int						redc;

	bool					has_arggrouptoks;
	bool					do_globbing;
	bool					do_expansion;
	bool					do_wordsplit;
	bool					do_redir_globbing;
	bool					do_redir_expansion;
	bool					has_redgrouptoks;
}							t_ast_node_cmd;

typedef struct s_redir
{
	char					*symbol;
	int						type;
	char					*filename;
	char					*heredoc_body;
	t_ast_node				*target_ptr;
	bool					in_dquotes;
	bool					do_globbing;
	bool					do_expansion;
	t_list					*lst_glob;

	t_state					*global_state;

	bool					is_groupred;
	t_list					*lst_tokens;
}							t_redir_data;

typedef struct s_arg
{
	char					*raw;
	char					**tmp;
	bool					option;
	bool					do_globbing;
	bool					do_expansion;
	bool					in_dquotes;
	t_state					*global_state;

	bool					is_grouparg;
	t_list					*lst_tokens;
}							t_arg_data;

typedef struct s_proc
{
	t_list					*cmds;
	int						cmdc;
	t_list					*redirs;
	int						redc;

	bool					has_redgrouptoks;
	bool					do_redir_globbing;
	bool					do_redir_expansion;
}							t_ast_node_proc;

typedef struct s_op
{
	t_list					*operators;
	t_list					*cmds;
	int						cmdc;
}							t_ast_node_log;

typedef struct s_pipeline
{
	char					symbol;
	t_list					*cmds;
	int						cmdc;
}							t_ast_node_pipeline;

union						u_ast_data
{
	t_ast_node_log			log;
	t_ast_node_pipeline		pipe;
	t_ast_node_cmd			cmd;
	t_ast_node_proc			proc;
};

typedef struct s_node
{
	enum e_ast_node_type	type;
	union u_ast_data		data;
}							t_ast_node;

typedef struct s_parser
{
	t_list					*tokens;
	int						token_count;
	t_list					*curr_tok;
	t_int_stack				*st;
	t_ast_node				*last_node;
	t_ast_node				*ref_node;
	int						curr_idx;
	t_tok					*curr_cmd;
	t_ast_node				*ast;
	bool					parse_error;
	t_state					*global_state;
	t_mem_mgr				*mmgr;
}							t_parser;

t_parser					*create_parser(t_state *s, t_list *tokens);
void						destroy_parser(t_mem_mgr *m, void **instance);
t_ast_node					*init_log(t_mem_mgr *m);
t_redir_data				*init_redir(t_mem_mgr *m, t_parser *p,
								t_ast_node *t, enum e_tok_type typ);
t_arg_data					*init_arg(t_mem_mgr *m, t_parser *p,
								t_ast_node *cmd_node, t_tok *tok);

/* Token list navigation */
t_tok						*peek(t_parser *p);
t_tok						*lookahead(t_parser *p);
t_tok						*previous(t_parser *p);
t_tok						*advance(t_parser *p);
bool						is_at_end(t_parser *p);

/* Recursive descent functions */
t_ast_node					*parse_cmd(t_state *s, t_parser *p);
t_ast_node					*parse_full_cmd(t_state *s, t_parser *p);
t_ast_node					*parse_pipeline(t_state *s, t_parser *p);
t_ast_node					*parse_proc(t_state *s, t_parser *p);
t_ast_node					*parse_logical(t_state *s, t_parser *p);

/* Parsing helpers */
int							process_redir(t_parser *p, t_ast_node *cmd_node);
int							process_normal_redir(t_parser *p, t_tok *tok,
								t_redir_data *red, t_ast_node *n);
void						*create_arg_data_node(void *content);
t_pstack					*create_stack(t_mem_mgr *m);
void						destroy_stack(t_mem_mgr *m, t_pstack *s);
int							push(t_pstack *stack);
int							pop(t_pstack *stack);

void						p_set_has_redgrouptoks(t_ast_node *n, bool val);
void						p_set_do_redir_expansion(t_ast_node *n, bool val);
void						p_set_do_redir_globbing(t_ast_node *n, bool val);
bool						p_get_do_redir_exp(t_ast_node *n);
bool						p_get_do_redir_glob(t_ast_node *n);

/* Arg Expansions */
int							p_do_arg_expansion(t_state *s, void *c);
int							p_do_globbing_args(t_mem_mgr *mgr,
								t_list **lst_node, void *lst_c);
int							p_do_grparg_processing(t_state *s,
								t_list **this_node, void *c);
void						do_grparg_inserts(t_state *s, t_list **this_node);

int							p_do_globbing_toks(t_mem_mgr *mgr,
								t_list **lst_node, void *lst_c);
int							p_do_wordsplits(t_mem_mgr *mgr, t_list **lst_node,
								void *lst_c);
int							do_arg_inserts(t_mem_mgr *mgr, t_list **lst_node,
								t_list **ins_lst, t_arg_data *content);
void						*token_to_arg(t_mem_mgr *m, const void *tok);
void						*create_arg_data_node_deep(t_mem_mgr *mgr,
								const void *content);

/* Redir Expansions */
int							p_do_red_expansion(t_state *s, void *r);
int							p_do_heredoc_expansion(t_state *s, t_redir_data *r);
int							p_do_globbing_redirs(t_mem_mgr *mgr, void *c);
int							p_do_grpred_processing(t_state *s,
								t_list **this_red, void *c);
int							do_redir_inserts(t_state *s, t_list **this_node);
int							mymax(int one, int two);

void						*create_redir_data_node_deep(t_mem_mgr *mgr,
								const void *content);
void						*token_to_redir(t_mem_mgr *m, const void *tok);

/* Expansion Utils */
int							check_special_expansions(t_state *s,
								const char *buf, char **value);
t_list						*match_glob(t_mem_mgr *mgr, const char *pattern);

/* Tests */
bool						is_option(t_tok *tok);
bool						is_redir_token(t_tok *tok);
bool						is_filename_token(t_tok *tok);
bool						is_open_paren(t_tok *tok);
bool						is_cmd_token(t_tok *tok);
bool						is_log_token(t_tok *tok);
bool						is_pipe_token(t_tok *tok);
bool						is_op_token(t_tok *tok);
bool						is_heredoc_token(t_tok *tok);
bool						is_arg_token(t_tok *tok);
bool						is_expansion(t_tok *tok);
bool						is_close_paren(t_tok *tok);
bool						is_group_op_token(t_tok *tok);
bool						is_group_token(t_tok *tok);

t_tok						*which_lower_priority(t_ast_node *ref, t_tok *cmp);

/* For traversing the AST */
bool						node_has_redirects(t_ast_node *n);

/* Redirect function table */
typedef int					(*t_redir_fn)(const t_redir_data *node);

int							handle_redirect_in(const t_redir_data *node);
int							handle_redirect_out(const t_redir_data *node);
int							handle_redirect_append(const t_redir_data *node);
int							handle_heredoc(const t_redir_data *node);

/* Utils */
char						**list_to_array(t_mem_mgr *m, t_list *args,
								int argc);
int							lstiter_state(t_state *s, t_list *lst,
								int (*f)(t_state *, void *));
int							lstiter_state_rwd_trim(t_state *s, t_list **lst,
								int (*f)(void *), void (*del)(t_mem_mgr *,
									void **));
int							ft_lstiter_state_ins_rwd_mem(t_state *s,
								t_list **lst, int (*f)(t_state *, t_list **,
									void *));
int							p_is_arg_null(void *c);

/* AST list frees */
void						destroy_ast_node(t_mem_mgr *m, void **node);
void						destroy_pipe_node(t_mem_mgr *m, void **n);
void						destroy_cmd_node(t_mem_mgr *m, void **n);
void						destroy_proc_node(t_mem_mgr *m, void **n);
void						destroy_log_node(t_mem_mgr *m, void **n);
void						destroy_arg(t_mem_mgr *m, void **in);

/* Debugging */
void						parse_print(t_ast_node *ast);
t_ast_node					*test_parse(t_state *s, t_parser *parser);
int							print_arg(void *arg);
int							print_redir_tok(void *redir);
void						print_ast_cmd_helper(t_ast_node *ast);

/* Setters and Getters */
int							p_update_redc(t_ast_node *a, int amt);
t_list						**p_get_redirs_ptr(t_ast_node *a);

#endif
