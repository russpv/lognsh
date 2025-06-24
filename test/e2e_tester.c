#include "../include/test.h"

#define DEFINE_E2E_TEST(test_name, input) \
 void test_name(void) { \
	printf("\n"); \
     char *my_shell_output = run_my_shell(input); \
     char *bash_output = run_bash(input); \
     TEST_ASSERT_EQUAL_STRING(bash_output, my_shell_output); \
     free(my_shell_output); \
     free(bash_output); \
 }

 #define DEFINE_E2E_ENV_TEST(test_name, input) \
 void test_name(void) { \
     printf("\n"); \
     char *output = run_my_shell(input); \
     TEST_ASSERT_NOT_NULL(strstr(output, "PATH=")); \
     TEST_ASSERT_NOT_NULL(strstr(output, "HOME=")); \
     TEST_ASSERT_NOT_NULL(strstr(output, "USER=")); \
     free(output); \
 }

#define DEFINE_E2E_GLOB_TEST(test_name, input) \
void test_name(void) { \
    printf("\n"); \
    char *my_output = run_my_shell(input); \
    char *bash_output = run_bash(input); \
    StringSet my_set, bash_set; \
    stringset_init(&my_set); \
    stringset_init(&bash_set); \
    stringset_from_string(&my_set, my_output); \
    stringset_from_string(&bash_set, bash_output); \
    TEST_ASSERT_TRUE_MESSAGE(stringset_equal(&my_set, &bash_set), "Globbing outputs differ"); \
}

// Tests for basic parser functionality
DEFINE_E2E_TEST(test_echo, "echo hello world");
DEFINE_E2E_TEST(test_simple_pipe, "ls -l | grep file");
DEFINE_E2E_TEST(test_simple_redirection, "echo test > output.txt");

//syntax, delimiters 
DEFINE_E2E_TEST(test_space_handling_norm, "echo $Hello World");
DEFINE_E2E_TEST(test_space_handling_dq, "echo \"$Hello World\"");
DEFINE_E2E_TEST(test_space_handling_sq, "echo \'$Hello World\'");
DEFINE_E2E_TEST(test_hash_no_space_norm, "echo Hello!@#$%^*World"); 
DEFINE_E2E_TEST(test_hash_no_space_sq, "echo \'Hello!@#$%^*World\'");
DEFINE_E2E_TEST(test_hash_no_space_dq, "echo \"Hello!@#$%^*World\"");

// BUILTINS
DEFINE_E2E_TEST(test_echo_builtin, "echo -n 'Hello World'");
DEFINE_E2E_TEST(test_echo_builtin2, "echo -nnnn 'Hello World'");
DEFINE_E2E_TEST(test_cd_builtin, "cd /home");
DEFINE_E2E_TEST(test_pwd_builtin, "pwd");
DEFINE_E2E_TEST(test_export_builtin1, "export VAR=value");
DEFINE_E2E_TEST(test_export_builtin2, "$VAR");
DEFINE_E2E_TEST(test_unset_builtin1, "unset VAR");
DEFINE_E2E_TEST(test_unset_builtin2, "$VAR");
DEFINE_E2E_ENV_TEST(test_env_builtin, "env");
DEFINE_E2E_TEST(test_exit_builtin, "exit");


// Parse errors 
DEFINE_E2E_TEST(test_missing_command, ">");
DEFINE_E2E_TEST(test_unexpected_token, "echo hello | | echo world");


// quote functionality 
DEFINE_E2E_TEST(test_single_quote_metacharacters, "echo 'hello > world'");
DEFINE_E2E_TEST(test_double_quote_metacharacters, "echo \"hello $USER\"");
DEFINE_E2E_TEST(test_single_quote_special_chars, "echo 'this should escape \\$PATH'");
DEFINE_E2E_TEST(test_double_quote_special_chars, "echo \"this should not escape \\$PATH\"");


// expansion 
DEFINE_E2E_TEST(test_variable_expansion, "echo $HOME");
DEFINE_E2E_TEST(test_variable_expansion_multiple, "echo $HOME $PATH");
DEFINE_E2E_TEST(test_exit_status_expansion, "echo hello");
DEFINE_E2E_TEST(test_exit_status_expansion2, "echo $?");


// globbing 
DEFINE_E2E_GLOB_TEST(test_wildcard_basic, "echo *"); //FAIL, different sort algo
DEFINE_E2E_GLOB_TEST(test_wildcard_in_path, "ls *"); 


// control_flow 
// TODO figure out how to simulate signals

