/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 18:23:49 by guilamar          #+#    #+#             */
/*   Updated: 2026/07/23 16:08:08 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *format, ...)
{
	size_t	i;
	int		printed;
	va_list	args;

	if (!format)
		return (-1);
	i = 0;
	printed = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			printed += get_type(format[i + 1], args);
			i++;
		}
		else if (format[i] != '%')
		{
			ft_putchar_fd(format[i], 1);
			printed++;
		}
		i++;
	}
	va_end(args);
	return (printed);
}
