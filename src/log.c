#include "log.h"
# include "parse/parse.h"
# include "command/command.h"


void log_command_info(t_cmd *c, t_ast_node *a)
{
    int i;

    i = -1;
	if (LOGGING)
	{
		if (p_get_cmd(a)) 
			colored_printf(YELLOW, "\tExecuting command: %s\n", p_get_cmd(a));
		else
			colored_printf(RED, "\tExecuting command: (NULL)\n");
		colored_printf(YELLOW, "\tArguments:\n");
		if (c_get_argv(c))
		{
            while (++i < c_get_argc(c))
				colored_printf(YELLOW, "\t  argv[%d]: %s\n", i, c_get_argv(c)[i]);
		}
		if (p_get_argc(a) > 0 && c_get_argv(c) && c_get_argv(c)[p_get_argc(a)] == NULL)
			colored_printf(YELLOW, "\t  argv[%d]: (NULL)\n", p_get_argc(a));
	}
    (void)i;
}

