#include "error.h"

void	pbufflow(const char *msg)
{
	const int	fd = STDERR_FILENO;

	write(fd, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(fd, ": ", ft_strlen(": "));
	if (NULL == msg)
		ft_putstr_fd("Input exceeds limits.\n", fd);
	else
	{
		ft_putstr_fd("Input exceeds limits:\n", fd);
		ft_putstr_fd(msg, fd);
		write(fd, "\n", 1);
	}
}

/* Prints ": Buffer overflow\n"*/
void	print_lex_buffer_overflow(void)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": Buffer overflow\n", 18);
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

int	get_wincols(void)
{
	struct winsize	ws;

	if (-1 == ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws))
	{
		perror("ioctl");
		return (ERR_GENERAL);
	}
	return (ws.ws_row);
}

void	print_inv_cmd(void)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	ft_putstr_fd(": Invalid command.\n", STDERR_FILENO);
}

/* Parsing */

void	print_parse_error(t_state *s, const char *word, size_t pos)
{
	const char	*input = (const char *)get_input(s);
	size_t		i;

	if (!input)
		return ;
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": parse error near `",
		ft_strlen(": parse error near `"));
	if (word)
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

	if (!input)
		return ;
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

void	print_lex_error(t_lex *l, char *word)
{
	const char		*input = (const char *)lex_get_raw(l);
	size_t			i;
	const size_t	pos = (size_t)(lex_get_ptr(l) - (char *)input);

	if (!input)
		return ;
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

void	print_value_toolong(void)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	ft_putstr_fd(": Value too long.\n", STDERR_FILENO);
}

/* Redirects */

void	print_redir_error(t_state *s, const char *word, size_t pos)
{
	const char	*input = (const char *)get_input(s);
	size_t		i;

	if (!input)
		return ;
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

void	print_parse_redir_error(t_state *s, size_t pos)
{
	const char		*input = (const char *)get_input(s);
	size_t			i;
	const size_t	cols = (size_t)get_wincols();

	if (!input)
		return ;
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

/* Heredocs */

void	print_hdoc_eof_error(t_lex *l, char *word)
{
	const char		*input = (const char *)lex_get_raw(l);
	size_t			i;
	const size_t	pos = (size_t)(lex_get_ptr(l) - (char *)input);

	if (!input)
		return ;
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

/* More */

void	print_dne(const char *path)
{
	ft_putstr_fd("warning: Path '", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd("' does not exist\n", STDERR_FILENO);
}

// Don't like the unix error msg
void	print_redirect_warning(char *topath)
{
	write(STDERR_FILENO, "warning: An error occurred while redirecting file '",
		ft_strlen("warning: An error occurred while redirecting file '"));
	write(STDERR_FILENO, topath, ft_strlen(topath));
	write(STDERR_FILENO, "' \n", 3);
}

/* Prints "too many arguments"*/
void	print_too_many_args(const char *caller)
{
	write(STDERR_FILENO, SHELL_NAME ": ", ft_strlen(SHELL_NAME) + 2);
	if (caller)
	{
		write(STDERR_FILENO, caller, ft_strlen(caller));
		write(STDERR_FILENO, ": ", ft_strlen(": "));
	}
	write(STDERR_FILENO, "too many arguments\n",
		ft_strlen("too many arguments\n"));
}

void	print_invalid_name(const char *caller, const char *name)
{
	write(STDERR_FILENO, caller, ft_strnlen(caller, MAX_NAME_LEN));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, name, ft_strnlen(name, MAX_NAME_LEN));
	write(STDERR_FILENO, ": invalid parameter name\n",
		ft_strlen(": invalid parameter name\n"));
}

bool	is_error(int v)
{
	if (v <= ERR_STAT && v >= ERR_GENERAL)
		return (true);
	else if (v == ERR_CHILD_SIGINT)
		return (true);
	else if (v == ERR_CMD_NOT_EXEC)
		return (true);
	else if (v == ERR_CMD_NOT_FOUND)
		return (true);
	else if (v == ERR_EXIT_RANGE)
		return (true);
	return (false);
}

/* Custom */


void	print_gen_err(const char *msg)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	ft_putstr_fd(msg, STDERR_FILENO);
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
	if (isatty(STDERR_FILENO))
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
}

void	print_filename_missing(const char *name)
{
	write(STDERR_FILENO, SHELL_NAME, ft_strlen(SHELL_NAME));
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": filename argument required\n", STDERR_FILENO);
}
