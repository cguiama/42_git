/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 16:57:57 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/10 15:48:49 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_and_accumulate(int fd, char *over)
{
	char	*buffer;
	char	*tmp;
	int		bytes_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(over, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buffer);
			free(over);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		tmp = ft_strjoin(over, buffer);
		free(over);
		over = tmp;
	}
	free(buffer);
	return (over);
}

static char	*extract_line(char *over)
{
	char	*line;
	size_t	len;

	len = 0;
	if (!over || !over[0])
		return (NULL);
	while (over[len] && over[len] != '\n')
		len++;
	if (over[len] == '\n')
		len++;
	line = ft_substr(over, 0, len);
	return (line);
}

static char	*update_over(char *over)
{
	char	*new_over;
	size_t	i;

	i = 0;
	while (over[i] && over[i] != '\n')
		i++;
	if (!over[i])
	{
		free(over);
		return (NULL);
	}
	new_over = ft_substr(over, i + 1, ft_strlen(over) - i - 1);
	free(over);
	if (new_over && !new_over[0])
	{
		free(new_over);
		return (NULL);
	}
	return (new_over);
}

char	*get_next_line(int fd)
{
	static char	*over;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!over)
	{
		over = malloc(1);
		if (!over)
			return (NULL);
		over[0] = '\0';
	}
	over = read_and_accumulate(fd, over);
	if (!over)
		return (NULL);
	line = extract_line(over);
	over = update_over(over);
	return (line);
}
