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
# define EMSG_OPT "No options provided. Sorry!\n"
# define EMSG_ARGC "No args for you! (Not implemented)\n"

int					bi_echo(t_state *s, char **args, int argc);
int					bi_pwd(t_state *s, char **args, int argc);
int					bi_env(t_state *s, char **argv, int argc);
int					bi_export(t_state *s, char **argv, int argc);
int					bi_unset(t_state *s, char **argv, int argc);
int					bi_cd(t_state *s, char **args, int argc);
int					bi_exit(t_state *s, char **args, int argc);
int					bi_exec(t_state *s, char **args, int argc);

//int	env_set_value(t_env **env_list, const char *key, const char *value);
t_env	*find_env_key(t_env *env_list, const char *key);
int	update_existing_var(t_mem_mgr *m, t_env *existing_key, const char *value);


// export helper functions (bi_export_do 1 & 2)
int	process_arg_update_add(t_state *s, const char *arg, \
	char *equal_pos, int *error_occurred);
int					parse_arg(t_state *s, const char *arg, \
						int *error_occurred);

/* This ensures block buffer is flushed during execution
 * as a background process
 */
void				flushbuf(void);

#endif
