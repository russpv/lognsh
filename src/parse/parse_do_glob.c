#include "parse_int.h"

#define ERRMSG_OPENDIR "ERR opendir()\n"
#define ERRMSG_AMBIG "ERR multiple redirect targets globbed\n"
#define DBGMSG_REDIR_NODE "Parser: p_do_globbing_redirs got redir type: %d fn:%s doc:\
		%s do_glob:%d\n"
#define DBGMSG_REDIR_GLOB "Parser: p_do_globbing_redirs found:%s...\n"
#define DBGMSG_GOTARGS "Parser: p_do_globbing - arg: %s, lst:%p\n"
#define DBGMSG_MATCHES "Parser: p_do_globbing found %d matches, 1st: %s\n"
#define DBGMSG_GOTPATTERN "Parser: match_glob got pattern: %s\n"
#define DBGMSG_GLOBMATCH "Parser: match_glob matched: %s\n"

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
	while (0 == ft_strncmp(res->d_name, ".", 1) \
		|| 0 == ft_strncmp(res->d_name, "..", 2))
	{
		log_print(YELLOW"glob_readdir skipping %s\n"RESET, res->d_name);
		res = readdir(dir);
	}
	return (res);
}

/* Returns array of globbing matches from cwd
 * EXCEPT '.' and '..' Uses opendir() on "." 
 * since this will always
 * run from interactive mode and not in background
 */
static t_list	*_match_glob(const char *pattern)
{
	t_list			*lst;
	t_list			*new;
	struct dirent	*res;
	DIR				*dir;

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
			char *name =ft_strdup(res->d_name);
			new = ft_lstnew(name);
			if (NULL == new || NULL == name)
				return (ft_lstclear(&lst, free), NULL);
			ft_lstadd_back(&lst, new);
		}
		res = _glob_readdir(dir);
	}
	closedir(dir);
	ft_lstprint(lst);
	debug_detect_cycle(lst);
	t_list *got = ft_lstsort(&lst, ft_strcmplow);
	debug_detect_cycle(got);

	return (got);
}

/* Operates on a single t_redir_data node. */
int	p_do_globbing_redirs(void *c)
{
	t_redir_data		*r;
	char				*new_fn;

	r = (t_redir_data *)c;
	debug_print(DBGMSG_REDIR_NODE, r->type, r->filename,\
		r->heredoc_body, r->do_globbing);
	if (true == r->do_globbing)
	{
		r->lst_glob = _match_glob((const char *)r->filename);
		if (r->lst_glob)
		{
			debug_print(DBGMSG_REDIR_GLOB,(char *)r->lst_glob->content);
			if (ft_lstsize(r->lst_glob) > 1)
				return (err(ERRMSG_AMBIG), ERR_AMBIGUOUS_REDIR);
			else
			{
				new_fn = ft_strdup(r->lst_glob->content);
				if (!new_fn)
					return (ERR_MEM);
				free(r->filename);
				r->filename = new_fn;
			}
		}
	}
	return (0);
}

/* 
 * lst is incoming list of glob matches to be inserted 
 * after the lst_node.
 * content is the existing arg node.
 */
static int	_do_ops(t_list **lst_node, t_list **glst, t_arg_data	*content)
{
	t_list		*new_arg_data_lst;
	char		*new_arg;

	new_arg_data_lst = NULL;
	new_arg = NULL;
	if (ft_lstsize(*glst) > 1)
	{
		ft_lstdelone_rwd(lst_node, lst_node, destroy_arg);
		new_arg_data_lst = ft_lstmap(*glst, create_arg_data_node,\
			destroy_arg);
		ft_lstadd_insert(lst_node, new_arg_data_lst);
	}
	else
	{
		new_arg = ft_strdup((*glst)->content);
		if (!new_arg)
			return (ERR_MEM);
		free(content->raw);
		content->raw = new_arg;
		ft_lstclear(glst, free);
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
 * Arg c: t_redir_data *
 * Arg parent_lst:
 *
 * Error msg is "[filename]: no such file or directory\n"
 * 
 * If multiple words found by glob, returns error:
 * "[original string]: ambiguous redirect\n"
 *
 * Anything returned from globbing matches will 
 * NOT be processed further.
 */
int	p_do_globbing(t_list **lst_node, void *lst_c)
{
	t_list		*lst;
	t_arg_data	*content;
	int			res;

	res = 0;
	lst = NULL;
	content = (t_arg_data *)lst_c;
	debug_print(DBGMSG_GOTARGS, content->raw, (void *)lst_node);
	if (true == content->do_globbing)
	{
		lst = _match_glob(content->raw);
		if (lst)
		{
			debug_print(DBGMSG_MATCHES, ft_lstsize(lst), lst->content);
			res = _do_ops(lst_node, &lst, content);
			if (0 != res)
				return (res);
		}
	}
	return (0);
}
