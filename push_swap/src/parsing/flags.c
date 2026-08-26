/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 16:30:00 by guilamar          #+#    #+#             */
/*   Updated: 2026/08/18 16:30:00 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	get_strategy(char *flag)
{
	if (!ft_strncmp(flag, "--simple", 9))
		return (STRATEGY_SIMPLE);
	if (!ft_strncmp(flag, "--medium", 9))
		return (STRATEGY_MEDIUM);
	if (!ft_strncmp(flag, "--complex", 10))
		return (STRATEGY_COMPLEX);
	if (!ft_strncmp(flag, "--adaptive", 11))
		return (STRATEGY_ADAPTIVE);
	return (STRATEGY_INVALID);
}

static int	set_strategy(char *flag, int *strategy, int *selected)
{
	int	current;

	if (*selected)
		return (0);
	current = get_strategy(flag);
	if (current == STRATEGY_INVALID)
		return (0);
	*strategy = current;
	*selected = 1;
	return (1);
}

int	parse_flags(int argc, char **argv, int *strategy, int *bench)
{
	int	index;
	int	selected;

	if (!strategy || !bench)
		return (-1);
	*strategy = STRATEGY_ADAPTIVE;
	*bench = 0;
	index = 1;
	selected = 0;
	while (argv && index < argc && !ft_strncmp(argv[index], "--", 2))
	{
		if (!ft_strncmp(argv[index], "--bench", 8))
			*bench = 1;
		else if (!set_strategy(argv[index], strategy, &selected))
		{
			*strategy = STRATEGY_INVALID;
			return (-1);
		}
		index++;
	}
	return (index);
}
