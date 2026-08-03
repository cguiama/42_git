/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 18:28:01 by guilamar          #+#    #+#             */
/*   Updated: 2026/07/30 18:50:56 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>

void	ft_putnbr_base_fd(unsigned long n, char *base, int fd);
int		ft_nlemb(unsigned long nbr, char *base);
int		get_type(char c, va_list args);
int		ft_printf(const char *fmt, ...);

#endif
