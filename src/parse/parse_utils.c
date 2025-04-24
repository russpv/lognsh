
#include "parse_int.h"

static void	_destroy_arr(t_mem_mgr *m, char **arr)
{
	int	i;

	i = 0;
	if (!arr[i])
		return ;
	while (arr[i])
		m->dealloc(&m->list, arr[i++]);
	m->dealloc(&m->list, arr);
}

// Deep copies linked list of arguments to char **array
char	**list_to_array(t_mem_mgr *m, t_list *args, int argc)
{
	char	**array;
	int		i;
	char	*new_s;

	array = m->f(&m->list, sizeof(char *) * (size_t)(argc + 1));
	if (NULL == array)
		exit_clean(&m->list, ENOMEM, __FUNCTION__, EMSG_MALLOC);
	i = 0;
	while (i < argc && args)
	{
		new_s = NULL;
		array[i] = new_s;
		if (NULL == array[i])
			array[i] = ft_strdup_tmp(m, ((t_arg_data *)args->content)->raw);
		if (NULL == array[i])
		{
			_destroy_arr(m, array);
			return (NULL);
		}
		args = args->next;
		i++;
	}
	array[argc] = NULL;
	return (array);
}

int	ft_lstiter_redir(t_list *lst, t_redir_ctx *ctx, \
	int (*f)(void *, t_redir_ctx *))
{
	int	res;

	res = 0;
	if (lst == NULL)
		return (-1);
	while (lst)
	{
		res = f((lst)->content, ctx);
		if (0 != res)
			return (res);
		lst = (lst)->next;
	}
	return (0);
}

void	set_parse_err(t_parser *p)
{
	if (!p)
		return ;
	p->parse_error = 1;
}

