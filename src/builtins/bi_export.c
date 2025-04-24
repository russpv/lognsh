
#include "bi_int.h"
#define CMD_NAME "export"
#define MSG_NOARGS "results are not specified with no arguments(refer to man)\n"

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
