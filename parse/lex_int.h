#include "lex.h"

# define FAIL_TOKEN 10 //TODO group somewhere else

/* chars that need to be quoted if meant literally */
#define NORMALDELIMS "#^*()=|{}[]`<>?~;&\n\t \\\'\"" //$ should not break tokens
#define NORMALTRANSITIONS "\'\"\0"
#define LEX_BUFSZ 1024
#define	RESET 0

/* LEX
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
// track state with flags in_single_quotes, in_double_quotes, escaping
// treat space as delimiter only if all these are false
// toggle quotes if escaping is false

/* On separators
 *
 * "A cornerstone of UNIX wisdom is that “plaintext is the 
 * universal interface.” Hence, it is not strongly typed; it’s stringly typed."
 * - a grumpy programmer
 */
// Notes: These bash separators must be lexed properly
// line-separator \n (treated as separate commands) vs continuation '\'
// but continuation char not needed for pipes
// continuation expected for heredocs
// continuation is equivalent of ';'
// 
// Be careful of spaces var="name with space" will expand to a list, not a string
// bash doesn't allow this to expand to two arguments: var='a "b c"', just 1 or 3
//
// Colon 
// Could be in a dir or filename, but for simplicity will treat as a char, or 
// the PATH delimiter
//
/* The State Machine
 * General states:
 * START: Beginning of input or start of a new token.
 * IN_WORD: Collecting characters in a word token.
 * IN_OPERATOR: Collecting characters in an operator token.
 * IN_QUOTE: Processing quoted text.
 * IN_SUBSTITUTION: Processing parameter, command, or arithmetic substitution.
 * IN_COMMENT: Discarding characters in a comment.
 * IN_HEREDOC: 
 * END: End of input.
 *
 * Use token buffer and token list 
 *
 * Traverse input ONCE
 *
 * Handle these rules
 * Hash - Ignore everything till newline
 * Whitespace - discard unquoted space and finalize current token
 * Start char - start a new token
 * Quotes - track ' " or \ and collect until closing symbol found
 * Operators - look ahead one char for double char ops
 * Null term - finalize current token
 * (distinguish reserved words)
 * (remove escape chars)
 *
 * Tokens
 * TOKEN: WORD, ASSIGNMENT_WORD, NAME, (reserved word)
 * OPERATOR: (below)
 * NEWLINE
 * IO_NUMBER e.g. &2>1
 *
 * AND_IF
 * OR_IF
 * DLESS
 * DGREAT
 */ 

typedef enum {
	NORMAL,
	IN_SINGLE_QUOTES,
	IN_DOUBLE_QUOTES,
	ON_EOF,
	DONE
} e_lex_state;

typedef int	(*t_tokenizer)(t_lex *l); 

typedef struct s_lex {
	const char 	*raw_string;
	char 		*ptr;
	e_lex_state	state;
	bool		escape_mode;
	t_tokenizer	tokenizer;
	t_list	*token_list;
	t_ht	hasht;
	char	*buf;
	int		do_expansion;
	int		do_globbing;
} t_lex;

/* This gets inserted as s_ht_entry->data */
struct s_ht_data {
	bool		is_substring;
	e_tok_type	type;
};

typedef	struct s_ht_data *t_ht_data;

void	build_hasht(t_lex *lexer);
t_ht_data	lex_create_ht_node(int is_substring, int type);
void	lex_destroy_ht_node(void *node);
void *lex_copy_ht_data(void *data);

int	tokenize_normal(t_lex *lexer);
int	tokenize_single_quotes(t_lex *lexer);
int	tokenize_double_quotes(t_lex *lexer);
int	tokenize_null(t_lex *lexer);
t_tok	*lex_create_token(t_lex *lexer, int type);
int	add_token(t_lex *lexer, t_tok *token);

void debug_detect_cycle(t_list *head);
