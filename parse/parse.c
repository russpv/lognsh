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


#define TK_EOF "\0"
#define TK_STAT "$?"
#define TK_NEWL	"\n"
#define TK_ESC "\\"

#define OP_EQUALS "="
#define OP_HASH "#" // skip everything till newline
#define OP_ANDIF "&&"
#define OP_FD_REF "&" // redirection
#define OP_ORIF "||" 
#define OP_REDOUT ">"
#define OP_REDIN "<"
#define OP_DGREAT ">>"
#define OP_DLESS "<<"
#define OP_PIPE "|"
#define OP_ENV "$"  // this plus underscore or alphanumeric, an env_var
#define OP_STAR "*"
#define OP_SQUOTE "\'"
#define OP_DQUOTE "\"" // backslash retained if followed by [$`"\\n], " requires \ inside ""
#define OP_DLESSDASH "<<-" // heredoc with opt tab trimming (not implemented)
#define OP_CLOBBER ">|" // force overwrite (not implemented)						   

/* Reserved words */
#define WD_IF "if"
#define WD_THEN "then"
#define WD_ELSE "else"
#define WD_FI "fi"
#define WD_DO "do"
#define WD_DONE "done"
#define WD_CASE "case"
#define WD_ESAC "esac"
#define WD_WHIL	"while"
#define WD_UNTL	"until"
#define WD_FOR	"for"
#define WD_IN	"in"

typedef enum {
    TOK_WORD, // command names, built-ins
			  // In the shell command language, a token other than 
			  // an operator. In some cases a word is also a portion 
			  // of a word token: in the various forms of parameter 
			  // expansion, such as ${name-word}, and variable 
			  // assignment, such as name=word, the word is the 
			  // portion of the token depicted by word. The concept 
			  // of a word is no longer applicable following word 
			  // expansions-only fields remain. 
    TOK_ASSIGNMENT_WORD,
    TOK_NAME, // In the shell command language, a word consisting 
			  // solely of underscores, digits, and alphabetics 
			  // from the portable character set. The first character 
			  // of a name is not a digit.
    TOK_NEWLINE,
    TOK_IO_NUMBER, // REDOUT/REDIN plus digits
    TOK_REDIRECT_IN,
    TOK_REDIRECT_OUT,
    TOK_REDIRECT_APPEND,
    TOK_HEREDOC,
    TOK_PIPE,
    TOK_SINGLE_QUOTE,
    TOK_DOUBLE_QUOTE,
    TOK_ENV_VAR,
    TOK_EXIT_STATUS,
    TOK_IF,    // Reserved word "if"
    TOK_THEN,  // Reserved word "then"
    TOK_ELSE,  // Reserved word "else"
    TOK_FI,    // Reserved word "fi"
    TOK_DO,    // Reserved word "do"
    TOK_DONE,  // Reserved word "done"
    TOK_CASE,  // Reserved word "case"
    TOK_ESAC,  // Reserved word "esac"
    TOK_WHILE, // Reserved word "while"
    TOK_UNTIL, // Reserved word "until"
    TOK_FOR,   // Reserved word "for"
    TOK_IN,    // Reserved word "in"
    TOK_EOF,
    TOK_UNKNOWN
} e_tok_type;

typedef enum {
	NORMAL,
	IN_SINGLE_QUOTES,
	IN_DOUBLE_QUOTES
} e_lex_state;

typedef struct s_tok {
	e_tok_type type;
	char *raw;
} t_tok;

typedef t_tok	*(*t_tokenizer)(const char **input); 

t_tok	*tokenize_normal(const char **input);
t_tok	*tokenize_single_quotes(const char **input);
t_tok	*tokenize_double_quotes(const char **input);

typedef struct s_lex {
	e_lex_state	state;
	t_tokenizer	get_next_token;
} t_lex;

t_tok	*create_token(int type, const char *s)
{
	t_tok *token = malloc(sizeof(t_tok));
	if (token)
	{
		token->e_tok_type = type;
		token->raw = strdup(s);
		return (token);
	}
	return (NULL);
}
// Use a hashtable on the tokens where entries identify 100% overlap
// if overlap flag, lookahead and recheck hashtable
// else return word or name using a helper func
t_tok	*_match_char(const char *s, const char *delims)
{
	int i  = 0;
	t_ht_node res = NULL;
	buf = calloc(sizeof(char) * MAX_BUF_SZ);
	if (buf)
	{
		while (s[i] && 0 != is_delim(s[i]))
		{	
			res = ht_lookup(buf);
			if (res)
			{
				if (res->overlap)
					continue;
				else
					break;
			}
			buf[i] = s[i++];
		}
		if (res)
			return (create_token(res->type, buf)
		else
			return (create_token(give_type(buf), buf);
	}
	return (NULL);
}

tokenize_single_quotes(const char **input)
{
	// everything is a literal
	// do spaces delimit? NO
	match_char(input, SINGLE_QUOTE_DELIMS);
}

tokenize_double_quotes(const char **input)
{
	// everything except '$' is a literal, no reserved words
	// do spaces and $ delimit? NO just $ and \
}

tokenize_normal(const char **input)
{
	
}

t_lex	*create_lexer(int start_state)
{
	t_lex *lexer = malloc(sizeof(t_lex));
	if (lexer)
	{
		lexer->state = start_state;
		if (NORMAL == start_state)
			lexer->get_next_token = tokenize_normal;
		if (IN_SINGLE_QUOTES == start_state)
			lexer->get_next_token = tokenize_single_quotes;
		if (IN_DOUBLE_QUOTES == start_state)
			lexer->get_next_token = tokenize_double_quotes;
		return (lexer);
	}
	return (NULL);
}

t_tok	*create_token(int type, const char *s)
{
	t_tok *token = malloc(sizeof(t_tok));
	if (token)
	{
		token->e_tok_type = type;
		token->raw = strdup(s);
		return (token);
	}
	return (NULL);
}

// somehow need to recognize when quotes appear. 
// quotes are a token.



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

