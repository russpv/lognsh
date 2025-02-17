/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:56:51 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/16 15:18:15 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BI_H
# define BI_H

# include "../../include/libft.h"
# include "../debug.h"
# include "../state/state.h"
# include "../state/state_int.h"  //Don't like this coupling
# include "../env/env.h"
# include "../env/env_int.h" //Don't like this coupling
# include <unistd.h>
# include <stdio.h>

/* These are indexed in the lexer */
# define BI_ECHO "echo"
# define BI_CD "cd"
# define BI_PWD "pwd"
# define BI_EXPORT "export"
# define BI_UNSET "unset"
# define BI_ENV "env"
# define BI_EXIT "exit"
# define BI_EXEC "exec"
# define BI_COUNT 8

typedef int	(*t_builtin_fn)(t_state *s, char **args, int argc);

t_builtin_fn	get_builtin(char *command);

#endif
