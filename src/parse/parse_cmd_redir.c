#include "parse_int.h"

/*
// Parses a single redirection token and returns the parsed redirection data.
static t_redir_data	*parse_redir_token(t_parser *p, t_ast_node *cmd)
{

	return (red);
}

// Parses the filename after a redirection operator.
static t_redir_data	*parse_filename(t_parser *p, t_redir_data *red)
{
	t_tok	*name;

	return (red);
}

// Handles the case of a heredoc token.
static t_redir_data	*parse_heredoc(t_redir_data *red, \
		const t_tok *tok)
{
	debug_print("Got heredoc document\n");

	return (red);
}

// Creates and adds the redirection data to the command's redirection list.
static int	add_redirection_to_cmd(t_ast_node *cmd, t_redir_data *red)
{

	return (1);
}

// Main function for parsing redirection tokens.
static t_list	*_parse_redir(t_parser *p, t_ast_node *cmd)
{
	t_redir_data	*red;
	t_tok			*tok;

	if (!cmd)
	{
		err("Invalid cmd\n");
		return (NULL);
	}
	while (!is_at_end(p) && is_redir_token(peek(p)))
	{
		red = parse_redir_token(p, cmd);
		if (!red)
			return (NULL);
		tok = advance(p);
		if (!is_heredoc_token((t_tok *)tok))
		{
			red = parse_filename(p, red);
			if (!red)
				return (NULL);
		}
		else
			red = parse_heredoc(red, tok);
		if (!add_redirection_to_cmd(cmd, red))
			return (NULL);
	}
	return (cmd->data.cmd.redirs);
}
*/
/* Consumes redir tokens and adds them to command node linked list
 * Returns NULL if syntax error
 * Heredocs already turned into TOK_HEREDOC_WORD
 * TODO: handle TOK_HEREDOC_WORDs
 */

static t_list	*_parse_redir(t_parser *p, t_ast_node *cmd)
{
	t_redir_data	*red;
	t_list			*new;
	t_tok			*tok;
	t_tok			*name;

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
		tok = advance(p);
		red->type = tok_get_type((t_tok *)tok);
		red->do_globbing = false;
		red->do_expansion = false;
		if (!is_heredoc_token((t_tok *)tok))
		{
			red->doc = NULL;
			red->symbol = ft_strdup(tok_get_raw((t_tok *)tok));
			if (!red->symbol)
				return (destroy_redir(red), err("ERR malloc\n"), NULL);
			debug_print("Redirection: type=%d symbol=%s\n", red->type,
				red->symbol);
			if (is_at_end(p))
			{
				err("Expected a filename after redirection operator\n");
				free(red);
				return (NULL);
			}
			name = advance(p);
			if (!is_filename_token((t_tok *)name) && !is_expansion((t_tok *)name))
			{
				err("Expected a filename after redirection operator\n");
				free(red);
				return (NULL);
			}
			red->do_globbing = tok_get_globbing((t_tok *)name);
			red->do_expansion = tok_get_expansion((t_tok *)name);
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
		debug_print("Adding redirection: (%s %s | doc:%s glob:%d exp:%d)\n", red->symbol,
			red->filename, red->doc, red->do_globbing, red->do_expansion);
		ft_lstadd_back(&cmd->data.cmd.redirs, new);
		cmd->data.cmd.redc++;
	}
	return (cmd->data.cmd.redirs);
}

int	process_redir(t_parser *p, t_ast_node *cmd_node)
{
	if (is_redir_token(peek(p)))
	{
		if (!_parse_redir(p, cmd_node))
		{
			free(cmd_node);
			err("Failed to parse redirection\n");
			return (1);
		}
	}
	return (0);
}
