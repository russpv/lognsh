/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:32:35 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/01 01:49:10 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Low-level implementation of forking, piping, redirects
#ifndef EXECUTE_H
# define EXECUTE_H

# include "../builtins/bi.h"
# include "../state/state.h"
# include "../command/command.h"
# include <sys/wait.h>
# include <sys/types.h>

//# include "../parse/parse.h"

/* Duplicate declares */
typedef struct s_node			t_ast_node;

typedef int (*execute_fn)(t_state *s, t_ast_node *node);

int	redirect(int *to, char *topath, int from, t_bool ifappend);

/* Atomic commands */
int	exec_fork_execve(t_state *s);
int	exec_bi_call(t_state *s, t_builtin_fn bi);

/* Higher level commands */
int exec_fork_func(t_state *s, t_ast_node *node, execute_fn executor);

#endif
