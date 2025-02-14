#include "../include/test.h"

//TEST_GROUP(basic);
//TEST_GROUP(expansions_advanced);
//TEST_GROUP(compound_advanced);

// Tests for basic parser functionality

DEFINE_E2E_TEST(test_echo, "echo hello world");
DEFINE_E2E_TEST(test_simple_pipe, "ls -l | grep file");
DEFINE_E2E_TEST(test_simple_redirection, "echo test > output.txt");
DEFINE_E2E_TEST(test_echo_flag, "echo -nnnnnn haha");

/* logicals */
DEFINE_E2E_TEST(test_andif_operator, "echo hello && echo world");
DEFINE_E2E_TEST(test_andif_fail, "echo hello && nonexistent_command");
DEFINE_E2E_TEST(test_orif_operator, "nonexistent_command || echo world");
DEFINE_E2E_TEST(test_orif_fail, "echo hello || echo world");

DEFINE_E2E_TEST(test_andif_redirection, "echo hello && echo world > output.txt");
DEFINE_E2E_TEST(test_orif_redirection, "nonexistent_command || echo world > output.txt");

/* pipes */
DEFINE_E2E_TEST(test_multiple_pipes, "echo hello | grep he | wc -c");
DEFINE_E2E_TEST(test_empty_pipe, "echo hello | grep nonexistent | wc -c");
DEFINE_E2E_TEST(test_pipe_invalid, "echo hello | | echo world");

/* syntax */
DEFINE_E2E_TEST(test_redirection_error, "echo hello >");


/* subshells */
DEFINE_E2E_TEST(test_proc_redirection0, "echo /'will be empty/' > in");
DEFINE_E2E_TEST(test_proc_redirection, "(echo haha | cat) >in >crap");
DEFINE_E2E_TEST(test_proc_redirection2, "cat crap");
DEFINE_E2E_TEST(test_proc_redirection3, "cat in");


TEST_GROUP_RUNNER(basic) {
	RUN_TEST(test_echo);
    RUN_TEST(test_echo_flag);
	RUN_TEST(test_simple_pipe);
	RUN_TEST(test_simple_redirection);
}

TEST_GROUP_RUNNER(pipes) {
    RUN_TEST(test_multiple_pipes);
    RUN_TEST(test_empty_pipe);
    RUN_TEST(test_pipe_invalid);
}

TEST_GROUP_RUNNER(logicals) {
    RUN_TEST(test_andif_operator);
    RUN_TEST(test_andif_fail);
    RUN_TEST(test_orif_operator);
    RUN_TEST(test_orif_fail);
    RUN_TEST(test_andif_redirection);
    RUN_TEST(test_orif_redirection);
}

TEST_GROUP_RUNNER(subshells) {
    RUN_TEST(test_proc_redirection0);
    RUN_TEST(test_proc_redirection);
    RUN_TEST(test_proc_redirection2);
    RUN_TEST(test_proc_redirection3);
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
    
    printf(UYEL"### Running Pipes Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(pipes);
    
    printf(UYEL"### Running Logical Operators Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(logicals);
    
    printf(UYEL"### Running Subshells Tests ###\n"RESET);
    fflush(stdout);
    RUN_TEST_GROUP(subshells);
    
    // Uncomment and add other test groups as needed
    // printf("### Running Expansions Advanced Tests ###\n");
    // RUN_TEST_GROUP(expansions_advanced);
    
    printf("\n### All Tests Complete ###\n");

    return UNITY_END();
}

