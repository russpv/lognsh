#ifndef PARSE_INT_H
# define PARSE_INT_H

# include "parse.h"

# define MAX_CMD_ARGS 10

// TODO: $ and * expansions. Send to lexer. Create tokens_insert method.
// unsure when redirect expansion occurs; might as well do it asap.
// if multiple words result, throw 'bash: $VAR: ambiguous redirect'

// TODO expand_env_var(): if expansion flag is set, searches ENV for value.
// TODO expand_exit_status(): if TOK_EXIT_STATUS, gets last value.
// TODO split_words(): split on IFS, default space, tab, newline.

// Note: multi-word expansions trigger an error for redirects. they cannot introduce functionality,
//	so lexing is different.
// Note: redirections can supersede piping
// Note: expansion does not alter the parse tree structure. All words are treated as arguments,
// unless no command was parsed,
//	then the first word is treated as the command name.
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
}							t_ast_node_cmd;

typedef struct s_redir
{
	char					*symbol;
	int						type;
	char					*filename;
	char					*doc;
	t_ast_node_cmd			*cmd;
}							t_redir_data;

typedef struct s_arg
{
	char					*raw;
	char					**tmp;
	bool					option;
	bool					do_globbing;
	bool					do_expansion;
	bool					in_dquotes;
}							t_arg_data;

/* Next refactor, remove the t_list since
 * this can only be a wrapper for one cmd
 */
typedef struct s_proc
{
	t_list					*cmds;
	int						cmdc;
}							t_ast_node_proc;

typedef struct s_op
{
	char					**operators;
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
	t_stack					*st;
	t_ast_node				*last_node;
	t_ast_node				*ref_node;
	int						curr_idx;
	t_tok					*curr_cmd;
	t_ast_node				*ast;
	bool					parse_error;
}							t_parser;

t_parser					*create_parser(t_state *s, t_list *tokens);
void						destroy_parser(void *instance);
t_tok						*peek(t_parser *p);
t_tok						*lookahead(t_parser *p);
t_tok						*previous(t_parser *p);
t_tok						*advance(t_parser *p);

t_tok						*peek(t_parser *p);
t_tok						*lookahead(t_parser *p);
t_tok						*previous(t_parser *p);
bool						is_at_end(t_parser *p);

t_ast_node					*parse_cmd(t_parser *p);
t_ast_node					*parse_full_cmd(t_parser *p);
t_ast_node					*parse_pipeline(t_parser *p);
t_ast_node					*parse_proc(t_parser *p);
t_ast_node					*parse_logical(t_parser *p);

int							process_redir(t_parser *p, t_ast_node *cmd_node);

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

void						parse_print(t_ast_node *ast);
t_ast_node					*test_parse(t_parser *parser);

void	p_do_expansion(void *content);


#endif
