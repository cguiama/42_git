/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 18:28:01 by guilamar          #+#    #+#             */
/*   Updated: 2026/07/23 16:26:10 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>

void	ft_putnbr_base_fd(unsigned long n, char *base, int fd);
int		ft_nlemb(unsigned long nbr, char *base);
int		get_type(char c, va_list args);
int		ft_printf(const char *format, ...);

#endif
