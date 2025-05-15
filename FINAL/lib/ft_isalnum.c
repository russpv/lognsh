/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:26:55 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/03 15:26:57 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

/* ISALNUM
** Tests if char is alphanumeric 
*/
t_bool	ft_isalnum(int c)
{
	return (ft_isdigit(c) || ft_isalpha(c));
}
