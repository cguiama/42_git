/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 17:38:10 by guilamar          #+#    #+#             */
/*   Updated: 2026/06/10 16:20:59 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ptr;
	int		i;
	int		j;
	size_t	ss1;

	i = 0;
	if (!s1 || !set)
		return (NULL);
	while (s1[i] && ft_strchr(set, s1[i]))
		i++;
	if (!s1[i])
	{
		ptr = ft_strdup("");
		return (ptr);
	}
	ss1 = ft_strlen(s1);
	j = ss1 - 1;
	while (ft_strchr(set, s1[j]))
	{
		j--;
	}
	ptr = ft_substr(s1, i, (j - i) + 1);
	return (ptr);
}
