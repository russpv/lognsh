#include "parse_int.h"

#define DBGMSG_P_ANNOUNCE "Parser: \t###### parse ####### \n"
#define DBGMSG_PNODE_ANNOUNCE "Parser: ######## parse_full_cmd ########\n"
#define DBGMSG_PNODE_GOT "Parser: parse_full_cmd: got tok of %s \n"
#define DBGMSG_PNODE_STOP "Parser: reached end of tokens\n"
#define DBGMSG_PNODE_NOTPROC "Parser: not a proc...\n"
#define DBGMSG_PNODE_NOTCMD "Parser: not a cmd...\n"
#define DBGMSG_PNODE_NOTPIPE "Parser: not a pipe...\n"
#define ERRMSG_PNODE_SYNTAX "Syntax error near: [TODO, empty cmd okay]\n"

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
t_ast_node	*parse_full_cmd(t_state *s, t_parser *p)
{
	debug_print(DBGMSG_PNODE_ANNOUNCE);
	debug_print(DBGMSG_PNODE_GOT, tok_get_raw(peek(p)));
	if (is_at_end(p))
	{
		debug_print(DBGMSG_PNODE_STOP);
		return (NULL);
	}
	if (is_open_paren(peek(p)))
		return (parse_proc(s, p));
	debug_print(DBGMSG_PNODE_NOTPROC);
	if ((is_cmd_token(peek(p)) || is_redir_token(peek(p)))
		&& (st_int_peek(p->st) > 0 || !p->last_node))
		return (parse_cmd(s, p));
	debug_print(DBGMSG_PNODE_NOTCMD);
	if (p->last_node && is_pipe_token(peek(p)))
		return (parse_pipeline(s, p));
	debug_print(DBGMSG_PNODE_NOTPIPE);
	if (p->last_node && is_log_token(peek(p)))
		return (parse_logical(s, p));
	err(ERRMSG_PNODE_SYNTAX);
	p->parse_error = true;
	return (NULL);
}

/* Inserts easily into test function */
t_ast_node	*test_parse(t_state *s, t_parser *parser)
{
	t_ast_node	*ast;

	while (!is_at_end(parser) && !parser->parse_error)
		ast = parse_full_cmd(s, parser);
	parser->ast = ast;
	return (ast);
}

// 
t_ast_node	*parse(t_state *s, char *input)
{
	t_lex		*lexer;
	t_ast_node	*ast;
	t_list		*tokens;
	t_parser	*parser;

	ast = NULL;
	lexer = tokenize(s, input);
	if (NULL == lexer)
		return (set_error(s, ERR_TOKEN), NULL);
	tokens = lex_get_tokens(lexer);
	parser = create_parser(s, tokens);
	debug_print(DBGMSG_P_ANNOUNCE);
	while (!is_at_end(parser) && !parser->parse_error)
		ast = parse_full_cmd(s, parser);
	parser->ast = ast;
	set_parser(s, parser);
	set_lexer(s, lexer);
	return (ast);
}
