/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 09:07:02 by dayeo             #+#    #+#             */
/*   Updated: 2025/03/19 14:55:23 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

#define CMD_NAME "cd"
#define EMSG_OLDPWDNOTSET "OLDPWD not set.\n"
#define EMSG_INVLD "invalid state or arguments\n"
#define EMSG_NOFILE "No such file or directory\n"
#define EMSG_TOOMANYARGS "too many arguments.\n"
#define EMSG_NOHOME "HOME not set.\n"
#define EMSG_BADMALLOC "memory allocation failed.\n"

static int	_set_env_value(t_mem_mgr *m, t_env **env_list, const char *key, const char *value)
{
	t_env	*node;

	if (!env_list || !key)
		return (1);
	node = find_env_key(*env_list, key);
	if (node)
	{
		if (!update_existing_var(m, node, value))
			return (1);
	}
	else
	{
		node = create_env_node(m, key, value);
		if (!node)
			return (1);
		env_add_node(env_list, node);
	}
	return (0);
}

// Wraps chdir()
static int	_change_dir(t_state *s, const char *target)
{
	char	*new_dir;
	char	*old_pwd;

	if (0 == ft_strcmp(target, "-"))
	{
		new_dir = env_getenv_value("OLDPWD", get_sh_env_list(s));
		if (!new_dir || ft_strcmp(new_dir, "") == 0)
			return (print_custom_err(CMD_NAME, EMSG_OLDPWDNOTSET), 1);
	}
	else
		new_dir = (char *)target;
	if (0 != chdir(new_dir))
	{
		print_custom_err_err(CMD_NAME, target, EMSG_NOFILE);
		return (ERR_GENERAL);
	}
	old_pwd = get_pwd(s);
	if (old_pwd && 0 != _set_env_value(get_mem(s), get_sh_env_list_add(s), \
		"OLDPWD", old_pwd))
	{
		print_custom_err(CMD_NAME, EMSG_BADMALLOC);
		return (ERR_GENERAL);
	}
	myfree(&get_mem(s)->list, old_pwd);
	return (0);
}

/* Does what it says on the tin */
int	bi_cd(t_state *s, char **args, int argc)
{
	char	*target;

	if (!s || !args)
	{
		print_custom_err(CMD_NAME, EMSG_INVLD);
		return (ERR_ARGS);
	}
	if (argc == 1)
		target = env_getenv_value("HOME", get_sh_env_list(s));
	else if (argc == 2)
		target = args[1];
	else
	{
		print_custom_err(CMD_NAME, EMSG_TOOMANYARGS);
		return (ERR_ARGS);
	}
	if (!target || ft_strcmp(target, "") == 0)
	{
		print_custom_err(CMD_NAME, EMSG_NOHOME);
		return (ERR_GENERAL);
	}
	return (_change_dir(s, target));
}
