#ifndef LEX_H
# define LEX_H

# include "../../debug.h"
# include "../builtins/builtins.h"
# include "../data_structures/ast.h"
# include "../data_structures/hashtable.h"
# include "../data_structures/llist.h"
# include "../state/state.h"
# include "../token/token.h"
# include <stdbool.h>

/* Error handling */
// TODO Unmatched symbol pairs should be prompted for more input
// Heredoc Ctrl+D signal: silent abort

struct s_lex;
typedef struct s_lex	t_lex;

t_lex					*create_lexer(int start_state, const char *s);
void					destroy_lexer(t_lex *lexer);
t_lex					*tokenize(const char *input);
void					lex_print(t_lex *lexer);
bool					lex_get_incomplete(t_lex *lexer);
t_lex	*tokenize_more_input(t_lex *lexer); // TODO
t_list					*lex_get_tokens(t_lex *lexer);

#endif
