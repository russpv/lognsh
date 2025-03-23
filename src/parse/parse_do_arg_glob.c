#include "parse_int.h"

#define EMSG_OPENDIR "ERR opendir()\n"
#define DBGMSG_GOTARGS _MOD_ ": p_do_globbing_args - arg: %s, lst:%p\n"
#define DBGMSG_MATCHES _MOD_ ": p_do_globbing_args found %d matches, 1st: %s\n"
#define DBGMSG_GOTPATTERN _MOD_ ": match_glob got pattern: %s\n"
#define DBGMSG_GLOBMATCH _MOD_ ": match_glob matched: %s\n"

/* Matches char for char until '*', then until the
 * char after, or returns true if nothing after.
 * Assumes pattern will contain '*'.
 * Returns false if empty string passed.
 */
static bool	_matches_pattern(const char *s, const char *pattern)
{
	while (*s && *pattern)
	{
		if ('*' == *pattern)
		{
			pattern++;
			if (*pattern)
			{
				while (*s)
				{
					if (true == _matches_pattern(s, pattern))
						return (true);
					s++;
				}
				return (false);
			}
			else
				return (true);
		}
		else if (*s != *pattern)
			return (false);
		s++;
		pattern++;
	}
	return (!(*s || *pattern));
}

/* Ensures dir contents drawn exclude
 * . and .. . Continues if readdir() fails.
 */
static struct dirent	*_glob_readdir(DIR *dir)
{
	struct dirent	*res;

	if (!dir)
		return (NULL);
	errno = 0;
	res = readdir(dir);
	if (NULL == res)
	{
		if (errno != 0)
			perror(SHELL_NAME);
		return (NULL);
	}
	while (0 == ft_strncmp(res->d_name, ".", 1) || 0 == ft_strncmp(res->d_name,
			"..", 2))
	{
		log_print(YELLOW "glob_readdir skipping %s\n" RESET, res->d_name);
		res = readdir(dir);
	}
	return (res);
}

/* Returns array of globbing matches from cwd
 * EXCEPT '.' and '..' Uses opendir() on "."
 * since this will always
 * run from interactive mode and not in background
 */
t_list	*match_glob(t_mem_mgr *m, const char *pattern)
{
	t_list			*lst;
	t_list			*new;
	t_list			*got;
	struct dirent	*res;
	DIR				*dir;
	char			*name;

	lst = NULL;
	debug_print(DBGMSG_GOTPATTERN, pattern);
	dir = opendir(".");
	if (!dir)
		return (perror(SHELL_NAME), NULL);
	res = _glob_readdir(dir);
	while (res)
	{
		if (true == _matches_pattern(res->d_name, pattern))
		{
			debug_print(DBGMSG_GLOBMATCH, res->d_name);
			name = ft_strdup_tmp(m, res->d_name);
			new = ft_lstnew_tmp(m, name);
			if (NULL == new || NULL == name)
				exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
			ft_lstadd_back(&lst, new);
		}
		res = _glob_readdir(dir);
	}
	closedir(dir);
	ft_lstprint(lst);
	debug_detect_cycle(lst);
	got = ft_lstsort(&lst);
	debug_detect_cycle(got);
	return (got);
}

/*
 * ins_lst is incoming list of glob matches to be inserted
 * at the lst_node position, overwriting it.
 * content is the existing arg node.
 * Deletes lst_node, deep copies glst and inserts
 * at lst_node position.
 * glst destroyed in arg_destroy
 */
int	do_arg_inserts(t_mem_mgr *mgr, t_list **lst_node, t_list **ins_lst,
		t_arg_data *content)
{
	t_list	*new_arg_data_lst;
	char	*new_arg;

	new_arg_data_lst = NULL;
	new_arg = NULL;
	if (ft_lstsize(*ins_lst) > 1)
	{
		ft_lstdelone_rwd_tmp(mgr, lst_node, lst_node, destroy_arg);
		new_arg_data_lst = ft_lstmap_tmp(mgr, *ins_lst, create_arg_data_node_deep,
				destroy_arg);
		ft_lstadd_insert(lst_node, new_arg_data_lst);
		ft_lstclear_str_tmp(mgr, ins_lst);
	}
	else
	{
		new_arg = ft_strdup_tmp(mgr, (*ins_lst)->content);
		if (!new_arg)
			exit_clean(&mgr->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
		mgr->dealloc(&mgr->list, content->raw);
		content->raw = new_arg;
		ft_lstclear_str_tmp(mgr, ins_lst);
	}
	return (0);
}

/* DO GLOBBING
 *
 * Called within t_list reverse iterator
 *  to handle insertions (in args lists)
 * Modifies argument t_list. Called during
 * execution, as globbing is done after parsing.
 *
 * Returns early in case of ambiguous redirect.
 *
 * Arg lst_c: lst_node's t_redir_data *content
 * Arg lst_node: node
 *
 * Error msg is "[filename]: no such file or directory\n"
 *
 * If multiple words found by glob, returns error:
 * "[original string]: ambiguous redirect\n"
 *
 * Anything returned from globbing matches will
 * NOT be processed further.
 */
int	p_do_globbing_args(t_mem_mgr *mgr, t_list **lst_node, void *lst_c)
{
	t_arg_data	*arg;
	t_list		*lst;
	int			res;

	res = 0;
	lst = NULL;
	arg = (t_arg_data *)lst_c;
	debug_print(DBGMSG_GOTARGS, arg->raw, (void *)lst_node);
	if (true == arg->do_globbing)
	{
		lst = match_glob(mgr, arg->raw);
		if (lst)
		{
			debug_print(DBGMSG_MATCHES, ft_lstsize(lst), lst->content);
			res = do_arg_inserts(mgr, lst_node, &lst, arg);
			if (0 != res)
				return (res);
		}
	}
	return (0);
}
