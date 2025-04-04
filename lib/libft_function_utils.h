/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_function_utils.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dayeo <dayeo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 11:53:06 by dayeo             #+#    #+#             */
/*   Updated: 2025/04/04 11:58:40 by dayeo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_FUNCTION_UTILS_H
# define LIBFT_FUNCTION_UTILS_H

# include "../include/libft.h"

//ft_split_ifs_mem_utils
void	_arr_free_if(struct s_mem_utils *m, char **arr, unsigned int i);
t_bool	_is_print_if(char const *p);
int		_get_word_count_if(char const *p, char const *set);
void	*_copy_word_if(struct s_mem_utils *m, \
		char const *s, char **elem, size_t len);
char	*_get_temp_if(char const *str, char const *delims);

//ft_split_mem_utils
void	_arr_free(struct s_mem_utils *m, char **arr, unsigned int i);
t_bool	_is_print(char const *p);
int		_get_word_count(char const *p, char ch);
void	*_copy_word(struct s_mem_utils *m, \
		char const *s, char **elem, size_t len);
char	*_get_temp(char const *str, char delim);

//shared ft_split_ifs_mem and ft_split_mem functions

#endif
