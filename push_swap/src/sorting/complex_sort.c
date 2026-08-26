/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 21:35:00 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:14 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	get_max_bits(t_stack *stack)
{
	int	max;
	int	bits;

	max = 0;
	bits = 0;
	while (stack)
	{
		if (stack->index > max)
			max = stack->index;
		stack = stack->next;
	}
	while ((max >> bits) != 0)
		bits++;
	return (bits);
}

static void	radix_pass(t_stack **a, t_stack **b, int bit)
{
	int	count;
	int	size;

	count = 0;
	size = ft_stacksize(*a);
	while (count++ < size)
	{
		if (((*a)->index >> bit) & 1)
			ra(a);
		else
			pb(a, b);
	}
	while (*b)
		pa(a, b);
}

void	complex_sort(t_stack **a, t_stack **b)
{
	int	bit;
	int	max_bits;

	if (!a || !*a || !b)
		return ;
	if (ft_stacksize(*a) <= 5)
	{
		sort_5(a, b);
		return ;
	}
	bit = 0;
	max_bits = get_max_bits(*a);
	while (bit < max_bits)
	{
		radix_pass(a, b, bit);
		bit++;
	}
}
