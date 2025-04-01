#include "error.h"

extern char	*get_input(t_state *s);
extern char	*lex_get_ptr(t_lex *l);
extern const char	*lex_get_raw(t_lex *l);
extern void	ft_putstr_fd(char const *s, int fd);
extern char	*ft_strchr(const char *s, int c_in);


void	print_bufflow(void)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, "Input exceeds limits.\n", ft_strlen("Input exceeds limits.\n"));
}

/* Prints errno err for 'dingus' */
void	print_perror(const char *dingus)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	perror(dingus);
}

/* Prints msg for the 'dingus' (no \n added) */
void	print_custom_err(const char *dingus, const char *msg)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, dingus, ft_strlen(dingus));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, msg, ft_strlen(msg));
}

/* Prints msg for the 'dingus'
 * Note: \n will be printed
 */
void	print_custom_err_err(const char *dingus, const char *gizmo,
		const char *msg)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, dingus, ft_strlen(dingus));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, "\'", ft_strlen("\'"));
	write(STDERR_FILENO, gizmo, ft_strlen(gizmo));
	write(STDERR_FILENO, "\'", ft_strlen("\'"));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}

/* Prints ": ambiguous redirect\n"*/
void	print_ambiguous_redirect(const char *orig_fn)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	write(STDERR_FILENO, orig_fn, ft_strlen(orig_fn));
	write(STDERR_FILENO, ": ambiguous redirect\n", 21);
}

/* Prints "cmd: command not found\n"*/
void	print_command_not_found(const char *cmd)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	if (cmd)
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
	else
		write(STDERR_FILENO, "(null)", ft_strlen("(null)"));
	write(STDERR_FILENO, ": command not found\n", 20);
}

/* Prints ": Buffer overflow\n"*/
void	print_lex_buffer_overflow(void)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": Buffer overflow\n", 18);
}

void	print_parse_error(t_state *s, const char *word, size_t pos)
{
	const char	*input = (const char *)get_input(s);
	size_t		i;

	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": parse error near `", ft_strlen(": parse error near `"));
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
	const char 	*alt = "...";
	size_t		i;

	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": expected valid command after `", ft_strlen(": expected valid command after `"));
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
	write(STDERR_FILENO, ": expected valid filename after `", ft_strlen(": expected valid filename after `"));
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
	const char	*input = (const char *)lex_get_raw(l);
	size_t		i;
	const size_t pos = (size_t)(lex_get_ptr(l) - (char *)input);

	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": parse error near `", ft_strlen(": parse error near `"));
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

void	print_is_dir(void)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": Is a directory\n", ft_strlen(": Is a directory\n"));
}
