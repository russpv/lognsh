#ifndef TEST_H
# define TEST_H

# include "../external/unity/unity.h"
# include "../external/unity/unity_fixture.h"
# include "../external/unity/unity_fixture_internals.h"
# include "../external/unity/unity_internals.h"
# include "ansi_colors.h"
# include "shell.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# ifdef TESTMODE

#  define MYTEST 1
# else
#  define MYTEST 0
# endif

# define SHELL_EXE "../shell_test"

typedef struct
{
	char	**items;
	size_t	size;
	size_t	capacity;
}			StringSet;

char		*run_bash(const char *cmd);
char		*run_my_shell(const char *cmd);
void		print_string_debug(const char *str);

void		stringset_init(StringSet *set);
void		stringset_free(StringSet *set);
void		stringset_add(StringSet *set, const char *item);
bool		stringset_equal(const StringSet *a, const StringSet *b);
void		stringset_from_string(StringSet *set, const char *str);

#endif
