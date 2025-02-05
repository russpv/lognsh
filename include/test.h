#ifndef TEST_H
# define TEST_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "minishell.h"
# include "../external/unity/unity.h"
# include "../external/unity/unity_internals.h"
# include "../external/unity/unity_fixture.h"
# include "../external/unity/unity_fixture_internals.h"

# ifdef TESTMODE

#  define MYTEST 1
# else
#  define MYTEST 0
# endif

#define SHELL_EXE "./minish"

#define RUN_E2E_TEST(test_name, input) \
    void test_name(void) { \
        char *my_shell_output = run_my_shell(input); \
        char *bash_output = run_bash(input); \
        TEST_ASSERT_EQUAL_STRING(bash_output, my_shell_output); \
        free(my_shell_output); \
        free(bash_output); \
    }

char *run_bash(const char *cmd);
char *run_my_shell(const char *cmd);


#endif
