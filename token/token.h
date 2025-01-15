#ifndef TOKEN_H
# define TOKEN_H

// Macros defining tokens
// ...

typedef enum {
	NODE_COMMAND,
	NODE_CONDITIONAL, // "||" "&&"
	NODE_OPERATOR, // "|"
	NODE_REDIRECTION, //"<" ">"

} e_ast_node_type;

// typesafe union for AST node metadata
typedef union {
	t_token	tok;
	char *filename;
} u_ast_node_data;



#endif
