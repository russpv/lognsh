#ifndef EXECUTE_INT_H
# define EXECUTE_INT_H

# include "execute.h"
# define _MOD_ "Exec"

/* Forwards */

extern size_t	write_heredoc(int fd, t_lex *l);
extern size_t	read_heredoc(int fd, t_lex *l);

int		handle_exit(t_state *s, int code);

#endif
