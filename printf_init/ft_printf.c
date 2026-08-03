/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 18:23:49 by guilamar          #+#    #+#             */
/*   Updated: 2026/07/30 18:55:53 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	process_char(const char *fmt, int *i, va_list args)
{
	if (fmt[*i] == '%' && !fmt[*i + 1])
		return (-1);
	if (fmt[*i] == '%')
		return (get_type(fmt[++(*i)], args));
	ft_putchar_fd(fmt[*i], 1);
	return (1);
}

int	ft_printf(const char *fmt, ...)
{
	int		i;
	int		printed;
	int		ret;
	va_list	args;

	if (!fmt)
		return (-1);
	i = -1;
	printed = 0;
	va_start(args, fmt);
	while (fmt[++i])
	{
		ret = process_char(fmt, &i, args);
		if (ret == -1)
		{
			printed = -1;
			break ;
		}
		printed += ret;
	}
	va_end(args);
	return (printed);
}
