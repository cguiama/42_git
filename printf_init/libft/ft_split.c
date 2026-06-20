/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 17:13:08 by guilamar          #+#    #+#             */
/*   Updated: 2026/06/19 15:47:21 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *s, char c)
{
	int	y;

	y = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			y++;
		while (*s && *s != c)
			s++;
	}
	return (y);
}

static void	*ft_malloc_destroy(char **s, size_t qnt)
{
	while (qnt > 0)
	{
		qnt--;
		free(s[qnt]);
	}
	free(s);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**ptr;
	char	*start;
	size_t	i;

	ptr = malloc((ft_count_words(s, c) + 1) * sizeof(char *));
	if (!ptr)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			start = (char *)s;
			while (*s && *s != c)
				s++;
			ptr[i] = ft_substr(start, 0, s - start);
			if (!ptr[i])
				return (ft_malloc_destroy(ptr, i));
			i++;
		}
	}
	ptr[i] = NULL;
	return (ptr);
}
