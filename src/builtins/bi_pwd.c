/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:02:05 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/16 15:15:55 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define CMD_NAME "pwd"

const char	*env_find_value(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

int	bi_pwd(t_state *s, char **args, int argc)
{
	const char	*pwd_value;
	char		*cwd;

	(void)args;
	(void)argc;
	pwd_value = env_find_value(s->sh_env_list, "PWD");
	if (pwd_value)
	{
		write(STDOUT_FILENO, pwd_value, ft_strlen(pwd_value));
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		print_perror(CMD_NAME);
		return (1);
	}
	write(STDOUT_FILENO, cwd, ft_strlen(cwd));
	write(STDOUT_FILENO, "\n", 1);
	free(cwd);
	return (0);
}
