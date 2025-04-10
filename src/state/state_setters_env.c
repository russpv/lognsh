/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_setters_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:34:58 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:34:59 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

int	set_oldpwd(t_state *s, const char *caller)
{
	char	*old_pwd;

	old_pwd = get_pwd(s);
	if (!old_pwd || 0 != env_upsert_value(get_mem(s), get_env_list(s),
			OLDPWD_KEY, old_pwd))
	{
		print_custom_err(caller, "Corrupted working directory.\n");
		return (ERR_GENERAL);
	}
	return (0);
}

void	set_pwd(t_state *s)
{
	t_env	*lst;
	char	*pwd;
	char	*tmp;
	char	*tmp2;

	if (!s)
		return ;
	lst = get_env_list(s);
	if (!lst)
		return ;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("Cannot retrieve working directory\n"));
	tmp = ft_strdup_tmp(get_mem(s), pwd);
	if (!tmp)
		exit_clean(&get_mem(s)->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	free(pwd);
	env_upsert_value(get_mem(s), lst, CWD_KEY, tmp);
	tmp2 = ft_strdup_tmp(get_mem(s), tmp);
	s->pwd = tmp2;
	dprint("Set pwd:%s\n", s->pwd);
}

// Updates internal PATH with string delimited by :
void	set_path(t_state *s, const char *value)
{
	struct s_mem_utils	mem;

	if (!s)
		return ;
	mem_struct_init(get_mem(s), &mem);
	dprint(_MOD_ ": setting path\n");
	if (NULL != s->path)
		ft_freearr_mem(mem.head, mem.u, (void **)s->path, -1);
	dprint(_MOD_ ": got path:%s\n", value);
	if (value)
		s->path = ft_split_mem(&mem, value, ':');
	else
		s->path = NULL;
}
