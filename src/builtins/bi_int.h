/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_int.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:57:34 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/16 15:16:06 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BI_INT_H
# define BI_INT_H

# include "bi.h"

int					bi_echo(t_state *s, char **args, int argc);
int					bi_pwd(t_state *s, char **args, int argc);
int					bi_cd(t_state *s, char **args, int argc);
int					bi_exit(t_state *s, char **args, int argc);
int					bi_env(t_state *s, char **argv, int argc);
int					bi_export(t_state *s, char **argv, int argc);
int					bi_unset(t_state *s, char **argv, int argc);
int					bi_exec(t_state *s, char **args, int argc);

// export helper functions
t_env				*env_find_key(t_env *env_list, const char *key);
int					is_valid_key(const char *key);
int					validate_key(const char *key);
int					handle_underscore(const char *key);
int					handle_no_equal(const char *arg, int *error_occurred);
int					update_existing_var(t_env *existing_key, const char *value);
int					add_new_var(t_env **sh_env_list, const char *key, \
					const char *value);
int					process_arg_validation(const char *arg, char **equal_pos, \
						int *error_occurred);
int					process_arg_update_add(t_state *s, const char *arg, \
						char *equal_pos, int *error_occurred);
int					process_arg(t_state *s, const char *arg, \
						int *error_occurred);

// unset helper function
void				env_remove_key(t_env **env_list, const char *key);

// pwd helper function
const char			*env_find_value(t_env *env_list, const char *key);

// exit helper functions
int					is_numeric(const char *str);
void				handle_no_arg(t_state *s);
void				handle_invalid_arg(t_state *s);
void				handle_too_many_args(t_state *s);
void				handle_valid_arg(t_state *s, char *arg);

/* This ensures block buffer is flushed during execution
 * as a background process
 */
void				flushbuf(void);

#endif
