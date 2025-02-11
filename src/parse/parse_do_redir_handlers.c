#include "parse_int.h"

void	handle_redirect_in(const t_redir_data *node)
{
	const int	append = false;

	if (!node->filename)
		err("No filename for input redirection.\n");
	// TODO handle expansions
	if (-1 == redirect(NULL, node->filename, STDIN_FILENO, append))
		err("Input redirection issue\n");
}

void	handle_heredoc(const t_redir_data *node)
{
	const int	append = false;
	int			fildes[2];

	if (!node->heredoc_body)
		err("No heredoc for input redirection.\n");
	if (pipe(fildes) < 0)
		err("heredoc pipe()");
	write(fildes[1], node->heredoc_body, ft_strlen(node->heredoc_body));
	close(fildes[1]);
	if (-1 == redirect(&fildes[0], NULL, STDIN_FILENO, append))
		err("Heredoc redirection issue\n");
}

void	handle_redirect_out(const t_redir_data *node)
{
	const int	append = false;

	if (!node->filename)
		err("No filename for output redirection.\n");
	// TODO handle expansions
	if (-1 == redirect(NULL, node->filename, STDOUT_FILENO, append))
		err("Output redirection issue\n");
}

void	handle_redirect_append(const t_redir_data *node)
{
	const int	append = true;

	if (!node->filename)
		err("No filename for output redirection.\n");
	// TODO handle expansions
	if (-1 == redirect(NULL, node->filename, STDOUT_FILENO, append))
		err("Append redirection issue\n");
}
