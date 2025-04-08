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

static void	_print_bool(bool val)
{
	if (val)
		printf("true\n");
	else
		printf("false\n");
}

static void	_print_rest(t_arg_data *data)
{
	printf("  lst: ");
	if (data->lst_tokens)
		printf("%p\n", data->lst_tokens);
	else
		printf("0x0\n");
	printf("  option: ");
	_print_bool(data->option);
	printf("  do_globbing: ");
	_print_bool(data->do_globbing);
	printf("  do_expansion: ");
	_print_bool(data->do_expansion);
	printf("  in_dquotes: ");
	_print_bool(data->in_dquotes);
}

static void	_print_arg_data(t_arg_data *data)
{
	int	i;

	if (DEBUG && isatty(STDOUT_FILENO))
	{
		if (!data)
			return ((void)printf("  t_arg_data is NULL\n"));
		printf("  raw: _");
		if (data->raw)
			printf("%s_\n", data->raw);
		else
			printf("(null)_\n");
		printf("  tmp: ");
		if (data->tmp)
		{
			i = -1;
			while (data->tmp[++i] != NULL)
				printf("%s ", data->tmp[i]);
			printf("\n");
		}
		else
			printf("(null)\n");
		printf("  grouparg: ");
		_print_bool(data->is_grouparg);
		_print_rest(data);
	}
}

void	dprint_arglist(t_list *head)
{
	t_list	*current;
	int		i;

	current = head;
	i = 0;
	if (DEBUG && isatty(STDOUT_FILENO))
	{
		while (current)
		{
			dprint("Node %d:\n", i);
			_print_arg_data((t_arg_data *)current->content);
			current = current->next;
			i++;
		}
		dprint("List printed.\n");
	}
}
