/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_callbacks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:55:27 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/05 11:55:28 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_int.h"

void	register_parser_destroy(t_state *s, t_destroy_fn fn)
{
	s->destroy_parser = fn;
}

void	register_lexer_destroy(t_state *s, t_destroy_fn fn)
{
	s->destroy_lexer = fn;
}

void	register_command_destroy(t_state *s, t_destroy_fn fn)
{
	s->destroy_command = fn;
}

void	register_token_destroy(t_state *s, t_destroy_fn fn)
{
	s->destroy_token = fn;
}