// Redirs 
DEFINE_E2E_TEST(test_input_redirection, "echo hello < input.txt");
DEFINE_E2E_TEST(test_output_redirection, "echo hello > output.txt");
DEFINE_E2E_TEST(test_append_redirection1, "echo hello >> append.txt");
DEFINE_E2E_TEST(test_append_redirection2, "cat append.txt");
DEFINE_E2E_TEST(test_input_redirection_invalid, "echo hello < nonexistent.txt");
DEFINE_E2E_TEST(test_heredoc_no_delim, "echo hello <<");
DEFINE_E2E_TEST(test_redirection_error, "echo hello >");
DEFINE_E2E_TEST(test_redirection_glob, "echo hello > *");
DEFINE_E2E_TEST(test_redirection_expansion, "echo hello > $PATH"); //ambiguous redirect
DEFINE_E2E_TEST(test_heredoc_redirection2, "cat << EOF\n$?$?$\nEOF"); // 00$
DEFINE_E2E_TEST(test_heredoc_redirection3, "cat << 'EO'F\n$?$?$\nEOF"); // $?$?$
DEFINE_E2E_TEST(test_heredoc_redirection4, "cat << EOF\n\\$?\\$?$\\\nEOF"); // $?$?$



// logicals 
DEFINE_E2E_TEST(test_andif_operator, "echo hello && echo world"); 
DEFINE_E2E_TEST(test_andif_fail, "echo hello && nonexistent_command"); 
DEFINE_E2E_TEST(test_orif_operator, "nonexistent_command || echo world");
DEFINE_E2E_TEST(test_orif_fail, "echo hello || echo world"); 

DEFINE_E2E_TEST(test_andif_redirection, "echo hello && echo world > output.txt");
DEFINE_E2E_TEST(test_orif_redirection, "nonexistent_command || echo world > output.txt");

DEFINE_E2E_TEST(test_andif_priority, "echo hello && (echo world || echo foo)"); 
DEFINE_E2E_TEST(test_orif_priority, "echo hello || (echo world && echo foo)"); 


// pipes 
DEFINE_E2E_TEST(test_multiple_pipes, "echo hello | grep he | wc -c");
DEFINE_E2E_TEST(test_empty_pipe, "echo hello | grep nonexistent | wc -c");
DEFINE_E2E_TEST(test_pipe_invalid, "echo hello | | echo world");
DEFINE_E2E_TEST(test_simple_pipe_invalid, "echo hello | nonexistent_command");
DEFINE_E2E_TEST(test_pipe_with_redirection, "echo hello | grep he > result.txt");

// subshells 
DEFINE_E2E_TEST(test_proc_redirection0, "echo /'will be empty/' > in");
DEFINE_E2E_TEST(test_proc_redirection, "(echo haha | cat) >in >me");
DEFINE_E2E_TEST(test_proc_redirection2, "cat me");
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
    RUN_TEST(test_single_quote_metacharacters);
    RUN_TEST(test_double_quote_metacharacters);
    RUN_TEST(test_single_quote_special_chars);
    RUN_TEST(test_double_quote_special_chars);
}

TEST_GROUP_RUNNER(redirection) {
    RUN_TEST(test_input_redirection);
    RUN_TEST(test_output_redirection);
    RUN_TEST(test_append_redirection1);
	RUN_TEST(test_append_redirection2);
    RUN_TEST(test_input_redirection_invalid);
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

TEST_GROUP_RUNNER(delimiters) {
	RUN_TEST(test_space_handling_norm);
	RUN_TEST(test_space_handling_dq);
	RUN_TEST(test_space_handling_sq);
	RUN_TEST(test_hash_no_space_norm);
	RUN_TEST(test_hash_no_space_sq);
	RUN_TEST(test_hash_no_space_dq);
}

TEST_GROUP_RUNNER(builtins) {
    RUN_TEST(test_echo_builtin);
    RUN_TEST(test_cd_builtin);
    RUN_TEST(test_pwd_builtin);
    RUN_TEST(test_export_builtin1);
	RUN_TEST(test_export_builtin2);
    RUN_TEST(test_unset_builtin1);
	RUN_TEST(test_unset_builtin2);
    RUN_TEST(test_env_builtin);
    RUN_TEST(test_exit_builtin);
}

TEST_GROUP_RUNNER(control_flow) {
// TODO
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
    RUN_TEST(test_unexpected_token);
}


int main(void) {
    UNITY_BEGIN();

    printf(UYEL"\n### Running Basic Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(basic);

	printf(UYEL"\n### Running Delimiter Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(delimiters);

    printf(UYEL"\n### Running Quoting Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(quoting);

    printf(UYEL"\n### Running Redirection Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(redirection);

    printf(UYEL"\n### Running Pipe Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(pipe);

    printf(UYEL"\n### Running Variable Expansion Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(variable_expansion);

    printf(UYEL"\n### Running Builtins Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(builtins);

    printf(UYEL"\n### Running Control Flow Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(control_flow);

    printf(UYEL"\n### Running Logical Operators Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(logicals);

    printf(UYEL"\n### Running Wildcard Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(wildcards);

    printf(UYEL"\n### Running Subshell Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(subshells);

    printf("\n### All Tests Complete ###\n");

    return UNITY_END();
}
