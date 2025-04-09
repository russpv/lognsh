#include "error.h"

void	print_parse_error(t_state *s, const char *word, size_t pos)
{
	const char	*input = (const char *)get_input(s);
	size_t		i;

	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": parse error near `",
		ft_strlen(": parse error near `"));
	ft_putstr_fd(word, STDERR_FILENO);
	write(STDERR_FILENO, "` in:\n", ft_strlen("` in:\n"));
	ft_putstr_fd(input, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
	i = -1;
	while (++i < pos)
		write(STDERR_FILENO, "-", 1);
	write(STDERR_FILENO, "^\n", 2);
}

void	print_nocmd_error(t_state *s, const char *word, size_t pos)
{
	const char	*input = (const char *)get_input(s);
	const char	*alt = "...";
	size_t		i;

	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": expected valid command after `",
		ft_strlen(": expected valid command after `"));
	if (ft_strchr(word, '\n'))
		ft_putstr_fd(alt, STDERR_FILENO);
	else
		ft_putstr_fd(word, STDERR_FILENO);
	write(STDERR_FILENO, "` in:\n", ft_strlen("` in:\n"));
	ft_putstr_fd(input, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
	i = -1;
	while (++i < pos)
		write(STDERR_FILENO, "-", 1);
	write(STDERR_FILENO, "^\n", 2);
}

void	print_redir_error(t_state *s, const char *word, size_t pos)
{
	const char	*input = (const char *)get_input(s);
	size_t		i;

	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": expected valid filename after `",
		ft_strlen(": expected valid filename after `"));
	ft_putstr_fd(word, STDERR_FILENO);
	write(STDERR_FILENO, "` in:\n", ft_strlen("` in:\n"));
	ft_putstr_fd(input, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
	i = -1;
	while (++i < pos)
		write(STDERR_FILENO, "-", 1);
	write(STDERR_FILENO, "^\n", 2);
}

void	print_lex_error(t_lex *l, char *word)
{
	const char		*input = (const char *)lex_get_raw(l);
	size_t			i;
	const size_t	pos = (size_t)(lex_get_ptr(l) - (char *)input);

	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": parse error near `",
		ft_strlen(": parse error near `"));
	ft_putstr_fd(word, STDERR_FILENO);
	write(STDERR_FILENO, "` in:\n", ft_strlen("` in:\n"));
	ft_putstr_fd(input, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
	i = -1;
	while (++i < pos)
		write(STDERR_FILENO, "-", 1);
	write(STDERR_FILENO, "^\n", 2);
}

void print_value_toolong(void)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	ft_putstr_fd(": Value too long.\n", STDERR_FILENO);
}
