#include "parse_int.h"

/* Returns AST node.
 * Validates token type, parses args into list
 * For commands, checks for redirect operator(s) before or after
 * REDIR WORD REDIR WORD
 * 
 * Parses proc when the token is an open parentheses
 * Parses command when: the token starts a new command AND
 * 	it's either in the middle of parsing a group node (stack non empty), or 
 * 	it's the first node
 * Parses pipe when it's none of the above and its currently a pipe token
 * Parses logical when it's none of the above and its currently a logical token
 */
t_ast_node	*parse_full_cmd(t_parser *p)
{
	debug_print("######## parse_full_cmd ########\n");
	debug_print("parse_full_cmd: got tok of %s \n", tok_get_raw(peek(p)));
	if (is_at_end(p))
	{
		debug_print("reached end of tokens\n");
		return (NULL);
	}
	if (is_open_paren(peek(p)))
		return (parse_proc(p));
	debug_print("not a proc...\n");
	if ((is_cmd_token(peek(p)) || is_redir_token(peek(p)))
		&& (st_peek(p->st) > 0 || !p->last_node))
		return (parse_cmd(p));
	debug_print("not a cmd...\n");
	if (p->last_node && is_pipe_token(peek(p)))
		return (parse_pipeline(p));
	debug_print("not a pipe...\n");
	if (p->last_node && is_log_token(peek(p)))
		return (parse_logical(p));
	err("Syntax error near: (TODO, empty cmd okay)\n");
	p->parse_error = true;
	return (NULL);
}

/* Inserts easily into test function */
t_ast_node	*test_parse(t_parser *parser)
{
	t_ast_node	*ast;

	while (!is_at_end(parser) && !parser->parse_error)
		ast = parse_full_cmd(parser);
	parser->ast = ast;
	/*if (true == lex_get_incomplete(lexer))
		tokens = tokenize_more_input(lexer); */
	return (ast);
}

// TODO error handling
t_ast_node	*parse(t_state *s, char *input)
{
	t_lex		*lexer;
	t_ast_node	*ast;
	t_list		*tokens;
	t_parser	*parser;

	lexer = tokenize(s, input);
	if (NULL == lexer)
		return (null_and_stat(s, ERR_TOKEN)); // Ctrl+D abort
	tokens = lex_get_tokens(lexer);
	parser = create_parser(s, tokens);
	while (!is_at_end(parser) && !parser->parse_error)
		ast = parse_full_cmd(parser);
	parser->ast = ast;
	set_parser(s, parser);
	set_lexer(s, lexer);
	return (ast);
}
