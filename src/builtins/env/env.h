/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:57:34 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/16 15:16:06 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ENV_H
# define ENV_H

# include "../../../include/libft.h"
# include "../../data_structures/hashtable.h"

# define SHELL_PATH "PATH"

struct					s_env;
typedef struct s_env	t_env;

// linked list/node creation functions (env_llist.c)
t_env				*create_env_node(const char *key, const char *value);
void				add_env_node(t_env **env, t_env *new_node);
void				remove_env_node(t_env **env, const char *key);
t_env				*copy_envp(char **envp);	

// convert list to array for execve (env_convert_utils.c)
char				**lst_to_array(t_env *env_list);

// used by destroy state to free list (env_utils.c)
void				env_free_list(t_env *env);
// print env list in bi_env (env_utils.c)
void				env_print(t_env *env);

// env getters
// gets the PATH environment var as an array for cmd_run.c
char				**env_getenv(void);
// gets value of an environment variable from linked list
char				*env_getenv_value(const char *key, t_env *env);
char				*env_get_key(t_env *node);
char				*env_get_value(t_env *node);
t_env				*env_get_next(t_env *node);

// env setters
void				env_set_node_value(t_env *node, \
						const char *value);
void				env_set_next(t_env *node, t_env *next);
#endif
