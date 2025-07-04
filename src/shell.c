#include "shell.h"

static void	_do_test(t_state *s)
{
	char		input[INPUT_BUF_LIM];
	t_ast_node	*ast;

	if (MYTEST)
	{
		if (NULL == fgets(input, sizeof(input), stdin))
			return ;
		set_input(s, ft_strdup(input));
		ast = parse(s, input);
		if (!ast)
			fprintf(stderr, "ERR parse()\n");
		set_exit_status(s, cmd_execute(s, ast));
		s_free_cmd(s);
	}
}

static int	_do_one_loop(t_state *s, char *input)
{
	t_ast_node	*ast;
	
	g_last_signal = -1;
	set_input(s, ft_strdup(input));
	if (NULL == get_input(s))
		return (SIGEOF_AT_INPUT);
	g_last_signal = -1;
	if (get_input(s)[0] == '\0')
		return (0);
	ast = parse(s, get_input(s));
	if (ast)
		set_exit_status(s, cmd_execute(s, ast));
	s_free_cmd_lex_parse(s);
	return (*get_status(s));
}

static int	_do_loop(t_state *s)
{
	t_ast_node	*ast;

	g_last_signal = -1;
	set_input(s, readline(get_prompt(s)));
	if (NULL == get_input(s))
	{
		write(STDOUT_FILENO, "exit\n", sizeof("exit\n") - 1);
		return (SIGEOF_AT_INPUT);
	}
	g_last_signal = -1;
	if (get_input(s)[0] != '\0')
		add_history(get_input(s));
	else
		return (0);
	ast = parse(s, get_input(s));
	if (ast)
		set_exit_status(s, cmd_execute(s, ast));
	cprintf(MAGENTA, "Exit code set: %d\n", *get_status(s));
	s_free_cmd_lex_parse(s);
	return (*get_status(s));
}

int	main(int argc, char **argv, char **envp)
{
	t_state	*s;
	int		res;

	res = 0;
	s = init_state(envp);
	if (s)
	{
		if (MYTEST)
			_do_test(s);
		else if (argc > 1)
			res = _do_one_loop(s, argv[1]);
		else
		{
			while (1)
			{
				res = _do_loop(s);
				if (SIGEOF_AT_INPUT == res)
					break ;
			}
		}
		destroy_state(&s);
	}
	return (res);
}
