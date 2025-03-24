#ifndef PARSE_INT_H
# define PARSE_INT_H

# include "parse.h"

# define _MOD_ "Parser"
# define MAX_CMD_ARGS 10
# define DMSG_IN _MOD_ ": %s: got: %s\n"
# define DMSG_OUT _MOD_ ": %s: found: %s\n"

/*
From Bash Manual:
These (expansion) operations are performed in this order:

1) Parameter expansion: $VAR is replaced with its ENV value. No fancy stuff. Also,
	$0 and $? do their thing.
(excluded) Command substition: $(command) replaces command with its output. If this appears in double quotes,
	word splitting and filename expansion are not performed on output.
2) Word splitting: checks results of expansions to split words
3) Filename expansion: if word has unquoted ‘*’,
	it is replaced with alphabetically sorted
	list of matching (case sensitive) filenames in pwd. If no matches,
		the word is left
	unchanged, or removed,
		or error thrown. Starting dots must be matched explicitly.
(excluded) Pattern matching: * Matches any string, including the null string
4) Quote removal: any [\ ' "], not resulting from an expansion,
	and outside of quotes, is removed

Execution:
- Expansions (see above) on words other than redirections. If any,
	first word is taken as the command
		name, remaining are args.
- Redirections are performed.
- If no command name results, redirections are performed,
	but do not affect the current
	shell environment. A redirection error causes the command to exit with a non-zero status.
- If one of the expansions contained a command substitution,
	the exit status of the command
	is the exit status of the last command substitution performed. If there were no command
		substitutions, the command exits with a status of zero.

See Noyce, Robert. Crafting Interpreters.

full_command	→ logical_command*
logical_command	→ (proc | simple | pipeline) [operator (proc | simple | pipeline)]*
pipeline		→ (proc | simple) ['|' (proc | simple) ]*
simple_command	→ (redirects)* command_name args* (redirects)* (command_name optional,
		bash just does nothing)

proc			→ '(' full_command ')'

redirect  		→ [('<' | '>' | '>>' | '<<')][proc | literal]) (no space)

args → (flags)* word*
word → literal | variable | command_substitution
	// note: no command_substitution now
variable → '$' (alphanumeric | '_')+

command_substitution → '$(' full_command ')'


Attempting a hand-written LL(1) recursive descent parser.

"redirection operators may precede or appear anywhere within a simple
command or may follow a command. Redirections are processed in the
 order they appear, from left to right."
if you redirect multiple times, all the redirections are performed,
 but only the last redirection will take effect (assuming none of
 the earlier redirections cause error)

 3.6.2 Redirecting Output

Redirection of output causes the file whose name results from the
expansion of word to be opened for writing on file descriptor n,
	or the standard output (file descriptor 1) if n is not specified.
	If the file does not exist it is created; if it does exist it is truncated to zero size.

 Expansions
 Precedence:
 $(command) Command substitions Note: not implemented.
 ${parameter:=word} Note: not implemented.
 filename matching Note: not implemented.

Note: any split words on an expansion of a redirection fd results in an error
*/

typedef struct
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

	bool					do_globbing;
	bool					do_expansion;
	bool					do_wordsplit;
	bool					do_redir_globbing;
	bool					do_redir_expansion;
	bool					has_grouptoks;
}							t_ast_node_cmd;

// TODO, add heredoc flags for:
// dquoted heredoc "EOF" (no globs, do expansions, escapes)
// squoted heredoc 'EOF' (no globs, no expansions, no escapes)
// ensure quotes removed from eof token prior to input
// ensure eof token is never expanded itself, and $ is literal

// normal flags for:
// dquoted <"$fn" (no word splitting on expansion)
// squoted <'$fn' (no expansion on fn)
typedef struct s_redir
{
	char					*symbol;
	int						type;
	char					*filename;
	char					*heredoc_body;
	t_ast_node				*target_ptr;
	bool					do_globbing;
	bool					do_expansion;
	t_list					*lst_glob;
	t_state					*global_state;
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

/* Next refactor, remove the t_list since
 * this can only be a wrapper for one cmd
 * cmds is a llist vehicle for ast nodes
 */
typedef struct s_proc
{
	t_list					*cmds;
	int						cmdc;
	t_list					*redirs;
	int						redc;
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
void						destroy_parser(t_mem_mgr *m, void *instance);
t_ast_node					*init_log(t_mem_mgr *m);
t_redir_data				*init_redir(t_mem_mgr *m, t_ast_node *target,
								enum e_tok_type type);
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
void						*create_arg_data_node(void *content);
void						*create_arg_data_node_deep(t_mem_mgr *mgr,
								const void *content);
t_pstack					*create_stack(t_mem_mgr *m);
void						destroy_stack(t_mem_mgr *m, t_pstack *s);
int							push(t_pstack *stack);
int							pop(t_pstack *stack);

/* Execution helpers */
int							p_do_globbing_args(t_mem_mgr *mgr,
								t_list **lst_node, void *lst_c);
int							p_do_globbing_redirs(t_mem_mgr *mgr, void *c);
int							check_special_expansions(t_state *s,
								const char *buf, char **value);
t_list						*match_glob(t_mem_mgr *mgr, const char *pattern);
int							do_arg_inserts(t_mem_mgr *mgr, t_list **lst_node,
								t_list **ins_lst, t_arg_data *content);
void						*token_to_arg(t_mem_mgr *m, const void *tok);
int							p_do_grparg_processing(t_state *s, t_list **this,
								void *c);
int							p_do_wordsplits(t_mem_mgr *mgr, t_list **lst_node,
								void *lst_c);

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
									void *));
int							ft_lstiter_state_ins_rwd_tmp(t_state *s,
								t_list **lst, int (*f)(t_state *, t_list **,
									void *));
int							p_is_arg_null(void *c);

/* AST list frees */
void						destroy_ast_node(t_mem_mgr *m, void *node);
void						destroy_pipe_node(t_mem_mgr *m, void *n);
void						destroy_cmd_node(t_mem_mgr *m, void *n);
void						destroy_proc_node(t_mem_mgr *m, void *n);
void						destroy_log_node(t_mem_mgr *m, void *n);
void						destroy_arg(t_mem_mgr *m, void *in);

/* Debugging */
void						parse_print(t_ast_node *ast);
t_ast_node					*test_parse(t_state *s, t_parser *parser);

/* Setters and Getters */
int							p_update_redc(t_ast_node *a, int amt);
t_list						**p_get_redirs_ptr(t_ast_node *a);

#endif
