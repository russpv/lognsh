#include "../data_structures/llist.h"
#include "parse_int.h"

#define MAX_CMD_ARGS 10
#define OP_OPTION '-'
// Use Strategy pattern for validation functions per built-in command
// Use Visitor pattern for validating all AST nodes
// validate_filename - ensure no '~' '?', length 255 chars, if '*', then no '/'
// 

//RULES
//- The very first token shall be interpreted as a command
//- Next tokens until an operator shall be interpreted as arguments
 //   - If arguments begin with a dash, they are flags/options
//- An operator, if any, must have an expression following

/* 
 * CMD only followed by [ARGS, REDIRS] not Flags (except echo)
 * An ARG only followed by another ARG or OP
 * REDIRS followed by NAME
 * PIPE followed by NAME
 * Certain OPs cannot be at the beginning
 */

bool	is_option(t_tok *tok)
{
	const char *s = tok_get_raw(tok);

	if (s && *s)
		return ((unsigned char)OP_OPTION == s[0]);
	return (false);
}

bool	is_cmd_token(t_tok *tok)
{
	const enum e_tok_type type = get_tok_type(tok);
	return (type == TOK_WORD || type == TOK_NAME || type == TOK_BI || type == TOK_EXIT_STATUS || type == TOK_REDIRECT_APPEND \
	|| type == TOK_REDIRECT_IN || type == TOK_REDIRECT_OUT || type == TOK_HEREDOC_WORD);
}

bool	is_op_token(t_tok *tok)
{
	const enum e_tok_type type = get_tok_type(tok);
	return (type == TOK_REDIRECT_IN || type == TOK_REDIRECT_OUT || type == TOK_REDIRECT_APPEND || type == TOK_PIPE \
			|| type == TOK_ANDIF || type == TOK_ORIF);
}

bool	is_arg_token(t_tok *tok)
{
	const enum e_tok_type type = get_tok_type(tok);
	return (type == TOK_WORD || type == TOK_NAME);
}

void *destroy_cmd_node(t_ast_node *n)
{
	//TODO
	return (NULL);
}

void *destroy_args(t_arg *args, t_ast_node_cmd *cmd)
{
	//TODO
}

/* TODO: add expansion
*/
t_arg *parse_args(t_parser *p, t_ast_node *cmd_node)
{
	t_arg *args = malloc(sizeof(t_list));
	if (!args)
		return (NULL);
	cmd_node->data.cmd.argc = 0;
	while (!is_at_end(p) && is_arg_token(peek(p)))
	{
		t_arg *arg = malloc(sizeof(t_arg));
		if (!arg)
			return (destroy_cmd_node(cmd_node));
		t_tok *arg_token = advance(p);
		arg->raw = tok_get_raw(arg_token);
		arg->option = is_option(arg_token);
		arg->do_globbing = tok_get_globbing(arg_token);
		arg->do_expansion = tok_get_expansion(arg_token);
		t_list *new = ft_lstnew(arg);
		if (!new)
			return (destroy_args(arg, cmd_node));
		ft_lstadd_back(cmd_node->data.cmd.args, new);
		cmd_node->data.cmd.argc++;
	}
	return (args);
}

t_ast_node *parse_operators(t_parser *p, t_ast_node *left_cmd)
{
	const t_tok *op_token = advance(p);
	if (true == is_op_token(peek(p)))
	{
		t_ast_node *op_node = malloc(sizeof(t_ast_node));
		if (op_node)
		{
			//branch based on which operator
			//do_redirs
			//do_logicals
			//do_pipe
		}
	}
	else
	{
		debug_print("ERROR: Only operators may follow commands.\n");
		kill_parser(p);
	}
}

t_ast_node *_test_parens(t_parser *p, t_ast_node *cmd_node)
{
	cmd_node->type = AST_NODE_CMD; 
	if (get_tok_type(peek(p)) == TOK_OPEN_PAREN && !is_at_end(p))
	{
		if (1 == push(p->stack))
		{
			kill_parser(p); // too much nesting
			return (NULL);
		}
		t_tok *next_cmd_token = advance(p);
		return (parse_cmd(p)); // TODO, if NULL, then ...
	}
	if (get_tok_type(peek(p)) == TOK_CLOSE_PAREN)
	{
		if (1 == pop(p->stack))
		{
			kill_parser(p); //unmatched '('
			return (NULL);
		}
		return (cmd_node);
	}
}

/* Returns cmd node.
 * Validates token type, parses args into list
 * checks for redirect operator(s) before or after
 * REDIR WORD REDIR WORD
 */
t_ast_node *parse_cmd(t_parser *p)
{
	const t_tok *cmd_token = advance(p);

	if (!is_cmd_token(cmd_token))
	{
		debug_print("WARNING: First token is not a command.\n");
		return (NULL); // KEEP?
	}
	t_ast_node *cmd_node = malloc(sizeof(t_ast_node));
	if (cmd_node)
	{
		cmd_node = _test_parens(p, cmd_node);
		cmd_node->data.cmd.name = tok_get_raw(cmd_token);
		cmd_node->data.cmd.args = parse_args(p, cmd_node);
		if (is_at_end(p))
			return (cmd_node);
		t_ast_node *next = parse_operators(p, cmd_node);
		if (next)
			return (next);
	}
	return (cmd_node);
}

bool	match(t_parser *p, enum e_tok_type type)
{
	if (is_at_end(p))
		return (false);
	if (get_tok_type(p->current_tok) == type)
	{
		advance(p);
		return (true);
	}
	return (false);
}

t_tok	*peek(t_parser *p)
{
	return (p->current_tok);
}

t_tok	*previous(t_parser *p)
{
	return (p->current_tok->prev);
}

bool	is_at_end(t_parser *p)
{
	return (p->current_idx >= p->token_count || \
		tok_get_type(p->current_tok->content) == TOK_EOF);
}

/* Moves ptr and returns previous object */
t_tok	*advance(t_parser *p)
{
	const t_list *tmp = p->current_tok;

	if (false == is_at_end(p))
	{
		p->current_idx++;
		p->current_tok = p->current_tok->next;
	}
	return (tmp);
}

t_parser *create_parser(t_list *tokens)
{
	if (!tokens)
		return (NULL);
	t_parser *p = (t_parser *)malloc(sizeof(t_parser));
	if (p)
	{
		p->tokens = tokens;
		p->current_idx = -1;
		p->current_tok = tokens;
		p->token_count = ft_lstsize(tokens);
		p->curr_cmd = NULL;
	}
	return (p);
}


t_ast_node	*parse(t_state s, char *input)
{
	t_lex	*lexer;
	t_ast_node	*ast;
	t_list	*tokens;
	t_parser *parser;

	lexer = tokenize(input);
	if (NULL == lexer)
		return (null_and_stat(s, ERR_TOKEN)); // Ctrl+D abort
	tokens = lex_get_tokens(lexer);
	parser = create_parser(tokens);
	ast = parse_cmd(parser);
	/*if (true == lex_get_incomplete(lexer))
		tokens = tokenize_more_input(lexer); */
	return (ast);
}

// For debugging
void	parse_print(t_ast_node *ast)
{
	// TODO call printer methods
	// ast_traverse
}
