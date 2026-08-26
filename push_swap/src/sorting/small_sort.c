/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   small_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 20:45:00 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:10 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	find_min_position(t_stack *stack)
{
	int	min;
	int	position;
	int	min_position;

	min = stack->value;
	position = 0;
	min_position = 0;
	while (stack)
	{
		if (stack->value < min)
		{
			min = stack->value;
			min_position = position;
		}
		position++;
		stack = stack->next;
	}
	return (min_position);
}

static void	move_min_to_top(t_stack **a)
{
	int	position;
	int	size;

	position = find_min_position(*a);
	size = ft_stacksize(*a);
	while (position > 0 && position <= size / 2)
	{
		ra(a);
		position--;
	}
	while (position > size / 2)
	{
		rra(a);
		position++;
		if (position == size)
			position = 0;
	}
}

void	sort_3(t_stack **a)
{
	int	first;
	int	second;
	int	third;

	if (!a || !*a || !(*a)->next)
		return ;
	if (!(*a)->next->next)
	{
		if ((*a)->value > (*a)->next->value)
			sa(a);
		return ;
	}
	first = (*a)->value;
	second = (*a)->next->value;
	third = (*a)->next->next->value;
	if (first > second && first > third)
		ra(a);
	else if (second > first && second > third)
		rra(a);
	if ((*a)->value > (*a)->next->value)
		sa(a);
}

void	sort_5(t_stack **a, t_stack **b)
{
	if (!a || !b || !*a)
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
