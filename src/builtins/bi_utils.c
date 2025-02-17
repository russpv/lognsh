/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 09:17:53 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/16 15:15:48 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define CMD_NAME "export"
#define ERRMSG_KEY_NOTVALID "not a valid identifier\n"

/* Returns 1 if key is alphanumeric */
int	is_valid_key(const char *key)
{
	int	i;

	i = 0;
	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	validate_key(const char *key)
{
	if (!is_valid_key(key))
	{
		print_custom_err(CMD_NAME, ERRMSG_KEY_NOTVALID);
		return (0);
	}
	return (1);
}

t_env	*env_find_key(t_env *env_list, const char *key)
{
	t_env	*current;

	if (!env_list || !key)
		return (NULL);
	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

/* RKP: is this arch specific? */
int	handle_underscore(const char *key)
{
	if (key[0] == '_')
	{
		write(STDERR_FILENO, "export: '", 9);
		write(STDERR_FILENO, key, ft_strlen(key));
		write(STDERR_FILENO, \
			"': child process already inherit env variable\n", 46);
		return (0);
	}
	return (1);
}

/* RKP: lack of equals doesn't throw err */
int	handle_no_equal(const char *arg, int *error_occurred)
{
	write(STDERR_FILENO, "export: '", 9);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, "': not a valid key=value pair\n", 31);
	*error_occurred = 1;
	return (0);
}
