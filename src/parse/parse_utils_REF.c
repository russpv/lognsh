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

static void	_print_arg_data(t_arg_data *data)
{
	if (DEBUG && isatty(STDOUT_FILENO))
	{
		if (!data)
			return ((void)printf("  t_arg_data is NULL\n"));
		printf("  raw: _%s_\n", data->raw ? data->raw : "(null)");
		printf("  tmp: ");
		if (data->tmp)
		{
			for (int i = 0; data->tmp[i] != NULL; i++)
				printf("%s ", data->tmp[i]);
			printf("\n");
		}
		else
			printf("(null)\n");
		printf("  grouparg: %s\n", data->is_grouparg ? "true" : "false");
		printf("  lst: %p\n", data->lst_tokens ? data->lst_tokens : 0x0);
		printf("  option: %s\n", data->option ? "true" : "false");
		printf("  do_globbing: %s\n", data->do_globbing ? "true" : "false");
		printf("  do_expansion: %s\n", data->do_expansion ? "true" : "false");
		printf("  in_dquotes: %s\n", data->in_dquotes ? "true" : "false");
	}
}

void	debug_print_list(t_list *head)
{
	t_list	*current;
	int		i;

	current = head;
	i = 0;
	if (DEBUG && isatty(STDOUT_FILENO))
	{
		while (current)
		{
			debug_print("Node %d:\n", i);
			_print_arg_data((t_arg_data *)current->content);
			current = current->next;
			i++;
		}
		debug_print("List printed.\n");
	}
}

