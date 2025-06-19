/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:21:02 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 15:22:54 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include "ansi_colors.h"

# ifdef TESTMODE

#  define MYTEST 1
# else
#  define MYTEST 0
# endif

# define SHELL_EXE "./shell_test"

char	*run_bash(const char *cmd);
char	*run_my_shell(const char *cmd);
void	print_string_debug(const char *str);

#endif
