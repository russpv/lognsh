/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:56:22 by dayeo             #+#    #+#             */
/*   Updated: 2025/02/07 17:01:19 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bi_int.h"

/* ECHO
 * derived from code echo.c in Bash.
 * Option -n.* defeats newline print
 */
static char	*_expand_variable(const char *arg)
{
	char	*env_value;
	
	if (arg[0] == '$')
	{
		env_value = getenv(arg + 1); // remove '$' and get value //RKP: this needs to get shell env?
		if (env_value)
			return (ft_strdup(env_value));
		return (ft_strdup("")); // return empty string if variable is not set
	}
	return (ft_strdup(arg));
}


int	bi_echo(t_state *s, char **argv, int argc)
{
	char	*expanded;
	int	i;


	(void)s;
	i = 1;
	while (i <argc)
	{
		expanded = _expand_variable(argv[i]);
		printf("%s", expanded);
		free(expanded);
		
		if (i < argc - 1)
			printf(" ");
		i++;
	}
	printf("\n");
	return (0);
}

//old echo
/*int	bi_echo(t_state *s, char **argv, int argc)
{
	int	i;
	int	no_newline;

	(void)s;
	i = 1;
	no_newline = 0;
	if (!argv)
		return (0);
	while (argv[i] && ft_strncmp(argv[i], "-n", -1) == 0)
	{
		no_newline = 1;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
	flushbuf();
	return (0);
}*/
