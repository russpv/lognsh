#include "parse.h"
#include "../data_structures/llist.h"
#include <stdbool.h>

# define FAIL_TOKEN 10 //TODO group somewhere else

/* chars that need to be quoted if meant literally */
#define SPECCHARS "#$^*()=|{}[]`<>?~;&\n\t \\\'\""
#define LEX_BUFSZ 1024
#define DO_GLOBBING 2
#define DO_EXPANSION 2
#define	RESET 0

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
    TOK_IO_NUMBER, // REDOUT/REDIN plus digits OVERLAP
    TOK_REDIRECT_IN,//OVERLAP
    TOK_REDIRECT_OUT, //OVERLAP
    TOK_REDIRECT_APPEND, //OVERLAP
    TOK_HEREDOC, //OVERLAP
    TOK_PIPE, //OVERLAP
	TOK_ANDIF,
	TOK_ORIF, //OVERLAP
    TOK_SINGLE_QUOTE,
    TOK_DOUBLE_QUOTE,
    TOK_ENV_VAR, //$
    TOK_EXIT_STATUS, //$?
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
    TOK_UNKNOWN,
	TOK_ERR
} e_tok_type;

typedef enum {
	NORMAL,
	IN_SINGLE_QUOTES,
	IN_DOUBLE_QUOTES,
	DONE
} e_lex_state;

typedef struct s_tok {
	e_tok_type type;
	char *raw;
	size_t	pos;
} t_tok;

typedef t_tok	*(*t_tokenizer)(const char **input); 

t_tok	*tokenize_normal(const char **input);
t_tok	*tokenize_single_quotes(const char **input);
t_tok	*tokenize_double_quotes(const char **input);
t_tok	*tokenize_escape(const char **input);

typedef struct s_lex {
	const char 	*raw_string;
	char 		*ptr;
	e_lex_state	state;
	bool		escape_mode;
	t_tokenizer	get_next_token;
	t_list	**token_list;
	char	*buf;
	int		do_expansion;
	int		do_globbing;
} t_lex;

int	_do_state_transition(t_lex *lexer)
{
	if (!lexer->ptr || !*(++lexer->ptr))
		lexer->state = DONE;
	if (OP_DQUOTE == lexer->ptr)
	{
		lexer->state = IN_DOUBLE_QUOTES;
		lexer->tokenizer = tokenize_double_quotes;
	}
	else if (OP_SQUOTE == lexer->ptr)
	{
		lexer->state = IN_SINGLE_QUOTES;
		lexer->tokenizer = tokenize_single_quotes;
	}
	else
	{
		lexer->state = NORMAL;
		lexer->tokenizer = tokenize_normal;
	}
	return (0);
}

/* Expected to add only one token to the llist */
int	tokenize_single_quotes(t_lex *lexer)
{
	const char *start = lexer->ptr;

	while (++lexer->ptr)
	{
		if ((unsigned char)OP_SQUOTE == lexer->ptr)
			break;
		else
			lexer->buf[lexer->ptr - start - 1] = lexer->ptr;
	}
	if ((unsigned char)OP_NULL == lexer->ptr)
		return (1);
	else
		add_token(lexer, TOK_WORD, start);
	return (0);
}

/* Returns next token, doesn't flush buf
 * if $, record it in the token record for later expansion
 * if bs, skip it if next char is bs, dollar, double quote or backtick
 */
t_tok	*_match_double(t_lex *lexer)
{
	const char *start = lexer->ptr;
	t_tok	*token = NULL;

	if (lexer->ptr)
	{
		while (++lexer->ptr)
		{
			if ((unsigned char)TK_ESC == lexer->ptr)
			{
				if ((unsigned char)OP_BACKSLASH == lexer->ptr + 1 || \
					(unsigned char)OP_ENV == lexer->ptr + 1 || \
					(unsigned char)OP_DQUOTE == lexer->ptr + 1 || \
					(unsigned char)OP_BACKTICK == lexer->ptr + 1)
				{
					lexer->escape_mode = true;
					++lexer->ptr;
				}
			}
			if ((unsigned char)OP_ENV == lexer->ptr && false == lexer->escape_mode)
				lexer->do_expansion = DO_EXPANSION;
			if ((unsigned char)OP_DQUOTE == lexer->ptr && false == lexer->escape_mode)
				return(create_token(lexer, TOK_WORD));
			lexer->escape_mode = false;
			lexer->buf[lexer->ptr - start - 1] = lexer->ptr;
		}
	}
	return (token);
}

/* Expected to add only one token to the llist */
int	tokenize_double_quotes(t_lex *lexer)
{
	/* search rest of string until " found else err
	 * if $, record it in the token record for later expansion
	 * if bs, skip it if next char is bs, dollar, double quote or backtick
	 * everything else is not a delimiter
	 */
	if (lexer)
	{
		t_tok *token = _match_double(lexer);
		if (!token)
			return (1);
		if (0 != add_token(token))
			return (1);
	}
	return (0);
}

int	_word_or_name(const char *s)
{
	if (!*s)
		return (TOK_ERR);
	while (*s)
	{
		if (!(ft_isalnum(*s) || '_' == *s))
			return (TOK_WORD);
		s++;
	}
	return (TOK_NAME);
}


/* Looks one char ahead to detect matches 
 * that are superstrings of substrings, e.g. '||' vs '|'
 * Updates lexer state if successful
 */
