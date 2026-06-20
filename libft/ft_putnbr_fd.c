/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 11:43:15 by guilherme         #+#    #+#             */
/*   Updated: 2026/06/06 12:08:55 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	nn;

	nn = n;
	if (n < 0)
	{
		nn *= -1;
		ft_putchar_fd('-', fd);
	}
	if (nn > 9)
		ft_putnbr_fd((nn / 10), fd);
	ft_putchar_fd(((nn % 10) + '0'), fd);
}
