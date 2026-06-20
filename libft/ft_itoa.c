/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 19:46:13 by guilherme         #+#    #+#             */
/*   Updated: 2026/06/06 02:16:00 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_get_len(long n)
{
	long	nn;
	size_t	len;

	nn = n;
	len = 0;
	if (n < 0)
	{
		len = 1;
		nn *= -1;
	}
	while (nn > 0)
	{
		nn /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*ptr;
	long	nn;
	size_t	len;

	if (n == 0)
		return (ft_strdup("0"));
	len = ft_get_len(n);
	ptr = malloc((len + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	ptr[len] = '\0';
	len--;
	nn = n;
	if (n < 0)
		nn *= -1;
	while (nn > 0)
	{
		ptr[len] = (nn % 10) + '0';
		nn /= 10;
		len--;
	}
	if (n < 0)
		ptr[0] = '-';
	return (ptr);
}
