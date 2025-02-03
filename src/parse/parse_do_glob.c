#include "parse_int.h"

/* Match char for char until '*', then until the char after, or return true if nothing after 
 * Assumes pattern will contain '*'
 * An empty string will return false.
 */
bool	matches_pattern(const char *s, const char *pattern)
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
					if (true == matches_pattern(s, pattern))
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

// Returns array of globbing matches in cwd
// Uses opendir() on "." since this will always
// run from interactive mode and not in background
t_list	*match_glob(const char *pattern)
{
	t_list *lst = NULL;
	struct dirent *res;

	debug_print("match_glob got pattern: %s\n", pattern);
	DIR *dir = opendir(".");
	if (!dir)
	{
		perror("ERR opendir()\n");
		return (NULL);
	}
	res = readdir(dir);
	while (res)
	{
		debug_print("match_glob readdir: %s\n", res->d_name);
		if (true == matches_pattern(res->d_name, pattern))
		{
			debug_print("match_glob matched: %s\n", res->d_name);
			ft_lstadd_back(&lst, ft_lstnew(ft_strdup(res->d_name))); // TODO malloc guard
		}
		res = readdir(dir);
	}
	closedir(dir);
	return (lst);
}

/* Returns redir raw string in case of error */
int	p_do_globbing_redirs(void *c)
{
	t_list *lst = NULL;

	t_redir_data *content = (t_redir_data *)c;
	debug_print("p_do_globbing_redirs got redir: %s do_glob:%d\n", content->filename, content->do_globbing);
	if (true == content->do_globbing)
	{
		lst = match_glob((const char *)content->filename);
		if (lst)
		{
			debug_print("p_do_globbing_redirs found (1st): %s\n", (char *)lst->content);
			if (ft_lstsize(lst) > 1) 
				return (-1);
			else
			{
				char *new_fn = ft_strdup(lst->content);
				if (!new_fn)
					return (1); //TODO malloc processing
				free(content->filename);
				content->filename = new_fn;
			}
		}
	}
	return (0);
}
/*
 * To be called by Command executor, not Parser.
 * Passed to t_list *redirs BACKWARDS iterator,
 * 	to handle insertions while iterating.
 * Modifies argument t_list.
 * Returns early in case of ambiguous redirect:
 * 
 * Arg c: t_redir_data *
 * Arg parent_lst: 
 * 
 * For filenames in redirs, does not modify * string unless results found
 * Error is "[filename]: no such file or directory\n"
 * If multiple words found in redir glob: error message is:
 * "[original string]: ambiguous redirect\n"
 * So, if multiple results found after expansion then globbing, display
 * original argument, ie throw error.
 * 
 * Anything returned from globbing matches will NOT be processed further.
*/
void	p_do_globbing(t_list **lst_node, void *lst_c)
{
	t_list *lst = NULL;
	t_arg_data *content = (t_arg_data *)lst_c;
	debug_print("p_do_globbing got arg: %s, lst:%p\n", content->raw, (void*)lst_node);
	if (true == content->do_globbing)
	{
		lst = match_glob(content->raw);
		if (lst)
		{
			debug_print("p_do_globbing found %d matches, 1st: %s\n", ft_lstsize(lst), lst->content);
			if (ft_lstsize(lst) > 1) //need to insert
			{
				ft_lstdelone_rwd(lst_node, lst_node, destroy_arg);
				t_list *new_arg_data_lst = ft_lstmap(lst, create_arg_data_node, destroy_arg);
				ft_lstadd_insert(lst_node, new_arg_data_lst); 
			}
			else
			{ 
				char *new_arg = ft_strdup(lst->content);
				if (!new_arg)
					return ; //TODO malloc processing
				free(content->raw);
				content->raw = new_arg;
				ft_lstclear(&lst, free);
			}
		}
	}
}
