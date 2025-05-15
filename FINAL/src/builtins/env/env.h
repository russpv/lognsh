/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:26:17 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:26:18 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "../../data_structures/hashtable.h"
# include "../../mem/mem.h"
# include "../../state/error.h"
# include <errno.h>

typedef struct s_env	t_env;

t_env					*create_env_node(t_mem_mgr *m, const char *key,
							const char *value);
t_env					*copy_envp(t_mem_mgr *m, char **envp);
void					env_free_list(t_mem_mgr *m, t_env *env);
void					env_add_node(t_env **env, t_env *new_node);
void					env_remove_node(t_mem_mgr *m, t_env **env,
							const char *key);

char					*env_find_value(const char *key, t_env *env);
t_env					*env_find_node(t_env *env_list, const char *key);
int						env_upsert_value(t_mem_mgr *m, t_env *env_list,
							const char *key, const char *value);

void					env_print(t_env *env);
char					**env_to_array(t_mem_mgr *m, t_env *env_list);
char					**env_getenv_path(t_mem_mgr *m);
#endif
