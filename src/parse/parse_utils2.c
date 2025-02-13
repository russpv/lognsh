#include "parse_int.h"

static void	_destroy_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr[i])
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

// Deep copies linked list of arguments to char **array
char	**list_to_array(t_list *args, int argc)
{
	char	**array;
	t_list	*current;
	int		i;

	array = malloc(sizeof(char *) * (size_t)(argc + 1));
	if (!array)
		return (NULL);
	current = args;
	i = 0;
	while (i < argc && current)
	{
		array[i] = ft_strdup(((t_arg_data *)current->content)->raw);
		if (!array[i])
		{
			_destroy_arr(array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	array[argc] = NULL;
	return (array);
}

/* t_arg_data llist copy constructor using a llist of char*.
 * Returns new void *content for llist construction/duplication.
 */
void	*create_arg_data_node(void *content)
{
	t_arg_data	*arg_data;

	content = (char *)content;
	arg_data = malloc(sizeof(t_arg_data));
	if (arg_data)
	{
		arg_data->do_expansion = false;
		arg_data->do_globbing = false;
		arg_data->global_state = NULL;
		arg_data->in_dquotes = false;
		arg_data->option = false;
		arg_data->tmp = NULL;
		arg_data->raw = content;
	}
	return ((void *)arg_data);
}
