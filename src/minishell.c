#include "minishell.h"

int	g_exit_code;

int	main(int argc, char **argv, char **envp)
{
	t_ast_node	*ast;
	t_state		*s;

	(void)argv;
	(void)argc;
	s = init_state(envp);
	set_signal_handlers(); // **working on signal handling in child process
	while (1)
	{
		set_input(s, readline(PROMPT));
		if (!get_input(s)) // handles Ctrl-D in parent process
		{
			write(STDOUT_FILENO, "exit\n", 5);
			destroy_state(s);
			exit(0);
		}
		if (get_input(s)[0] != '\0')
			add_history(get_input(s));
		ast = parse(s, get_input(s));
		if (!ast)
			err("ERR parse()\n");
		set_exit_status(s, cmd_execute(s, ast));
		s_free_cmd(s);
	}
	destroy_state(s);
	return (0);
}
