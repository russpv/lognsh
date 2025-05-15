/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tok_types_arg.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:34:12 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/12 20:06:54 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

/* Added builtins so that they can be considered
 * args. Ex: exec echo haha, or cat echo
 * Note: TOK_EMPTY is valid arg.
 */
bool	is_arg_token(t_tok *tok)
{
	const enum e_tok_type	type = tok_get_type(tok);

	return (type == TOK_WORD || type == TOK_NAME || type == TOK_ENV_VAR
		|| type == TOK_EXIT_STATUS || type == TOK_BI || type == TOK_GROUP_WORD
		|| type == TOK_EMPTY);
}

/* Returns true if tok's raw string is an OP_OPTION (-) */
bool	is_option(t_tok *tok)
{
	const char	*s = tok_get_raw(tok);

	if (s && *s)
		return ((unsigned char)OP_OPTION == s[0]);
	return (false);
}
