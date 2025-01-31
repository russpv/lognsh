/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:32:35 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/27 17:17:26 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Low-level implementation of forking, piping, redirects
#ifndef EXECUTE_H
# define EXECUTE_H

# include "../builtins/bi.h"
# include "../state/state.h"
# include "../command/command.h"
//# include "../parse/parse.h"

// typedef struct s_node	t_ast_node;

int	redirect(int *to, char *topath, int from, t_bool ifappend);
int	fork_and_run(t_state *s);

#endif
