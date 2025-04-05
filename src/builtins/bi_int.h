/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_int.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:57:34 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/19 14:59:07 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BI_INT_H
# define BI_INT_H

# include "bi.h"

# define _MOD_ "Bi"
# define EMSG_OPT "No options provided. Sorry!"
# define EMSG_ARGC "No args for you! (Not implemented)"

int		bi_echo(t_state *s, char **args, int argc);
int		bi_pwd(t_state *s, char **args, int argc);
int		bi_env(t_state *s, char **argv, int argc);
int		bi_export(t_state *s, char **argv, int argc);
int		bi_unset(t_state *s, char **argv, int argc);
int		bi_cd(t_state *s, char **args, int argc);
int		bi_exit(t_state *s, char **args, int argc);
int		bi_exec(t_state *s, char **args, int argc);

int		process_arg_update_add(t_state *s, const char *arg, char *equal_pos,
			int *error_occurred);
int		parse_arg(t_state *s, const char *arg, int *error_occurred);

/* This ensures block buffer is flushed during execution
 * as a background process
 */
void	flushbuf(void);

#endif
