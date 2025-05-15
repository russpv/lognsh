/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tok_types_proc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpeavey <rpeavey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:34:13 by rpeavey           #+#    #+#             */
/*   Updated: 2025/04/10 16:34:14 by rpeavey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

bool	is_open_paren(t_tok *tok)
{
	return (tok_get_type(tok) == TOK_OPEN_PAREN);
}

bool	is_close_paren(t_tok *tok)
{
	return (tok_get_type(tok) == TOK_CLOSE_PAREN);
}
