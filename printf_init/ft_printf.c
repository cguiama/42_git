/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 18:23:49 by guilamar          #+#    #+#             */
/*   Updated: 2026/06/20 19:28:15 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h";

static void	get_type(char *c)
{
	switch (c)
	{
	case 'c':
		/* code */
		break ;
	case 's':
		/* code */
		break ;
	case 'p':
		/* code */
		break ;
	case 'd':
		/* code */
		break ;
	case 'i':
		/* code */
		break ;
	case 'u':
		/* code */
		break ;
	case 'x':
		/* code */
		break ;
	case 'X':
		/* code */
		break ;
	default:
		break ;
	}
}

int	ft_printf(const char *format, ...)
{
	size_t	i;
	size_t	printed;

	i = 0;
	while (format[i])
	{
		if (format[i] != '%')
		{
			ft_putchar_fd(format[i], 1);
			printed++;
		}
		else
		{
			get_type(format[i + 1]);
			i += 1;
		}
		i++;
	}
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
