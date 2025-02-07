#include "../include/test.h"

//TEST_GROUP(basic);
//TEST_GROUP(expansions_advanced);
//TEST_GROUP(compound_advanced);

// Tests for basic parser functionality

DEFINE_E2E_TEST(test_echo, "echo hello world");
DEFINE_E2E_TEST(test_pipe, "ls -l | grep file");
DEFINE_E2E_TEST(test_redirection, "echo test > output.txt");
DEFINE_E2E_TEST(test_echo_flag, "echo -nnnnnn haha");


TEST_GROUP_RUNNER(basic) {
	RUN_TEST(test_echo);
	RUN_TEST(test_pipe);
	RUN_TEST(test_redirection);
    RUN_TEST(test_echo_flag);
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
	RUN_TEST_GROUP(basic);
	//RUN_TEST_GROUP(expansions_advanced);
	//RUN_TEST_GROUP(compound_basic);
    return UNITY_END();
}
