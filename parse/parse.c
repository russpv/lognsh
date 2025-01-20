#include "../data_structures/llist.h"
#include "parse.h"
#include <stdbool.h>

#define FAIL_TOKEN 10 // TODO group somewhere else

/* chars that need to be quoted if meant literally */
#define SPECCHARS "#$^*()=|{}[]`<>?~;&\n\t \\\'\""
#define LEX_BUFSZ 1024
#define DO_GLOBBING 2
#define DO_EXPANSION 2
#define RESET 0

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
// Be careful of spaces var="name with space" will expand to a list,
//	not a string
// bash doesn't allow this to expand to two arguments: var='a "b c"',
//	just 1 or 3
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

t_ast	parse(t_state s, char *input)
{
	t_lex	*lexer;

	// work with example: [builtcmd 'arg' && cmd -arg -arg "$VAR" > fn]
	lexer = tokenize(input);
	if (NULL == lexer)
		return (null_and_stat(s, ERR_TOKEN));
	// TODO make AST and parse
	// create_ast(lexer);
	return (NULL);
}
void	token_print(t_tok *token)
{
	printf("Token: %s, Type: %d\n", token->raw, token->type);
}

void	lexer_print(t_lex *lexer)
{
	ft_lstiter(lexer->token_list, token_print)
}

// For debugging
void	parse_print(t_ast ast)
{
	// TODO
	// ast_traverse
}
