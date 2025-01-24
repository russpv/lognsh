#include "parse_int.h"

/* Consumes redir tokens and adds them to command node linked list
 * Returns NULL if syntax error
 * Heredocs already turned into TOK_HEREDOC_WORD
 * TODO: handle TOK_HEREDOC_WORDs
 */

t_list	*parse_redir(t_parser *p, t_ast_node *cmd)
{
	t_redir_data	*red;
	t_list			*new;

	if (!cmd)
	{
		err("Invalid cmd\n");
		return (NULL);
	}
	while (!is_at_end(p) && is_redir_token(peek(p)))
	{
		red = malloc(sizeof(t_redir_data));
		if (!red)
		{
			err("Memory allocation failed for redirection data\n");
			return (NULL);
		}
		red->cmd = &cmd->data.cmd;
		const t_tok		*tok = advance(p);
		red->type = tok_get_type((t_tok *)tok);
		if (!is_heredoc_token((t_tok *)tok))
		{
			red->doc = NULL;
			red->symbol = tok_get_raw((t_tok *)tok);
			debug_print("Redirection: type=%d symbol=%s\n", red->type,
				red->symbol);
			if (is_at_end(p))
			{
				err("Expected a filename after redirection operator\n");
				free(red);
				return (NULL);
			}
			const t_tok		*name = advance(p);
			if (!is_filename_token((t_tok*)name))
			{
				err("Expected a filename after redirection operator\n");
				free(red);
				return (NULL);
			}
			red->filename = tok_get_raw((t_tok *)name);
			if (!red->filename)
			{
				err("Redirection filename is invalid\n");
				free(red);
				return (NULL);
			}
		}
		else
		{
			debug_print("Got heredoc document\n");
			red->symbol = NULL;
			red->filename = NULL;
			red->doc = tok_get_raw((t_tok *)tok);
		}
		new = ft_lstnew(red);
		if (!new)
		{
			err("Failed to create execution node for redirection\n");
			free(red);
			return (NULL);
		}
		debug_print("Adding redirection: (%s %s | %s)\n", red->symbol, \
			red->filename, red->doc);
		ft_lstadd_back(&cmd->data.cmd.redirs, new);
		cmd->data.cmd.redc++;
	}
	return (cmd->data.cmd.redirs);
}

