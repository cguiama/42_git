/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 13:39:09 by guilamar          #+#    #+#             */
/*   Updated: 2026/05/28 14:57:22 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	sbs_len;
	size_t	str_len;

	if (!s)
		return (NULL);
	str_len = ft_strlen(s);
	if (start >= str_len)
	{
		ptr = malloc(1);
		if (!ptr)
			return (NULL);
		ft_bzero(ptr, 1);
		return (ptr);
	}
	sbs_len = (str_len - start);
	if (len > sbs_len)
		len = sbs_len;
	ptr = malloc(len + 1 * sizeof(char));
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, &s[start], len + 1);
	return (ptr);
}
