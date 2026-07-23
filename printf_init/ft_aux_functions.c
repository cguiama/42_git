/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 22:55:44 by guilherme         #+#    #+#             */
/*   Updated: 2026/07/23 16:26:41 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_nlemb(unsigned long nbr, char *base)
{
	int	len;

	len = 0;
	if (nbr == 0)
		return (1);
	while (nbr > 0)
	{
		nbr /= ft_strlen(base);
		len++;
	}
	return (len);
}

void	ft_putnbr_base_fd(unsigned long n, char *base, int fd)
{
	size_t	base_len;

	base_len = ft_strlen(base);
	if (n >= base_len)
		ft_putnbr_base_fd((n / base_len), base, fd);
	ft_putchar_fd(base[n % base_len], fd);
}

static int	ft_handle_base(char c, va_list args)
{
	unsigned int	unb;

	unb = va_arg(args, unsigned int);
	if (c == 'u')
	{
		ft_putnbr_base_fd(unb, "0123456789", 1);
		return (ft_nlemb(unb, "0123456789"));
	}
	else if (c == 'x')
	{
		ft_putnbr_base_fd(unb, "0123456789abcdef", 1);
		return (ft_nlemb(unb, "0123456789abcdef"));
	}
	else if (c == 'X')
	{
		ft_putnbr_base_fd(unb, "0123456789ABCDEF", 1);
		return (ft_nlemb(unb, "0123456789ABCDEF"));
	}
	return (0);
}

static int	ft_handle_int(char c, va_list args)
{
	int		nbr;
	char	*str;

	if (c == 'c')
	{
		ft_putchar_fd(va_arg(args, int), 1);
		return (1);
	}
	else if (c == 'd' || c == 'i')
	{
		nbr = va_arg(args, int);
		ft_putnbr_fd(nbr, 1);
		if (nbr < 0)
			return (ft_nlemb((unsigned long)(-(long)nbr), "0123456789") + 1);
		return (ft_nlemb((unsigned long)nbr, "0123456789"));
	}
	else if (c == 's')
	{
		str = va_arg(args, char *);
		if (str == NULL)
			str = "(null)";
		ft_putstr_fd(str, 1);
		return (ft_strlen(str));
	}
	return (0);
}

int	get_type(char c, va_list args)
{
	void	*vd;

	if (c == 'c' || c == 'd' || c == 'i' || c == 's')
		return (ft_handle_int(c, args));
	else if (c == 'u' || c == 'x' || c == 'X')
		return (ft_handle_base(c, args));
	else if (c == 'p')
	{
		vd = va_arg(args, void *);
		if (vd == NULL)
		{
			ft_putstr_fd("(nil)", 1);
			return (5);
		}
		ft_putstr_fd("0x", 1);
		ft_putnbr_base_fd((unsigned long)vd, "0123456789abcdef", 1);
		return (2 + ft_nlemb((unsigned long)vd, "0123456789abcdef"));
	}
	else if (c == '%')
	{
		ft_putchar_fd('%', 1);
		return (1);
	}
	return (0);
}
