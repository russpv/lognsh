#include "../include/minishell.h"

static void	_do_test(t_state *s)
{
	char		input[INPUT_BUF_SZ];
	t_ast_node	*ast;

	if (MYTEST)
	{
		if (NULL == fgets(input, sizeof(input), stdin))
			return ;
		ast = parse(s, input);
		if (!ast)
			err("ERR parse()\n");
		set_exit_status(s, cmd_execute(s, ast));
		s_free_cmd(s);
	}
}

void	clear_current_line(void)
{
#ifndef MACOS
	rl_replace_line("", 0); // Replace the current line with an empty string
#endif
	rl_on_new_line();
	rl_redisplay();
}

static int	_do_loop(t_state *s)
{
	t_ast_node	*ast;
	int	exit_status;

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
	//if (ast && (!get_lexer(s) || !get_heredoc(s)))
	//	clear_current_line();
	//if (ast)
	//	set_exit_status(s, cmd_execute(s, ast));
	exit_status = 0;
	if (ast)
        	exit_status = cmd_execute(s, ast);
    // >> Only clear line after execution if not a heredoc or shell-spawning command
	if (ast && !get_heredoc(s) && exit_status == ERR_CMD_NOT_FOUND)
		clear_current_line();
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
	if (s)
	{
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
		destroy_state(&s);
	}
	return (0);
}
