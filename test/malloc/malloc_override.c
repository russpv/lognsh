#define _GNU_SOURCE
#include "../../include/minishell.h"  //TODO change as needed
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>  // For dlsym()
/* Must add compile flag '-ldl' */

// Override my_malloc, replace all my_malloc() with this in the test code
// add declaration in header
void* my_malloc(size_t size) {
    static void* (*real_malloc)(size_t) = NULL;

    if (!real_malloc) {
        // Initialize the real my_malloc function using dlsym
        real_malloc = dlsym(RTLD_NEXT, "malloc"); //TODO make sure this isn't replaced
        if (!real_malloc) {
            // Handle error if dlsym fails
            fprintf(stderr, "Error in dlsym: %s\n", dlerror());
            exit(1);
        }
    }
    // Simulate my_malloc failure on specific condition
    static int call_count = 0;
    char *env_call_count = getenv("CALL_COUNT");
	call_count++;
    // If the environment variable CALL_COUNT is set, use it for comparison
    if (env_call_count) {
        int fail_at = atoi(env_call_count);
        fprintf(stderr, "Env CALL_COUNT = %s, call_count = %d\n", env_call_count, call_count);
        // Compare call_count with CALL_COUNT (converted to an integer)
        if (call_count == fail_at) {
            fprintf(stderr, "Failing my_malloc call number: %d\n", call_count);
            return NULL;  // Force my_malloc failure on the ith call
        }
    }
    return real_malloc(size);  // Use the original my_malloc
}
