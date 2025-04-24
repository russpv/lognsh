
#include "bi_int.h"

#define _CMD_NAME_ "unset"

static bool	_is_valid_var_name(char *name)
{
	if (!name || !*name || ft_isdigit(*name))
		return (false);
	while (*name)
	{
		if (false == ft_isalnum(*name) && *name != '_')
			return (false);
		name++;
	}
	return (true);
}

// unset will always return ('0'); remove_env_node will only work for valid keys
int	bi_unset(t_state *s, char **argv, int argc)
{
	int	i;
	int	ret;

	if (!s || !argv)
		return (1);
	if (argc == 1)
		return (0);
	i = 0;
	ret = 0;
	while (argv[++i])
	{
		ret = 0;
		if (false == _is_valid_var_name(argv[i]))
		{
			print_invalid_name(_CMD_NAME_, argv[i]);
			ret = 1;
		}
		else
		{
			if (0 == ft_strncmp(PATH_KEY, argv[i], MAX_NAME_LEN))
				set_path(s, NULL);
			env_remove_node(get_mem(s), get_env_list_ptr(s), argv[i]);
		}
	}
	return (ret);
}
