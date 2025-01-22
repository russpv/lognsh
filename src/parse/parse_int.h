#ifndef PARSE_INT_H
# define PARSE_INT_H

#include "parse.h"

// Operator precedence for operators like &&, ||, |, etc.
// Grouping in commands (e.g., command1 && (command2 || command3)).

/*
See Noyce, Robert. Crafting Interpreters. 

(Lower Precedence)
command			→ pipe_command
pipe_command	→ logical_command ( "|" logical_command )*
logical_command	→ command ( "&&" command | "||" command )*
command			→ argument* operator* ( redirect_operator )
(Higher Precedence)

command    → simple_command ( flags )*
flags      → "-" letters


command      → logical_command ( pipe_command )*
logical_command → pipe_command ( logical_operator pipe_command )*
pipe_command  → simple_command ( redirect_operator )*
simple_command → command_name argument*

*/

/* Parser */

typedef struct {
    int depth;
} t_stack;

typedef	struct s_parser {
	t_list *tokens;
	t_list *current_tok;
	int		current_idx;
	int 	token_count; // ?
	t_tok 	*curr_cmd;
	t_stack	*stack;
} t_parser;



/* AST */
typedef bool (*t_validator_cmd)(t_ast_node_cmd *cmd);
typedef bool (*t_validator_op)(t_ast_node_op *op);
typedef bool (*t_validator_redir)(t_ast_node_op_redir *redir);
typedef bool (*t_validator_pipe)(t_ast_node_op_pipe *pipe);

typedef void (*t_printer_cmd)(t_ast_node_cmd *cmd);
typedef void (*t_printer_op)(t_ast_node_op *op);
typedef void (*t_printer_redir)(t_ast_node_op_redir *redir);
typedef void (*t_printer_pipe)(t_ast_node_op_pipe *pipe);

enum e_ast_node_type {
    AST_NODE_CMD,
    AST_NODE_OP,
    AST_NODE_OP_REDIR,
    AST_NODE_OP_PIPE
};

union u_ast_data {
	t_ast_node_cmd 		cmd;
	t_ast_node_op		op;
	t_ast_node_op_redir redir;
	t_ast_node_op_pipe	pipe;
};

typedef struct s_node {
	enum e_ast_node_type type;
	union u_ast_data data;
} t_ast_node;

typedef struct s_cmd {
	char 	*name;
	t_list *args;
	int		argc;
	t_validator_cmd accept_validator;
	t_printer_cmd accept_printer;
} t_ast_node_cmd;

typedef struct s_arg {
	char	*raw;
	bool	option;
	bool	do_globbing;
	bool	do_expansion;
} t_arg;

typedef struct s_op {
	char	*symbol;
	t_ast_node *left;
	t_ast_node *right;
	t_validator_op accept_validator;
	t_printer_op accept_printer;
} t_ast_node_op;

typedef struct s_op_redir {
	char	*symbol;
	t_ast_node_cmd *cmd;
	char 	*filename;
	t_validator_redir accept_validator;
	t_printer_redir accept_printer;
} t_ast_node_op_redir;

typedef struct s_op_pipe {
		char	*symbol;
	t_validator_pipe accept_validator;
	t_printer_pipe accept_printer;
} t_ast_node_op_pipe;

#endif
