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
	int		i;

	array = malloc(sizeof(char *) * (size_t)(argc + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (i < argc && args)
	{
		array[i] = ft_strdup(((t_arg_data *)args->content)->raw);
		if (!array[i])
		{
			_destroy_arr(array);
			return (NULL);
		}
		args = args->next;
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

/* t_arg_data llist copy constructor using a llist of char*.
 * Returns new void *content for llist construction/duplication.
 */
void	*create_arg_data_node_deep(void *content)
{
	t_arg_data	*arg_data;

	content = (char *)content;
	arg_data = malloc(sizeof(t_arg_data));
	if (arg_data)
	{
		arg_data->raw = ft_strdup(content);
		if (arg_data->raw)
		{
			arg_data->do_expansion = false;
			arg_data->do_globbing = false;
			arg_data->global_state = NULL;
			arg_data->in_dquotes = false;
			arg_data->option = false;
			arg_data->tmp = NULL;
		}
		else
		{
			free(arg_data);
			return (NULL);
		}
	}
	return ((void *)arg_data);
}

static void _print_arg_data(t_arg_data *data) 
{
	if (DEBUG)
	{
		if (!data) {
			printf("  t_arg_data is NULL\n");
			return;
		}
		printf("  raw: %s\n", data->raw ? data->raw : "(null)");
		printf("  tmp: ");
		if (data->tmp) {
			for (int i = 0; data->tmp[i] != NULL; i++) {
				printf("%s ", data->tmp[i]);
			}
			printf("\n");
		} else {
			printf("(null)\n");
		}
		printf("  option: %s\n", data->option ? "true" : "false");
		printf("  do_globbing: %s\n", data->do_globbing ? "true" : "false");
		printf("  do_expansion: %s\n", data->do_expansion ? "true" : "false");
		printf("  in_dquotes: %s\n", data->in_dquotes ? "true" : "false");
	}
}

void debug_print_list(t_list *head) 
{
    t_list *current = head;
    int i = 0;
	
	if (DEBUG)
	{
		while (current) 
		{
			printf("Node %d:\n", i);
			_print_arg_data((t_arg_data *)current->content);
			current = current->next;
			i++;
		}
	}
}
