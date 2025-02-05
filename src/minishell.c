#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_ast_node	*ast;
	t_state		*s;
	char input[MAX_INPUT];

	(void)argv;
	(void)argc;
	s = init_state(envp); // TODO register signal handlers
	if (MYTEST)
	{
		if (!fgets(input, sizeof(input), stdin)) 
        	return 1;
		set_input(s, input);
		ast = parse(s, get_input(s));
		if (!ast)
			err("ERR parse()\n"); //TODO refactor errors
		set_exit_status(s, cmd_execute(s, ast));
		s_free_cmd(s);
	}
	else
	{
		while (1)
		{
			set_input(s, readline(PROMPT));
			if (NULL == get_input(s))
				break ;
			if (get_input(s)[0] != '\0')
				add_history(get_input(s));
			else
				continue;
			ast = parse(s, get_input(s));
			if (!ast)
				err("ERR parse()\n"); //TODO refactor errors
			set_exit_status(s, cmd_execute(s, ast));
			s_free_cmd(s);
		}
	}
	destroy_state(s);
	return (0);
}
