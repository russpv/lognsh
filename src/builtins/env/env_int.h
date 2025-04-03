/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_int.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 21:06:33 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 11:30:05 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_INT_H
# define ENV_INT_H

# include "env.h"

# define _MOD_ "env"

// Env variable linked list node
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

char					*env_get_node_key(t_env *node);
char					*env_get_node_value(t_env *node);
t_env					*env_get_node_next(t_env *node);

void					env_set_node_value(t_mem_mgr *m, t_env *node, \
							const char *value);
void					env_set_node_next(t_env *node, t_env *next);

#endif
