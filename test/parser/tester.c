// Define the test groups
TEST_GROUP(parser_basic);
TEST_GROUP(parser_operators);
TEST_GROUP(parser_errors);

// Tests for basic parser functionality
TEST_GROUP_RUNNER(parser_basic) {
    RUN_TEST_CASE(parser_basic, test_simple_command);
    RUN_TEST_CASE(parser_basic, test_command_with_arguments);
}

// Tests for parsing operators
TEST_GROUP_RUNNER(parser_operators) {
    RUN_TEST_CASE(parser_operators, test_pipe_operator);
    RUN_TEST_CASE(parser_operators, test_redirection_operator);
}

// Tests for parser error handling
TEST_GROUP_RUNNER(parser_errors) {
    RUN_TEST_CASE(parser_errors, test_invalid_command);
    RUN_TEST_CASE(parser_errors, test_unexpected_token);
}
