#include "parse.h"

# define FAIL_TOKEN 10 //TODO group somewhere else

/* chars that need to be quoted if meant literally */
#define SPECCHARS "#$^*()=|{}[]`<>?~;&\n\t \\\'\""


/* PARSE
 * Returns AST object from user string input
 * Does variable expansions with t_state
 */

/* TOKENIZE
 * Does these rules:
 * white space skipped outside of quotes
 * single quotes are closed, and protect string literal
 * double quotes allow var expansion
 * escape sequences validated, even inside double quotes
 * looks ahead a few chars to identify multi-char operators
 * 
 */ 

/* Finite State Machine rules
 * an operator cannot be followed by a flag, must be followed by a word
 * a cmd cannot be followed by a cmd
 * an arg cannot be followed by a cmd (unless the arg is complex)
 */

#define OP_AND "&&"
#define OP_OR "||" 
#define OP_REDOUT ">"
#define OP_REDIN "<"
#define OP_APPEND ">>"
#define OP_HEREDOC "<<"
#define OP_PIPE "|"
#define OP_DOL "$"
#define OP_STAR "*"

// TODO add redirection types enum
// TODO add reserved words
// TODO add quote types

// Note: (&&, ||) are sublists
// Note: (<, >) are redirs, followed by fd1
// Note: (=) is assignment, followed by scalar (ignore array)
// Note: (reserved words)

/* States
 * inside single quotes
 * inside double quotes
 * inside escape sequence
 * operator parsing
 */


// TODO use a stack to track open/close delimiters
//

typedef enum {
	TOK_CMD,
	TOK_ARG,
	TOK_OP,
	TOK_QUOTE,
	TOK_ESCAPE,
	TOK_UNK
} e_tok_type

typedef struct s_tok *t_token;

typedef struct {
	e_tok_type type;
	char *raw;
} s_tok;


is_argument(const char *s)
{
	// flags '-' '--' and stuff until whitespace
	// flags with args '-l 100'
	//
}


is_operator(const char *s)
{

}


tokenize()
{

}


t_ast	parse(t_state s, char *input)
{
	// work with example: [builtcmd 'arg' && cmd -arg -arg "$VAR" > fn]
	t_ll llist; //TODO define this
	if (0 != tokenize(input, &llist))
		return (null_and_stat(s, FAIL_TOKEN));

}

// For debugging
void	parse_print(t_ast ast)
{
	//TODO
	// ast_traverse
}

