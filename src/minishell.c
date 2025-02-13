#include "../include/minishell.h"

static void	_do_test(t_state *s)
{
	char		input[INPUT_BUF_SZ];
	t_ast_node	*ast;

	if (NULL == fgets(input, sizeof(input), stdin))
		return ;
	set_input(s, input);
	ast = parse(s, get_input(s));
	if (!ast)
		err("ERR parse()\n");
	set_exit_status(s, cmd_execute(s, ast));
	s_free_cmd(s);
}

static int	_do_loop(t_state *s)
{
	t_ast_node	*ast;

	g_last_signal = -1;
	set_input(s, readline(PROMPT));
	if (NULL == get_input(s))
	{
		write(STDOUT_FILENO, "exit\n", sizeof("exit\n"));
		return (SIGEOF_AT_INPUT);
	}
	if (get_input(s)[0] != '\0')
		add_history(get_input(s));
	else
		return (0);
	ast = parse(s, get_input(s));
	if (ast)
		set_exit_status(s, cmd_execute(s, ast));
	s_free_cmd_lex_parse(s);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_state	*s;
	int		res;

	(void)argv;
	(void)argc;
	s = init_state(envp);
	if (MYTEST)
		_do_test(s);
	else
	{
		while (1)
		{
			res = _do_loop(s);
			if (SIGEOF_AT_INPUT == res)
				break ;
		}
	}
	destroy_state(s);
	return (0);
}
