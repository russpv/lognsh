#include "../test_utils/test_utils.h"

#define RUN_PARSER_TEST(test_name, input, expected_ast) \
    void test_name(void) { \
        t_parser *parser = create_and_parse(input); \
        t_ast_node *ast = parse_command(parser); \
        assert_ast_equal(expected_ast, ast); \
        destroy_parser_and_tokens(parser); \
        destroy_ast(expected_ast); \
    }

RUN_PARSER_TEST(test_parse_ls_command, "ls -l", create_simple_command_ast("ls", "-l"));
RUN_PARSER_TEST(test_parse_echo_command, "echo hello", create_simple_command_ast("echo", "hello"));


void test_parse_ls_command(void) {
    const char *input = "ls -l";

    t_parser *parser = create_and_parse(input);
    t_ast_node *ast = parse_command(parser);

    t_ast_node *expected_ast = create_simple_command_ast("ls", "-l");
    assert_ast_equal(expected_ast, ast);

    destroy_parser_and_tokens(parser);
    destroy_ast(expected_ast);
}


void test_parse_pipe_operator(void) {
    const char *input = "ls -l | grep file";

    t_parser *parser = create_and_parse(input);
    t_ast_node *ast = parse_pipeline(parser);

    t_ast_node *expected_ast = create_pipeline_ast("ls -l", "grep file");
    assert_ast_equal(expected_ast, ast);

    destroy_parser_and_tokens(parser);
    destroy_ast(expected_ast);
}

