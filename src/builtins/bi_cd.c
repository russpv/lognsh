/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:07:02 by dayeo             #+#    #+#             */
/*   Updated: 2025/01/28 09:14:42 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define ERRMSG_OLDPWDNOTSET "OLDPWD not set.\n"
#define ERRMSG_ARGC "No such file or directory.\n"
#define ERRMSG_INVLD "invalid state or arguments\n"
#define ERRMSG_TOOMANYARGS "too many arguments.\n"
#define ERRMSG_NOHOME "HOME not set.\n"
#define ERRMSG_BADMALLOC "memory allocation failed.\n"

static int	env_set_value(t_env **env_list, const char *key, const char *value)
{
	t_env	*node;

	if (!env_list || !key)
		return (1);
	node = env_find_key(*env_list, key);
	if (node)
	{
		if (!_update_existing_var(node, value))
			return (1);
	}
	else
	{
		node = create_env_node(key, value);
		if (!node)
			return (1);
		add_env_node(env_list, node);
	}
	return (0);
}

static int	change_dir(t_state *s, const char *target)
{
	char	*new_dir;
	char	*old_pwd;

	if (ft_strcmp(target, "-") == 0)
	{
		new_dir = env_getenv_value("OLDPWD", get_sh_env_list(s));
		if (!new_dir || ft_strcmp(new_dir, "") == 0)
			return (print_custom_err("cd", ERRMSG_OLDPWDNOTSET), 1);
	}
	else
		new_dir = (char *)target;
	if (chdir(new_dir) != 0)
	{
		print_custom_err("cd", ERRMSG_ARGC);
		return (1);
	}
	old_pwd = get_pwd(s);
	if (old_pwd && env_set_value(get_sh_env_list_add(s), \
		"OLDPWD", old_pwd) != 0)
	{
		print_custom_err("cd", ERRMSG_BADMALLOC);
		return (1);
	}
	free(old_pwd);
	return (0);
}

int	bi_cd(t_state *s, char **args, int argc)
{
	char	*target;

	if (!s || !args)
	{
		print_custom_err("cd", ERRMSG_INVLD);
		return (1);
	}
	if (argc == 1)
		target = env_getenv_value("HOME", get_sh_env_list(s));
	else if (argc == 2)
		target = args[1];
	else
	{
		print_custom_err("cd", ERRMSG_TOOMANYARGS);
		return (1);
	}
	if (!target || ft_strcmp(target, "") == 0)
	{
		print_custom_err("cd", ERRMSG_NOHOME);
		return (1);
	}
	return (change_dir(s, target));
}
