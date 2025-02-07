#include "log.h"
# include "parse/parse.h"
# include "command/command.h"


void	log_command_info(t_cmd *c, t_ast_node *a)
{
	debug_print("log_command_info: \n");
	if (LOGGING)
	{
		colored_printf(YELLOW, "\tExecuting command: %s\n", p_get_cmd(a));
		colored_printf(YELLOW, "\tArguments:\n");
		if (c_get_argv(c))
		{
			for (int i = 0; c_get_argv(c)[i] != NULL; i++)
				colored_printf(YELLOW, "\t  argv[%d]: %s\n", i,
					c_get_argv(c)[i]);
		}
		colored_printf(YELLOW, "\t  argv[%d]: (NULL)\n", p_get_argc(a) + 1);
	}
}