t_ht_node	_do_one_char_lookahead(t_lex *lexer, t_ht_node res)
{
	const size_t buflen = ft_strlen(lexer->buf);
	char *s;

	s = lexer->ptr;
	if (!lexer->ptr || !*(s + 1) || 0 == buflen)
		return (NULL);
	lexer->buf[buflen] = *(s + 1);
	test = ht_lookup(lexer->buf);
	if (test)
	{
		lexer->ptr++;
		return (test);
	}
	else	
		lexer->buf[buflen] = 0;
	return (res);
}

// Use a hashtable on the tokens where entries identify 100% overlap
// if overlap flag, 1-char lookahead and recheck hashtable
// else return word or name using a helper func
// modifies input string location
t_tok	*_match_normal(t_lex *lexer)
{
	int i  = 0;
	t_ht_node res = NULL;
	const char *start = lexer->ptr;
	char *s = lexer->ptr;

	while (' ' == *s)
		s++;
	while (*s && false == is_normal_delim(*s))
	{	
		if ((unsigned char)TK_ESC == *s)
		{
			lexer->escape_mode = true;
			++s;
		}
		if ((unsigned char)OP_ENV == lexer->ptr && false == lexer->escape_mode)
			lexer->do_expansion = DO_EXPANSION;
		if ((unsigned char)OP_STAR == lexer->ptr && false == lexer->escape_mode)
			lexer->do_globbing = DO_GLOBBING;
		lexer->escape_mode = false;
		lexer->buf[lexer->ptr - start - 1] = *s++;
		res = ht_lookup(lexer->buf);
		if (res)
		{
			if (true == res->is_substring)
				res = _do_one_char_lookahead(lexer, res);	
			return (create_token(lexer, res->type);
		}
	}
	if (true == is_normal_delim(*s))
		return (create_token(lexer, word_or_name(lexer->buf)));
	if ((unsigned char)TOK_EOF == *s)
		return (create_token(lexer, TOK_EOF));
	return (NULL);
}

/* Expected to add multiple tokens to the llist */
int	tokenize_normal(t_lex *lexer)
{
	const char *start = lexer->ptr;

	while (lexer->ptr && !is_transition_char(lexer->ptr))
	{
		t_tok *token = _match_normal(lexer)
		if (token)
			if (0 != add_token(lexer, token))
				return (1);
		if (token && token->type == TOK_EOF)
			return (0);
		else if (!token)
			err();
	}
	return (0);
}

/* add to llist tail a new token, clear buf */
int	add_token(t_lex *lexer, t_tok *token)
{
	if (token && lexer)
	{
		memset(lexer->buf, 0, LEX_BUFLEN);
		ft_lst_addback(lexer->token_list, token);
		return (0);
	}
	return (1);
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
		if (IN_ESCAPE == start_state)
			lexer->get_next_token = tokenize_escape;
		lexer->buf = malloc(sizeof(char) * LEX_BUFLEN);
		if (!lexer->buf)
		{
			free(lexer);
			lexer = NULL;
		}
	}
	return (lexer);
}

void	destroy_lexer(t_lex *lexer)
{
	if (!lexer)
		return ;
	free(lexer->buf);
	ft_lstclear(lexer->token_list, destroy_token);
	free(lexer);
	lexer = NULL;
}

t_tok	*create_token(t_lex *lexer, int type)
{
	if (!lexer)
		return (NULL);
	t_tok *token = malloc(sizeof(t_tok));
	if (token)
	{
		token->e_tok_type = type;
		token->raw = strdup(lexer->buf);
		token->pos = (size_t)(lexer->ptr - lexer->raw_string);
		if (DO_GLOBBING == lexer->do_globbing)
			token->do_globbing = DO_GLOBBING;
		if (DO_EXPANSION == lexer->do_expansion)
			token->do_expansion = DO_EXPANSION;
		lexer->do_globbing = RESET;
		lexer->do_expansion = RESET;
		if (!token->raw)
		{
			free(token);
			token = NULL;
		}
	}
	return (token);
}

/* void star due to linked list destroy method */
void	destroy_token(void *token)
{
	if (!token)
		return ;
	free(token->raw);
	free(token);
	token = NULL;
}

// assumes non-empty string
t_lex	*tokenize(char *input)
{
	e_lex_state state;

	if ((unsigned char)OP_SQUOTE == *input)
		state = IN_SINGLE_QUOTES;
	else if ((unsigned char)OP_DQUOTE == *input)
		state = IN_DOUBLE_QUOTES;
	else
		state = NORMAL;
	lexer = create_lexer(state);
	if (lexer)
	{
		while (DONE != lexer->state)
		{
			if (1 == lexer->tokenizer(lexer))
			{
				destroy_lexer(lexer);
				break ;
			}
			_do_state_transition(lexer);
		}
	}
	return (lexer);
}

t_ast	parse(t_state s, char *input)
{
	// work with example: [builtcmd 'arg' && cmd -arg -arg "$VAR" > fn]
	t_lex *lexer = tokenize(input);
	if (NULL == lexer)
		return (null_and_stat(s, ERR_TOKEN));
	// TODO make AST and parse
	// create_ast(lexer);
	return (NULL);
}

// For debugging
void	parse_print(t_ast ast)
{
	//TODO
	// ast_traverse
}

