/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_simple_run_path.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:29:07 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:29:08 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_int.h"

/* Note: command not found is thrown in caller */
// Checks permissions on path and also if a dir
static int	_check_access(const char *path)
{
	struct stat	info;

	if (0 == access(path, F_OK))
	{
		if (0 != access(path, X_OK))
			return (print_perror(path), ERR_CMD_NOT_EXEC);
		if (0 == stat(path, &info))
			if (S_ISDIR(info.st_mode))
				return (print_is_dir((char *)path), ERR_CMD_IS_A_DIR);
		return (0);
	}
	return (ERR_CMD_NOT_FOUND);
}

/* Returns 0 if absolute fullpath is found
 * All heap allocs are temporary within this scope
 */
static int	_search_path(t_state *s, const char *cmd, char **fullpath)
{
	char	**paths;
	char	*tmp;
	int		i;

	i = -1;
	paths = get_path(s);
	if (NULL == paths)
		return (ERR_CMD_NOT_FOUND);
	while (paths[++i])
	{
		tmp = ft_strjoin_mem(&get_mem(s)->list, get_mem(s)->f, paths[i], "/");
		if (NULL == tmp)
			exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		*fullpath = ft_strjoin_mem(&get_mem(s)->list, get_mem(s)->f, tmp, cmd);
		if (!*fullpath)
			exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		(get_mem(s))->dealloc(&get_mem(s)->list, tmp);
		if (0 == _check_access(*fullpath))
			return (0);
		(get_mem(s))->dealloc(&get_mem(s)->list, *fullpath);
		*fullpath = NULL;
	}
	return (ERR_CMD_NOT_FOUND);
}

static int	_do_search(t_state *s, const char *name, char **fullpath)
{
	int	res;

	res = 0;
	if (ft_strchr(name, '/'))
	{
		res = _check_access(name);
		if (0 == res)
		{
			*fullpath = (char *)name;
			return (0);
		}
	}
	else
	{
		res = _search_path(s, name, fullpath);
		if (0 == res)
		{
			res = _check_access(*fullpath);
			if (0 == res)
				return (0);
		}
	}
	return (res);
}

/* Stores command path in fullpath, if valid.
 * Checks PATH, or absolute path if slash is in the name */
int	find_and_validate_cmd(t_state *s, const char *name, char **fullpath)
{
	int	res;

	res = ERR_CMD_NOT_FOUND;
	if (NULL != name && '\0' != name[0])
	{
		res = _do_search(s, name, fullpath);
	}
	if (ERR_CMD_NOT_FOUND == res)
		print_command_not_found(name);
	return (res);
}
