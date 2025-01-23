#ifndef PARSE_INT_H
# define PARSE_INT_H

# include "parse.h"

// Operator precedence for operators like &&, ||, |, etc.
// Note: bash equates && and || unusually, so left to right order
// Grouping in commands (e.g., command1 && (command2 || command3)).

/*
From Bash Manual:
These (expansion) operations are performed in this order:

Parameter expansion: $var is replaced with its ENV value. No fancy stuff. Also, $0 and $? do their thing. 
Command substition: $(command) replaces command with its output. If this appears in double quotes, word splitting and filename expansion
are not performed on output. 
Word splitting: checks results of expansions to split words
Filename expansion: if word has unquoted ‘*’, it is replaced with alphabetically sorted list of matching (case sensitive) filenames in pwd.
If no matches, the word is left unchanged, or removed, or error thrown. Starting dots must be matched explicitly. 
Pattern matching: * Matches any string, including the null string
Quote removal: any [\ ' "], not resulting from an expansion, and outside of quotes, is removed

Execution:
- Expansions on words other than redirections. If any, first word is taken as the command name, remaining are args. 
- Redirections are performed.
- If no command name results, redirections are performed, but do not affect the current shell environment. A redirection error causes the command to exit with a non-zero status. 
- If one of the expansions contained a command substitution, the exit status of the command is the exit status of the last command substitution performed. If there were no command substitutions, the command exits with a status of zero. 

See Noyce, Robert. Crafting Interpreters.

(Lower Precedence)
command			→ pipe_command
pipe_command	→ logical_command ( "|" logical_command )*
logical_command	→ command ( "&&" command | "||" command )*
command			→ argument* operator* ( redirect_operator )
(Higher Precedence)

command    → simple_command ( flags )*
flags      → "-" letters


command			→ logical_command ( pipe_command )*
logical_command	→ pipe_command ( logical_operator pipe_command )*
pipe_command	→ simple_command ( redirect_operator )*
simple_command	→ command_name argument*

Redirects are higher than pipes
sort < names | head
( sort < names ) | head

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
 $(command) Command substitions
 ${parameter:=word}
 filename matching


*/

/* Parser */

typedef struct
{
	int						depth;
}							t_stack;

typedef struct s_parser
{
	t_list					*tokens;
	t_list					*current_tok;
	int						current_idx;
	int token_count; // ?
	t_tok					*curr_cmd;
	t_stack					*stack;
}							t_parser;

/* AST */
/* Note: not sure if we need these for separate validation pass */
typedef bool				(*t_validator_cmd)(t_ast_node_cmd *cmd);
typedef bool				(*t_validator_op)(t_ast_node_op *op);
typedef bool				(*t_validator_redir)(t_ast_node_op_redir *redir);
typedef bool				(*t_validator_pipe)(t_ast_node_op_pipe *pipe);

typedef void				(*t_printer_cmd)(t_ast_node_cmd *cmd);
typedef void				(*t_printer_op)(t_ast_node_op *op);
typedef void				(*t_printer_redir)(t_ast_node_op_redir *redir);
typedef void				(*t_printer_pipe)(t_ast_node_op_pipe *pipe);

enum						e_ast_node_type
{
	AST_NODE_CMD,
	AST_NODE_OP,
	AST_NODE_OP_REDIR,
	AST_NODE_OP_PIPE
};

union						u_ast_data
{
	t_ast_node_cmd			cmd;
	t_ast_node_op			op;
	t_ast_node_op_redir		redir;
	t_ast_node_op_pipe		pipe;
};

typedef struct s_node
{
	enum e_ast_node_type	type;
	union u_ast_data		data;
}							t_ast_node;

typedef struct s_cmd
{
	char					*name;
	t_list					*args;
	int						argc;
	t_validator_cmd			accept_validator;
	t_printer_cmd			accept_printer;
}							t_ast_node_cmd;

typedef struct s_arg
{
	char					*raw;
	bool					option;
	bool					do_globbing;
	bool					do_expansion;
}							t_arg;

typedef struct s_op
{
	char					*symbol;
	t_ast_node				*left;
	t_ast_node				*right;
	t_validator_op			accept_validator;
	t_printer_op			accept_printer;
}							t_ast_node_op;

typedef struct s_op_redir
{
	char					*symbol;
	t_ast_node_cmd			*cmd;
	char					*filename;
	t_validator_redir		accept_validator;
	t_printer_redir			accept_printer;
}							t_ast_node_op_redir;

typedef struct s_op_pipe
{
	char					*symbol;
	t_validator_pipe		accept_validator;
	t_printer_pipe			accept_printer;
}							t_ast_node_op_pipe;

#endif
