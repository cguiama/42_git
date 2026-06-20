/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:10:21 by guilamar          #+#    #+#             */
/*   Updated: 2026/05/28 18:11:37 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*fs;
	size_t	ts;
	size_t	ss1;
	size_t	ss2;

	if (!s1 || !s2)
		return (NULL);
	ss1 = ft_strlen(s1);
	ss2 = ft_strlen(s2);
	ts = ss1 + ss2 + 1;
	fs = malloc(ts * sizeof(char));
	if (!fs)
		return (NULL);
	ft_strlcpy(fs, s1, ss1 + 1);
	ft_strlcat(fs, s2, ts);
	return (fs);
}
