/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_int.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 21:06:33 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/09 19:35:32 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_INT_H
# define ENV_INT_H

# include "env.h"

// Env variable linked list node
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

// internal helper functions
t_env				*create_env_node(const char *key, const char *value);
void				add_env_node(t_env **env, t_env *new_node);
void				remove_env_node(t_env **env, const char *key);
int					extract_key_value(const char *env_str, char **key,
						char **value);
int					get_env_size(char **envp);
int					get_env_list_size(t_env *env_list);

// to convert list to array
void				copy_key_value(char *dest, const char *key,
						const char *value);
char				*malloc_pair_string(const char *key, const char *value);
char				**alloc_env_array(t_env *env_list);
void				free_env_array(char **env_array);

#endif
