#include <stdio.h>
#include <stdbool.h>

/* Match char for char until '*', then until the char after, or return true if nothing after 
 * Assumes pattern will contain '*'
 * An empty string will return false.
 */
bool	matches_pattern(const char *s, const char *pattern)
{
	while (*s && *pattern)
	{
		if ('*' == *pattern)
		{
			pattern++;
			if (*pattern)
			{
				while (*s)
				{
					if (true == matches_pattern(s, pattern))
						return (true);
					s++;
				}
				return (false);
			}
			else
				return (true);
		}
		else if (*s != *pattern)
			return (false);
		s++;
		pattern++;
	}
	return (!(*s || *pattern));
}

bool matches_pattern(const char *s, const char *pattern);

void run_test(const char *s, const char *pattern, bool expected) {
    bool result = matches_pattern(s, pattern);
    printf("matches_pattern(\"%s\", \"%s\") -> %d (Expected: %d) %s\n",
           s, pattern, result, expected, result == expected ? "✅" : "❌");
}

int main() {
    // Basic Wildcard Matching
    run_test("hello.c", "*.c", true);
    run_test("file.txt", "*.txt", true);
    run_test("document.pdf", "*.txt", false);

    // Matching Entire Strings
    run_test("hello", "hello", true);
    run_test("hello", "world", false);
    run_test("abc", "a*c", true);
    run_test("abc", "a*d", false);

    // '*' at Start
    run_test("hello", "*", true);
    run_test("world", "*", true);
    run_test("test123", "*123", true);
    run_test("123test", "*123", false);

    // '*' at End
    run_test("abcdef", "abc*", true);
    run_test("abcdef", "def*", false);
    run_test("xyz", "x*", true);
    run_test("xyz", "y*", false);

    // Multiple '*' in Pattern
    run_test("abcdef", "a*f", true);
    run_test("abcdef", "a*d*f", true);
    run_test("abcdef", "a*x*f", false);

    // Edge Cases
    run_test("", "*", true);
    run_test("", "", true);
    run_test("hello", "", false);
    run_test("", "hello", false);

    return 0;
}
