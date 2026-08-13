/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:10:21 by guilamar          #+#    #+#             */
/*   Updated: 2026/07/28 16:51:46 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*fs;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	fs = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!fs)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
	{
		fs[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		fs[i++] = s2[j++];
	}
	fs[i] = '\0';
	return (fs);
}
