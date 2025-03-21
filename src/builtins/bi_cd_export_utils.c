/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd_export_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 10:47:28 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/13 13:10:56 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define EMSG_BADMALLOC "memory allocation failed.\n"

// Returns 0 if error.
// updates an existing environment variable's value
int	update_existing_var(t_mem_mgr *m, t_env *existing_key, const char *value)
{
	char	*new_value;

	new_value = NULL;
	if (!existing_key)
		return (0);
	if (value)
		new_value = ft_strdup(value);
	else
		new_value = ft_strdup("");
	if (!new_value)
	{
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	}
	env_set_node_value(m, existing_key, new_value);
	free(new_value);
	return (1);
}

t_env	*find_env_key(t_env *env_list, const char *key)
{
	t_env	*current;

	if (!env_list || !key)
		return (NULL);
	current = env_list;
	while (current)
	{
		if (ft_strcmp(env_get_key(current), key) == 0)
			return (current);
		current = env_get_next(current);
	}
	return (NULL);
}
