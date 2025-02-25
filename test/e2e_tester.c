#include "../include/test.h"

//TEST_GROUP(basic);
//TEST_GROUP(expansions_advanced);
//TEST_GROUP(compound_advanced);

// Tests for basic parser functionality

DEFINE_E2E_TEST(test_echo, "echo hello world");
DEFINE_E2E_TEST(test_simple_pipe, "ls -l | grep file");
DEFINE_E2E_TEST(test_simple_redirection, "echo test > output.txt");


/* Parse errors */
DEFINE_E2E_TEST(test_missing_command, ">");
DEFINE_E2E_TEST(test_unmatched_quotes, "echo 'missing quote");
DEFINE_E2E_TEST(test_unexpected_token, "echo hello | | echo world");


/* quote functionality */
DEFINE_E2E_TEST(test_single_quote_open, "echo 'unclosed quote");
DEFINE_E2E_TEST(test_double_quote_open, "echo \"unclosed quote");
DEFINE_E2E_TEST(test_single_quote_metacharacters, "echo 'hello > world'");
DEFINE_E2E_TEST(test_double_quote_metacharacters, "echo \"hello $USER\""); // FAIL, env is not working
DEFINE_E2E_TEST(test_single_quote_special_chars, "echo 'this should not escape \;'");
DEFINE_E2E_TEST(test_double_quote_special_chars, "echo \"this should escape \$PATH\""); // FAIL, env is not working


/* expansion */
DEFINE_E2E_TEST(test_variable_expansion, "echo $HOME");
DEFINE_E2E_TEST(test_variable_expansion_multiple, "echo $HOME $PATH");
DEFINE_E2E_TEST(test_exit_status_expansion, "echo hello"); 
DEFINE_E2E_TEST(test_exit_status_expansion2, "echo $?");  // Ensure $? works after command execution


/* globbing */
DEFINE_E2E_TEST(test_wildcard_basic, "echo *"); //FAIL
DEFINE_E2E_TEST(test_wildcard_in_path, "ls *");


/* control_flow */
DEFINE_E2E_TEST(test_ctrl_c_behavior, "echo hello <ctrl-C> echo world");  // Ensure ctrl-C interrupts properly
DEFINE_E2E_TEST(test_ctrl_d_behavior, "echo hello <ctrl-D>");  // ctrl-D should exit
DEFINE_E2E_TEST(test_ctrl_backslash_behavior, "echo hello <ctrl-\\> echo world");  // ctrl-\ does nothing


/* builtins */
DEFINE_E2E_TEST(test_echo_builtin, "echo -n 'Hello World'");
DEFINE_E2E_TEST(test_echo_builtin2, "echo -nnnn 'Hello World'");
DEFINE_E2E_TEST(test_cd_builtin, "cd /home");
DEFINE_E2E_TEST(test_pwd_builtin, "pwd");
DEFINE_E2E_TEST(test_export_builtin, "export VAR=value"); //TODO an actual test
DEFINE_E2E_TEST(test_unset_builtin, "unset VAR"); //TODO an actual test
DEFINE_E2E_TEST(test_env_builtin, "env");
DEFINE_E2E_TEST(test_exit_builtin, "exit");


/* Redirs */
DEFINE_E2E_TEST(test_input_redirection, "echo hello < input.txt"); // FAIL, need to halt command if redir fails
DEFINE_E2E_TEST(test_output_redirection, "echo hello > output.txt");
DEFINE_E2E_TEST(test_append_redirection, "echo hello >> append.txt");
DEFINE_E2E_TEST(test_input_redirection_invalid, "echo hello < nonexistent.txt");
DEFINE_E2E_TEST(test_heredoc_redirection, "echo hello << EOF\nworld\nEOF");
DEFINE_E2E_TEST(test_heredoc_no_delim, "echo hello <<"); 
DEFINE_E2E_TEST(test_redirection_error, "echo hello >");
DEFINE_E2E_TEST(test_redirection_glob, "echo hello > *");
DEFINE_E2E_TEST(test_redirection_expansion, "echo hello > $PATH"); //ambiguous redirect
DEFINE_E2E_TEST(test_heredoc_redirection2, "cat << EOF\n$?$?$\nEOF"); // 00$
DEFINE_E2E_TEST(test_heredoc_redirection3, "cat << 'EO'F\n$?$?$\nEOF"); // $?$?$
DEFINE_E2E_TEST(test_heredoc_redirection4, "cat << EOF\n\\$?\\$?$\\\nEOF"); // $?$?$



/* logicals */
DEFINE_E2E_TEST(test_andif_operator, "echo hello && echo world");
DEFINE_E2E_TEST(test_andif_fail, "echo hello && nonexistent_command");
DEFINE_E2E_TEST(test_orif_operator, "nonexistent_command || echo world");
DEFINE_E2E_TEST(test_orif_fail, "echo hello || echo world");

DEFINE_E2E_TEST(test_andif_redirection, "echo hello && echo world > output.txt");
DEFINE_E2E_TEST(test_orif_redirection, "nonexistent_command || echo world > output.txt");

DEFINE_E2E_TEST(test_andif_priority, "echo hello && (echo world || echo foo)");
DEFINE_E2E_TEST(test_orif_priority, "echo hello || (echo world && echo foo)");


