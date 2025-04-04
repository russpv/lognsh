#include "parse_int.h"


#define NOFN "No filename for input redirection.\n"

extern int	redirect(int *to, char *topath, int from, bool ifappend);

int	handle_redirect_in(const t_redir_data *node)
{
	const int	append = false;

	if (!node->filename)
		return (err(NOFN), ERR_ARGS);
	if (0 != redirect(NULL, node->filename, STDIN_FILENO, append))
		return (err("Input redirection issue\n"), ERR_REDIR);
	return (0);
}

int	handle_heredoc(const t_redir_data *node)
{
	const int	append = false;
	int			fildes[2];

	if (!node->heredoc_body)
		return (err("No heredoc for input redirection.\n"), ERR_ARGS);
	if (pipe(fildes) < 0)
		return (perror(EMSG_PIPE), ERR_PIPE);
	write(fildes[1], node->heredoc_body, ft_strlen(node->heredoc_body));
	if (0 != close(fildes[1]))
		return (perror(EMSG_CLOSE), ERR_CLOSE);
	if (0 != redirect(&fildes[0], NULL, STDIN_FILENO, append))
		return (err("Heredoc redirection issue\n"), ERR_REDIR);
	return (0);
}

int	handle_redirect_out(const t_redir_data *node)
{
	const int	append = false;

	if (!node->filename)
		return (err(NOFN), ERR_ARGS);
	if (0 != redirect(NULL, node->filename, STDOUT_FILENO, append))
		return (err("Output redirection issue\n"), ERR_REDIR);
	return (0);
}

int	handle_redirect_append(const t_redir_data *node)
{
	const int	append = true;

	if (!node->filename)
		return (err(NOFN), ERR_ARGS);
	if (0 != redirect(NULL, node->filename, STDOUT_FILENO, append))
		return (err("Append redirection issue\n"), ERR_REDIR);
	return (0);
}
