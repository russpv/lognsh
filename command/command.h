// command.h
#ifndef COMMAND_H
# define COMMAND_H

# include "../data_structures/bin_tree.h"
// Note: do not need to include state.h

/* COMMAND ADT 
** represents single entered command's state and lifecycle
** links to token, parsing, execution
** does not manage global shell state or env, hist, multi-cmd
*/

// TODO bonus globbing and var expansion in parse_command()

typedef struct s_cmd	*t_cmd;
// we need to access global state for expansion and globbing
typedef struct s_state	*t_state; // forward declare instead of include state.h
								  // use wrapper functions in state.h

t_cmd	init_command(char *input);
void	destroy_command(t_cmd c);

// using integer return values to manage return scenarios
int	tokenize_command(t_cmd c); // use btree and token to create AST
int	parse_command(t_state s, t_cmd c); // test grammar: do globbing, var expansion
									   // access ENV methods via t_state 
void	exec_command(t_cmd c);

#endif