/* pipes */
DEFINE_E2E_TEST(test_multiple_pipes, "echo hello | grep he | wc -c");
DEFINE_E2E_TEST(test_empty_pipe, "echo hello | grep nonexistent | wc -c");
DEFINE_E2E_TEST(test_pipe_invalid, "echo hello | | echo world");
DEFINE_E2E_TEST(test_simple_pipe_invalid, "echo hello | nonexistent_command");
DEFINE_E2E_TEST(test_pipe_with_redirection, "echo hello | grep he > result.txt");


/* syntax */


/* subshells */
DEFINE_E2E_TEST(test_proc_redirection0, "echo /'will be empty/' > in");
DEFINE_E2E_TEST(test_proc_redirection, "(echo haha | cat) >in >crap");
DEFINE_E2E_TEST(test_proc_redirection2, "cat crap");
DEFINE_E2E_TEST(test_proc_redirection3, "cat in");


TEST_GROUP_RUNNER(basic) {
    RUN_TEST(test_echo);
    RUN_TEST(test_simple_pipe);
    RUN_TEST(test_simple_redirection);
}

TEST_GROUP_RUNNER(subshells) {
    RUN_TEST(test_proc_redirection0);
    RUN_TEST(test_proc_redirection);
    RUN_TEST(test_proc_redirection2);
    RUN_TEST(test_proc_redirection3);
}

TEST_GROUP_RUNNER(quoting) {
    RUN_TEST(test_single_quote_open);
    RUN_TEST(test_double_quote_open);
    RUN_TEST(test_single_quote_metacharacters);
    RUN_TEST(test_double_quote_metacharacters);
    RUN_TEST(test_single_quote_special_chars);
    RUN_TEST(test_double_quote_special_chars);
}

TEST_GROUP_RUNNER(redirection) {
    RUN_TEST(test_input_redirection);
    RUN_TEST(test_output_redirection);
    RUN_TEST(test_append_redirection);
    RUN_TEST(test_input_redirection_invalid);
    RUN_TEST(test_heredoc_redirection);
    RUN_TEST(test_heredoc_no_delim);
    RUN_TEST(test_redirection_error);
    RUN_TEST(test_redirection_glob);
    RUN_TEST(test_redirection_expansion);
}

TEST_GROUP_RUNNER(pipe) {
    RUN_TEST(test_simple_pipe_invalid);
    RUN_TEST(test_pipe_with_redirection);
}

TEST_GROUP_RUNNER(variable_expansion) {
    RUN_TEST(test_variable_expansion);
    RUN_TEST(test_variable_expansion_multiple);
    RUN_TEST(test_exit_status_expansion);
}

TEST_GROUP_RUNNER(builtins) {
    RUN_TEST(test_echo_builtin);
    RUN_TEST(test_cd_builtin);
    RUN_TEST(test_pwd_builtin);
    RUN_TEST(test_export_builtin);
    RUN_TEST(test_unset_builtin);
    RUN_TEST(test_env_builtin);
    RUN_TEST(test_exit_builtin);
}

TEST_GROUP_RUNNER(control_flow) {
    RUN_TEST(test_ctrl_c_behavior);
    RUN_TEST(test_ctrl_d_behavior);
    RUN_TEST(test_ctrl_backslash_behavior);
}

TEST_GROUP_RUNNER(logicals) {
    RUN_TEST(test_andif_operator);
    RUN_TEST(test_andif_fail);
    RUN_TEST(test_orif_operator);
    RUN_TEST(test_orif_fail);
    RUN_TEST(test_andif_redirection);
    RUN_TEST(test_orif_redirection);
    RUN_TEST(test_andif_priority);
    RUN_TEST(test_orif_priority);
}

TEST_GROUP_RUNNER(wildcards) {
    RUN_TEST(test_wildcard_basic);
    RUN_TEST(test_wildcard_in_path);
}

TEST_GROUP_RUNNER(errors) {
    RUN_TEST(test_missing_command);
    RUN_TEST(test_unmatched_quotes);
    RUN_TEST(test_unexpected_token);
}

/*
// Tests for parsing operators
TEST_GROUP_RUNNER(expansions_advanced) {
    RUN_E2E_TEST(test_no_command, "<$HOME");
    RUN_E2E_TEST(test_basic_expansion, "$PATH");
}

// Tests for parser error handling
TEST_GROUP_RUNNER(compound_basic) {
    RUN_E2E_TEST(test_andif, "echo haha && echo bobo && echo hoho");
    RUN_E2E_TEST(test_orif, "echo haha || echo bobo");
}
*/

int main(void) {
    UNITY_BEGIN();

    printf(CYAN"### Running Basic Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(basic);
    
    printf(UYEL"### Running Quoting Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(quoting);
    
    printf(UYEL"### Running Redirection Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(redirection);
    
    printf(UYEL"### Running Pipe Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(pipe);
    
    printf(UYEL"### Running Variable Expansion Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(variable_expansion);
    
    printf(UYEL"### Running Builtins Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(builtins);
    
    printf(UYEL"### Running Control Flow Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(control_flow);
    
    printf(UYEL"### Running Logical Operators Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(logicals);
    
    printf(UYEL"### Running Wildcard Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(wildcards);

    printf(UYEL"### Running Subshell Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(subshells);

    printf("\n### All Tests Complete ###\n");

    return UNITY_END();
}


