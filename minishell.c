#include "minishell.h"

int	g_exit_code;

int	main(int argc, char **argv, char **envp)
{
	t_ast_node	*ast;
	t_state		*s;

	(void)argv;
	(void)argc;
	s = init_state(envp);
	// signal(SIGINT, sigint_handler); // Ctrl-C
	// signal(SIG..., ..._handler); // Ctrl-...
	while (1)
	{
		set_input(s, readline(PROMPT));
		if (NULL == get_input(s))
			break ;
		if (get_input(s)[0] != '\0')
			add_history(get_input(s));
		ast = parse(s, get_input(s));
		if (!ast)
			fprintf(stderr, "PooPoo!\n");
		set_exit_status(s, cmd_execute_full(s, ast));
		s_free_cmd(s);
	}
	destroy_state(s);
	return (0);
}
