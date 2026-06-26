/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 18:23:49 by guilamar          #+#    #+#             */
/*   Updated: 2026/06/25 21:47:43 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	get_type(char c, va_list args)
{
	switch (c)
	{
	case 'c':
		ft_putchar_fd(va_arg(args, int), 1);
		break ;
	case 's':
		va_arg(args, char *);
		break ;
	case 'p':
		va_arg(args, void *);
		break ;
	case 'd':
	case 'i':
		va_arg(args, int);
		break ;
	case 'u':
	case 'x':
	case 'X':
		va_arg(args, unsigned int);
		break ;
	default:
		break ;
	}
}

int	ft_printf(const char *format, ...)
{
	size_t	i;
	size_t	printed;
	va_list	args;

	i = 0;
	printed = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] != '%')
		{
			ft_putchar_fd(format[i], 1);
			printed++;
		}
		else
		{
			get_type(format[i + 1], args);
			i += 1;
		}
		i++;
	}
	va_end(args);
	return (0);
}

/*

cspdiuxX%

%d, %i → va_arg(args, int)
%u, %x, %X → va_arg(args, unsigned int)
%s → va_arg(args, char *)
%c → va_arg(args,int) (char é promovido a int na lista variádica
 — nunca use char aqui)
%p → va_arg(args, void *)

*/
