#include "parse_int.h"

#define EMSG_OPENDIR "ERR opendir()\n"
#define DBGMSG_GOTPATTERN "%s: match_glob got pattern: %s\n"
#define DBGMSG_GLOBMATCH "%s: match_glob matched: %s\n"

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
		lgprint(YELLOW "glob_readdir skipping %s\n" RESET, res->d_name);
		res = readdir(dir);
	}
	return (res);
}

static void	_add_readdir_globs(DIR *dir, t_mem_mgr *m, const char *pattern,
		t_list **lst)
{
	t_list			*new;
	struct dirent	*dres;
	char			*name;

	dres = _glob_readdir(dir);
	while (dres)
	{
		if (true == _matches_pattern(dres->d_name, pattern))
		{
			dprint(DBGMSG_GLOBMATCH, _MOD_, dres->d_name);
			name = ft_strdup_mem(m, dres->d_name);
			new = ft_lstnew_tmp(m, name);
			if (NULL == new || NULL == name)
				exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
			ft_lstadd_back(lst, new);
		}
		dres = _glob_readdir(dir);
	}
}

/* Returns array of globbing matches from cwd
 * EXCEPT '.' and '..' Uses opendir() on "."
 * since this will always
 * run from interactive mode and not in background
 */
t_list	*match_glob(t_mem_mgr *m, const char *pattern)
{
	t_list	*lst;
	t_list	*got;
	DIR		*dir;

	lst = NULL;
	got = NULL;
	dprint(DBGMSG_GOTPATTERN, _MOD_, pattern);
	dir = opendir(".");
	if (!dir)
		return (perror(SHELL_NAME), NULL);
	_add_readdir_globs(dir, m, pattern, &lst);
	closedir(dir);
	if (lst)
	{
		ft_lstprint(lst);
		debug_detect_cycle(lst);
		got = ft_lstsort(&lst);
		debug_detect_cycle(got);
	}
	return (got);
}
