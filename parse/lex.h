#ifndef LEX_H
# define LEX_H

#include "../data_structures/ast.h"
#include "../data_structures/llist.h"
#include "../data_structures/hashtable.h"
#include "../state/state.h"
#include "token.h"
#include "../debug.h"
#include <stdbool.h>

/* delimiter handling */
// single vs double quotes
// preserve literal strings

/* Error handling */
// Undefined variables cannot be expanded
// Invalid globbing pattern
// Unmatched quotation marks?

struct s_lex;
typedef struct s_lex t_lex;

t_lex	*create_lexer(int start_state, const char *s);
void	destroy_lexer(t_lex *lexer);
t_lex	*tokenize(const char *input);
void	lex_print(t_lex *lexer);

#endif
