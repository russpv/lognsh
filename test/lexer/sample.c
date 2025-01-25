#include "../test_utils/test_utils.h"

void test_tokenize_ls(void) {
    const char *input = "ls -l";

    t_list *tokens = tokenize(input);  // Assuming you have a tokenize function
    TEST_ASSERT_EQUAL_UINT(2, list_length(tokens));  // Check number of tokens
    // Further assertions about the token types or values
}

