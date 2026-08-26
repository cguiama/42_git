/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 21:20:00 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:11 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	find_min_pos(t_stack *stack)
{
	int		min;
	int		pos;
	int		index;
	t_stack	*current;

	min = stack->value;
	pos = 0;
	index = 0;
	current = stack;
	while (current)
	{
		if (current->value < min)
		{
			min = current->value;
			pos = index;
		}
		current = current->next;
		index++;
	}
	return (pos);
}

static void	move_min_to_top(t_stack **a)
{
	int	pos;
	int	size;

	pos = find_min_pos(*a);
	size = ft_stacksize(*a);
	if (pos <= size / 2)
		while (pos-- > 0)
			ra(a);
	else
		while (pos++ < size)
			rra(a);
}

void	simple_sort(t_stack **a, t_stack **b)
{
	if (!a || !*a || !b)
		return ;
	while (ft_stacksize(*a) > 3)
	{
		move_min_to_top(a);
		pb(a, b);
	}
	sort_3(a);
	while (*b)
		pa(a, b);
}
