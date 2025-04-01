#ifndef LEX_H
# define LEX_H

# include "../builtins/bi.h"
# include "../data_structures/hashtable.h"
# include "../data_structures/llist.h"
# include "../data_structures/stack.h"
# include "../token/token.h"
# include <stdbool.h>

/* Error handling */
// TODO Unmatched symbol pairs should be prompted for more input
// Heredoc Ctrl+D signal: silent abort

# define LEXERKEEP$ 1 // 1 for keeping '$' in expansions

struct s_lex;
typedef struct s_lex	t_lex;

t_lex					*tokenize(t_state *s, const char *input);

void					lex_print(t_lex *lexer);
bool					lex_get_incomplete(t_lex *lexer);
t_list					*lex_get_tokens(t_lex *lexer);
unsigned int			lex_get_keepd(t_lex *lexer);
const char				*lex_get_raw(t_lex *lexer);
char					*lex_get_ptr(t_lex *lexer);
char *lex_get_eof(t_lex *lexer);
int	lex_get_lines(t_lex *lexer);


size_t					ft_varnamelen(const char *c);
int						match_heredoc(t_mem_mgr *m, t_lex *l);

#endif
