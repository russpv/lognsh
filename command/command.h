// command.h
#ifndef COMMAND_H
# define COMMAND_H

# include "../data_structures/bin_tree.h"
// Note: do not need to include state.h or parse.h

/* COMMAND ADT 
 * Validates AST grammar and sequences execution
 * Uses Strategy pattern to delegate atomic cmds to Execute 
 * does not manage global shell state or env, hist, multi-cmd
 */


typedef struct s_cmd	*t_cmd;
// we need to access global state for expansion and globbing
typedef struct s_state	*t_state; // forward declare instead of include state.h
								  // use wrapper functions in state.h

t_cmd	init_command(char *input);
void	destroy_command(t_cmd c);
void	cmd_execute(t_cmd c); //delegate to Execute. Must search builtins first.

#endif
