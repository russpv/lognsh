#include "bi_int.h"
#define CMD_NAME "export"
#define MSG_NOARGS "results are not specified with no arguments(refer to man)\n"


/* -------------------------------------------------------------------------
 * Function Group: Export env manipulation logic
 * Description   : 
 * ------------------------------------------------------------------------- */
#define EMSG_BADMALLOC "memory allocation failed.\n"

// Returns key string, or copy of arg if no equal sign found
static int	_extract_key(char key[], const char *arg, char *equal_pos)
{
	size_t	key_len;

	if (!arg)
		return (ERR_ARGS);
	if (!*arg)
		return (ERR_ARGS);
	if (!equal_pos)
	{
		key_len = ft_strnlen(arg, MAX_NAME_LEN);
		ft_strscpy(key, arg, key_len + 1);
	}
	else if (*equal_pos)
	{
		key_len = equal_pos - arg;
		if (key_len < MAX_NAME_LEN)
			ft_strscpy(key, arg, key_len + 1);
		else
			return (E2BIG);
	}
	return (0);
}

// Returns ptr to the start of value
static void	_reset_value_ptr(char *equal_pos, const char **value_ptr)
{
	if (equal_pos)
		*value_ptr = equal_pos + 1;
	else
		*value_ptr = NULL;
}

static void	_update_special_state_vars(t_state *s, char *key, const char *value)
{
	if (0 == ft_strncmp(PATH_KEY, key, MAX_NAME_LEN))
		set_path(s, value);
}

// updates or adds key-value pair in s->sh_env_list
// assumes equal_pos is within arg
int	process_arg_update_add(t_state *s, const char *arg, char *equal_pos,
		int *error_occurred)
{
	const char	*value;
	char		key[MAX_NAME_LEN];
	int			exit_code;

	if (!s || !arg || !error_occurred || !equal_pos)
		return (ERR_ARGS);
	ft_memset(key, 0, MAX_NAME_LEN);
	if (0 != _extract_key(key, arg, equal_pos))
		return (ERR_ARGS);
	if (0 == ft_strnlen(key, MAX_NAME_LEN))
		return (ERR_ARGS);
	_reset_value_ptr(equal_pos, &value);
	if (MAX_NAME_LEN == ft_strnlen(value, MAX_NAME_LEN))
		return (print_value_toolong(), ERR_ARGS);
	exit_code = env_upsert_value(get_mem(s), get_env_list(s), key, value);
	if (0 != exit_code)
		*error_occurred = 1;
	if (0 == exit_code)
		_update_special_state_vars(s, key, value);
	return (exit_code);
}


/* -------------------------------------------------------------------------
 * Function Group: Export parse logic
 * Description   : 
 * ------------------------------------------------------------------------- */

#define EMPTY_KEY 77
#define EMSG_BADMALLOC "memory allocation failed.\n"
#define EMSG_KEY_NOTVALID "not a valid identifier\n"

/* Tests key is of correct format */
static int	_validate_key(const char *key, int got_good_arg)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
	{
		if (ft_isspace(key[0]))
			if (1 == got_good_arg)
				return (EMPTY_KEY);
		return (ERR_GENERAL);
	}
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (ERR_GENERAL);
		i++;
	}
	return (0);
}

// validates the argument format and key syntax
static int	_process_arg_validation(const char *arg, char **equal_pos,
		int *error_occurred, int got_good_arg)
{
	if (!arg || !equal_pos || !error_occurred)
		return (0);
	*equal_pos = ft_strchr(arg, '=');
	if (*equal_pos)
	{
		**equal_pos = '\0';
		if (0 != _validate_key(arg, got_good_arg))
		{
			*error_occurred = 1;
			**equal_pos = '=';
			return (ERR_GENERAL);
		}
		**equal_pos = '=';
	}
	else if (0 != _validate_key(arg, got_good_arg))
	{
		*error_occurred = 1;
		return (ERR_GENERAL);
	}
	return (0);
}

// Processes a single export argument
int	parse_arg(t_state *s, const char *arg, int *error_occurred, \
int got_good_arg)
{
	char	*equal_pos;
	int		res;

	if (!s || !arg || !error_occurred)
		return (ERR_ARGS);
	res = _process_arg_validation(arg, &equal_pos, error_occurred, \
got_good_arg);
	if (EMPTY_KEY == res)
		return (0);
	else if (0 != res)
	{
		print_custom_err(CMD_NAME, EMSG_KEY_NOTVALID);
		return (ERR_GENERAL);
	}
	if (0 != process_arg_update_add(s, arg, equal_pos, error_occurred))
	{
		print_custom_err(CMD_NAME, EMSG_INVLD);
		return (ERR_GENERAL);
	}
	return (0);
}

static int	_do_logic(t_state *s, char **argv, int argc, int *error_occurred)
{
	int	i;
	int	got_good_arg;

	got_good_arg = 0;
	i = 1;
	while (i < argc)
	{
		if (!argv[i])
		{
			*error_occurred = 1;
			i++;
			continue ;
		}
		if (0 != parse_arg(s, argv[i], error_occurred, got_good_arg))
		{
			return (ERR_GENERAL);
		}
		got_good_arg = 1;
		i++;
	}
	return (0);
}

/* EXPORT
 * export [name[=value]]
 * Marks each name to be passed to child processes in the environment,
 * as long as equal sign is present.
 * Sets error exit code if nothing was exported.
 * Maximum name length is _
 * Maximum value length is _
 */
int	bi_export(t_state *s, char **argv, int argc)
{
	int	error_occurred;
	int	res;

	if (!s || !argv)
		return (print_custom_err(CMD_NAME, EMSG_INVLD), 1);
	if (argc == 1)
		return (print_custom_err(CMD_NAME, EMSG_INVLD), 0);
	error_occurred = 0;
	res = _do_logic(s, argv, argc, &error_occurred);
	if (0 != res)
		return (res);
	if (error_occurred)
		return (ERR_GENERAL);
	return (0);
}
