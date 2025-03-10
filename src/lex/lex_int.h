#include "lex.h"

#define _MOD_ "Lexer"
#define SKIPREST 1
#define NOGLOB 1
#define NOEXPD 1
#define FAIL_TOKEN 10 // TODO group somewhere else
#define LEX_MAX_TOKC 1000

/* Space is not a reliable delimiter. Assume no space. */
// Capture operators in NORMAL mode.
// Capture subtokens in ENVVAR mode. Simplifies NORMAL.
// Capture literal in SQUOTE.
// Capture reduced processed tokens in DQUOTE.
// How do I know I'm done with a subtoken streak?
/*

These are the conditions for exiting DOLLAR mode
... (end of valid name) +
These are the conditions for tokenizing full tokens
...&& not ...&
...|[|] is_normal_delim
...<* ...>* is_normal_delim
...( or )  is_normal_delim
...\n or \t or ' ' is_normal_delim

These are the conditions for tokenizing subtokens
...\' ...\'
...\" ...\"
...$?
...$[varname]
Delimited by normal delims

Any of the states must stop on the tokenizing delimiters
All state transitions otherwise signify subtokens unless tokenizing delimiter also follows.
So we can end on quotation marks,
	and if we were part of a subtoken streak we would have to
check if characters other than any of these delimiter patterns is present after the transition
chars. That must be done each call to lex_create_token. Might as well encapsulate in that func.

*/

/* chars that need to be quoted if meant literally */
#define NORMALDELIMS "?()|<>\n\t &\0"
// no space needed to split singles: "()|<> \n\t"
// no space needed to split doubles: "<<,>>,&&,||"
// space needed to split (hence addressed above) "[ \'],[ \"]"
// doesn't split "\'\"{}[]=""
// not implemented ~^`;&
// $, \, # do not delimit tokens, and are skipped
// Removed '*' so that it is included in token raws
// TODO, remove '?'
#define NORMALTRANSITIONS "$\'\"<\0"
// the '\0' isn't tested, keep at end,
//	< for heredoc
// # handled in NORMAL
// $ must be followed by alphanum or _ to delim a (sub)token
#define LEX_BUFSZ 1024
#define INITVAL 0 // Lexer default flag value
#define MOD "Lexer"

/* LEX
 *
 * Selectively loads input string into buf, marking
 * current section with processing properties
 * until a delimiter is found, then creates a token
 * and resets buf.
 *
 * Does these rules:
 * white space skipped outside of quotes
 * single quotes are closed, and protect string literal
 * double quotes allow var expansion
 * escape sequences validated, even inside double quotes
 * looks ahead a few chars to identify multi-char operators
 * tokenizes group tokens if, in the course of normal state,
 * single or double quote state is encountered.
 */

/* Finite State Machine rules
 * an operator cannot be followed by a flag, must be followed by a word
 * a cmd cannot be followed by a cmd
 * an arg cannot be followed by a cmd (unless the arg is complex)
 */

/* States
 * inside single quotes
 * inside double quotes
 * inside escape sequence
 * operator parsing
 */

// Improvements:
// Could use a stack to handle missing closing delimiter of a pair
// Could lex multi-line commands

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
// Be careful of spaces var="name with space" will expand to a list,
// not a string
// bash doesn't allow this to expand to two arguments: var='a "b c"',
// just 1 or 3
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
	bool					escape_mode;
	t_tokenizer				tokenizer;
	t_list					*token_list;
	int						tokc;
	t_tok					*last_grp_tok;
	t_ht					hasht;
	char					*buf;
	size_t					buf_idx;
	char					*expd_mask;
	char					*glob_mask;
	int						do_expansion;
	int						do_globbing;
	int						do_heredoc;
	int						do_heredoc_expansion;
	bool					is_subtoken;
	char					*eof_word;
	bool					input_incomplete;
	int						keep_dollar;
}							t_lex;

/* This gets inserted as s_ht_entry->data */
struct						s_ht_data
{
	bool					is_substring;
	enum e_tok_type			type;
};

typedef struct s_ht_data	*t_ht_data;

t_lex						*create_lexer(t_state *st, int start_state,
								const char *s);
void	destroy_lexer(t_state *s, void *instance);
void						init_token_masks(t_lex *l);

/* ht */
void						build_hasht(t_lex *lexer);
t_ht_data					lex_create_ht_node(int is_substring, int type);
void						lex_destroy_ht_node(void *node);
void						*lex_copy_ht_data(void *data);
t_tok						*lex_ht_lookup(t_state *s, t_lex *lexer);

int							tokenize_normal(t_state *s, t_lex *lexer);
int							tokenize_single_quotes(t_state *s, t_lex *lexer);
int							tokenize_double_quotes(t_state *s, t_lex *lexer);
int							tokenize_null(t_state *s, t_lex *lexer);
int							tokenize_heredoc(t_state *s, t_lex *lexer);
int							tokenize_dollar(t_state *s, t_lex *lexer);

t_tok						*lex_create_token(t_state *st, t_lex *lexer,
								int type);
int							add_token(t_lex *lexer, t_tok *token);

bool						is_normal_delim(unsigned char s, char *next);
bool						is_transition_delim(unsigned char s, char *next);
bool						is_dollar_delim(unsigned char c, char *next);

bool						is_dollar_question(t_lex *lexer);

int							word_or_name(const char *s);
bool						is_too_long(const char *input);

void						process_escape_sequence(t_lex *l);
int							process_special_operators(t_lex *lexer);
struct s_ht_entry			*do_one_char_lookahead(t_lex *lexer,
								struct s_ht_entry *res);
int							do_state_transition(t_lex *lexer);

/* heredoc */
int							get_eof_word(t_lex *l);
bool						on_cmd_op(t_lex *l);
bool						is_varnamechar(unsigned char c);

int							put_on_buf(t_lex *l);
