/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strategy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 17:00:00 by guilamar          #+#    #+#             */
/*   Updated: 2026/08/18 17:00:00 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	adaptive_strategy(double disorder)
{
	if (disorder < 0.2)
		return (STRATEGY_SIMPLE);
	if (disorder < 0.5)
		return (STRATEGY_MEDIUM);
	return (STRATEGY_COMPLEX);
}

void	adaptive_sort(t_stack **a, t_stack **b, double disorder)
{
	int	strategy;

	strategy = adaptive_strategy(disorder);
	if (strategy == STRATEGY_SIMPLE)
		simple_sort(a, b);
	else if (strategy == STRATEGY_MEDIUM)
		medium_sort(a, b);
	else
		complex_sort(a, b);
}

int	execute_strategy(t_stack **a, t_stack **b, t_metrics *metrics)
{
	if (!a || !*a || !b || !metrics)
		return (0);
	if (metrics->strategy == STRATEGY_ADAPTIVE && ft_stackis_sorted(*a))
	{
		metrics->strategy = adaptive_strategy(metrics->disorder);
		return (1);
	}
	if (ft_stackis_sorted(*a))
		return (1);
	assign_index(a);
	if (metrics->strategy == STRATEGY_ADAPTIVE)
	{
		metrics->strategy = adaptive_strategy(metrics->disorder);
		adaptive_sort(a, b, metrics->disorder);
	}
	else if (metrics->strategy == STRATEGY_SIMPLE)
		simple_sort(a, b);
	else if (metrics->strategy == STRATEGY_MEDIUM)
		medium_sort(a, b);
	else if (metrics->strategy == STRATEGY_COMPLEX)
		complex_sort(a, b);
	else
		return (0);
	return (1);
}
