#include "error.h"

void	print_parse_redir_error(t_state *s, size_t pos)
{
	const char		*input = (const char *)get_input(s);
	size_t			i;
	const size_t	cols = (size_t)get_wincols();

	if (pos > cols)
		pos = pos % cols;
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ambiguous redirect in:\n",
		ft_strlen(": ambiguous redirect in:\n"));
	ft_putstr_fd(input, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
	i = -1;
	while (++i < pos)
		write(STDERR_FILENO, "-", 1);
	write(STDERR_FILENO, "^\n", 2);
}

void	print_is_dir(char *path)
{
	(void)path;
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": `", ft_strlen(": `"));
	ft_putstr_fd(path, STDERR_FILENO);
	write(STDERR_FILENO, "`: is a directory\n",
		ft_strlen("`: Is a directory\n"));
}

/* Prints ": ambiguous redirect\n"*/
void	print_ambig_redir(const char *orig_fn)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, orig_fn, ft_strlen(orig_fn));
	write(STDERR_FILENO, ": ambiguous redirect\n", 21);
}

void	print_hdoc_eof_error(t_lex *l, char *word)
{
	const char		*input = (const char *)lex_get_raw(l);
	size_t			i;
	const size_t	pos = (size_t)(lex_get_ptr(l) - (char *)input);

	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": invalid delimiter `",
		ft_strlen(": invalid delimiter `"));
	ft_putstr_fd(word, STDERR_FILENO);
	write(STDERR_FILENO, "` in:\n", ft_strlen("` in:\n"));
	ft_putstr_fd(input, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
	i = -1;
	while (++i < pos)
		write(STDERR_FILENO, "-", 1);
	write(STDERR_FILENO, "^\n", 2);
}

#define HDOC_1 "warning: here-document at line "
#define HDOC_2 " delimited by end-of-file (wanted `"

void	print_hdoc_error(const char *line, const char *eof)
{
	ft_putstr_fd(HDOC_1, STDERR_FILENO);
	ft_putstr_fd(line, STDERR_FILENO);
	ft_putstr_fd(HDOC_2, STDERR_FILENO);
	ft_putstr_fd(eof, STDERR_FILENO);
	write(STDERR_FILENO, "\')\n", 3);
}
