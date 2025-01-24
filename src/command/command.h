// command.h
#ifndef COMMAND_H
# define COMMAND_H

# include "../data_structures/b_tree.h"
// Note: do not need to include state.h or parse.h

/* COMMAND ADT 
 * Validates AST grammar and sequences execution
 * Uses Strategy pattern to delegate atomic cmds to Execute 
 * does not manage global shell state or env, hist, multi-cmd
 */

/*
Bash Manual

If the command name contains no slashes, the shell attempts to locate it. If there exists a shell function by that name, that function is invoked as described in Shell Functions.
If the name does not match a function, the shell searches for it in the list of shell builtins. If a match is found, that builtin is invoked.
If the name is neither a shell function nor a builtin, and contains no slashes, Bash searches each element of $PATH for a directory containing an executable file by that name. Bash uses a hash table to remember the full pathnames of executable files to avoid multiple PATH searches (see the description of hash in Bourne Shell Builtins). A full search of the directories in $PATH is performed only if the command is not found in the hash table. If the search is unsuccessful, the shell searches for a defined shell function named command_not_found_handle. If that function exists, it is invoked in a separate execution environment with the original command and the original command’s arguments as its arguments, and the function’s exit status becomes the exit status of that subshell. If that function is not defined, the shell prints an error message and returns an exit status of 127.
If the search is successful, or if the command name contains one or more slashes, the shell executes the named program in a separate execution environment. Argument 0 is set to the name given, and the remaining arguments to the command are set to the arguments supplied, if any.
If this execution fails because the file is not in executable format, and the file is not a directory, it is assumed to be a shell script and the shell executes it as described in Shell Scripts.
If the command was not begun asynchronously, the shell waits for the command to complete and collects its exit status. 

*/

typedef struct s_cmd	*t_cmd;
// we need to access global state for expansion and globbing
typedef struct s_global_state	t_state; // forward declare instead of include state.h
								  // use wrapper functions in state.h

t_cmd	init_command(char *input);
void	destroy_command(t_cmd c);
void	cmd_execute(t_cmd c); //delegate to Execute. Must search builtins first.

#endif
