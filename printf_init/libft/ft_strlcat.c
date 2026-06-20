/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cguiama@student.42sp.org.br <cguiama@gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:09:22 by cguiama@stu       #+#    #+#             */
/*   Updated: 2026/05/26 21:16:21 by cguiama@stu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	s_len;
	size_t	d_len;

	s_len = ft_strlen(src);
	d_len = 0;
	while (dst[d_len] && d_len < size)
	{
		d_len++;
	}
	if (d_len >= size)
		return (size + s_len);
	ft_strlcpy(dst + d_len, src, size - d_len);
	return (d_len + s_len);
}
