#include "minishell.h"

int	g_exit_code;

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define PROMPT GREEN "minish> " RESET

int	main(void)
{
	t_ast_node	*ast;
	t_state		*s;

	s = init_state();
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
			fprintf(stderr, "SHIT!\n");
		set_exit_status(s, cmd_execute_simple(ast));
		s_free_cmd(s);
	}
	destroy_state(s);
	return (0);
}
