/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_normalization.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 22:40:00 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:16 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	free_words(char **words)
{
	int	index;

	if (!words)
		return ;
	index = 0;
	while (words[index])
	{
		free(words[index]);
		index++;
	}
	free(words);
}

static int	count_arguments(int argc, char **argv)
{
	char	**words;
	int		count;
	int		total;

	total = 0;
	while (argc-- > 0)
	{
		if (!*argv || !**argv)
			return (-1);
		words = ft_split(*argv, ' ');
		if (!words)
			return (-1);
		count = 0;
		while (words[count])
			count++;
		free_words(words);
		if (count == 0)
			return (-1);
		total += count;
		argv++;
	}
	return (total);
}

static int	copy_arguments(t_input *input, int argc, char **argv)
{
	char	**words;
	int		word;
	int		index;

	index = 0;
	while (argc-- > 0)
	{
		words = ft_split(*argv++, ' ');
		if (!words)
			return (0);
		word = 0;
		while (words[word])
			input->values[index++] = words[word++];
		free(words);
	}
	input->values[index] = NULL;
	return (1);
}

int	normalize_args(int argc, char **argv, t_input *input)
{
	int	count;

	input->values = NULL;
	input->count = 0;
	count = count_arguments(argc, argv);
	if (count <= 0)
		return (0);
	input->values = ft_calloc(count + 1, sizeof(char *));
	if (!input->values)
		return (0);
	if (!copy_arguments(input, argc, argv))
	{
		free_input(input);
		return (0);
	}
	input->count = count;
	return (1);
}

void	free_input(t_input *input)
{
	int	index;

	if (!input || !input->values)
		return ;
	index = 0;
	while (input->values[index])
	{
		free(input->values[index]);
		index++;
	}
	free(input->values);
	input->values = NULL;
	input->count = 0;
}
