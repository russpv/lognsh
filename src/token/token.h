#ifndef TOKEN_H
# define TOKEN_H

/* TOKEN ADT */

# include "../debug.h"
# include "../state/state.h"
# include <stdbool.h>
# include <stddef.h>
# include <stdlib.h>

# define TK_EOF '\0'
# define TK_STAT "$?"
# define TK_NEWL '\n'
# define TK_ESC '\\'

# define OP_EQUALS '='
# define OP_HASH '#' // skip everything till newline
# define OP_ANDIF "&&"
# define OP_FD_REF '&' // redirection
# define OP_ORIF "||"
# define OP_REDOUT '>'
# define OP_REDIN '<'
# define OP_DGREAT ">>"
# define OP_DLESS "<<"
# define OP_PIPE '|'
# define OP_ENV '$' // this plus underscore or alphanumeric, an env_var
# define OP_STAR '*'
# define OP_SQUOTE '\''
# define OP_NULL '\0'
# define OP_OPTION '-'
# define OP_OPENP '('
# define OP_CLOSEDP ')'
# define OP_DQUOTE \
	'\"'                   // backslash retained if followed by [$`"\\n],
							//" requires \ inside ""
# define OP_DLESSDASH "<<-" // heredoc with opt tab trimming (not implemented)
# define OP_CLOBBER ">|"    // force overwrite (not implemented)
# define OP_BACKTICK '`'

/* Reserved words */
# define WD_IF "if"
# define WD_THEN "then"
# define WD_ELSE "else"
# define WD_FI "fi"
# define WD_DO "do"
# define WD_DONE "done"
# define WD_CASE "case"
# define WD_ESAC "esac"
# define WD_WHIL "while"
# define WD_UNTL "until"
# define WD_FOR "for"
# define WD_IN "in"

enum					e_tok_type
{
	TOK_WORD,            // command names, built-ins
							// In the shell command language, a token other than
							// an operator. In some cases a word is also a portion
							// of a word token: in the various forms of parameter
							// expansion, such as ${name-word}, and variable
							// assignment, such as name=word, the word is the
							// portion of the token depicted by word. The concept
							// of a word is no longer applicable following word
							// expansions-only fields remain.
	TOK_ASSIGNMENT_WORD, // (non-functional)
	TOK_NAME,            // In the shell command language, a word consisting
							// solely of underscores, digits, and alphabetics
							// from the portable character set. The first character
							// of a name is not a digit.
	TOK_NEWLINE,         // (non-functional)
	TOK_IO_NUMBER,       // REDOUT/REDIN plus digits
	TOK_OP_REF,          // "&" for stdio
	TOK_REDIRECT_IN,     //
	TOK_REDIRECT_OUT,    //
	TOK_REDIRECT_APPEND, //
	TOK_HEREDOC,         // TODO - tokenize this for parser
	TOK_HEREDOC_WORD,    // TODO
	TOK_PIPE,            //
	TOK_ANDIF,
	TOK_ORIF, //
	TOK_OPEN_PAREN,
	TOK_CLOSE_PAREN,
	TOK_SINGLE_QUOTE, // (non-functional)
	TOK_DOUBLE_QUOTE, // (non-functional)
	TOK_ENV_VAR,      //$
	TOK_EXIT_STATUS,  //$?
	TOK_BI,           // Builtin command
	TOK_IF,           // Reserved word "if"
	TOK_THEN,         // Reserved word "then"
	TOK_ELSE,         // Reserved word "else"
	TOK_FI,           // Reserved word "fi"
	TOK_DO,           // Reserved word "do"
	TOK_DONE,         // Reserved word "done"
	TOK_CASE,         // Reserved word "case"
	TOK_ESAC,         // Reserved word "esac"
	TOK_WHILE,        // Reserved word "while"
	TOK_UNTIL,        // Reserved word "until"
	TOK_FOR,          // Reserved word "for"
	TOK_IN,           // Reserved word "in"
	TOK_EOF,
	TOK_UNKNOWN,
	TOK_ERR
};

struct s_tok;
typedef struct s_tok	t_tok;

t_tok					*create_token(const char *s, int type, size_t pos);
t_tok					*create_ht_token(void);
void					*copy_token_data(const void *data);
void					destroy_token(void *token);

int						tok_set_globbing(t_tok *token);
int						tok_set_expansion(t_tok *token);
char					*tok_get_raw(t_tok *token);
enum e_tok_type			tok_get_type(t_tok *token);
bool					tok_get_option(t_tok *token);
bool					tok_get_globbing(t_tok *token);
bool					tok_get_expansion(t_tok *token);
bool					tok_get_dquotes(t_tok *token);

int						tok_set_dquotes(t_tok *token);
void					tok_print(void *token);

#endif
