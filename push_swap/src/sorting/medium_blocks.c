/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medium_blocks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 22:00:00 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:13 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	medium_block_size(int size)
{
	int	root;

	if (size <= 0)
		return (0);
	root = 1;
	while (root < size / root)
		root++;
	return (root);
}

static void	push_block_value(t_stack **a, t_stack **b, int lower, int size)
{
	pb(a, b);
	if ((*b)->index < lower + size / 2)
		rb(b);
}

static void	update_block(int *lower, int *limit, int *pushed, int size)
{
	(*pushed)++;
	if (*pushed == size)
	{
		*lower = *limit;
		*limit += size;
		*pushed = 0;
	}
}

void	medium_push_blocks(t_stack **a, t_stack **b, int block_size)
{
	int	limit;
	int	lower;
	int	pushed;

	if (!a || !*a || !b || block_size <= 0)
		return ;
	limit = block_size;
	lower = 0;
	pushed = 0;
	while (*a)
	{
		if ((*a)->index < limit)
		{
			push_block_value(a, b, lower, block_size);
			update_block(&lower, &limit, &pushed, block_size);
		}
		else
			ra(a);
	}
}

void	medium_sort(t_stack **a, t_stack **b)
{
	int	size;

	if (!a || !*a || !b)
		return ;
	size = ft_stacksize(*a);
	if (size <= 5)
	{
		sort_5(a, b);
		return ;
	}
	medium_push_blocks(a, b, medium_block_size(size));
	medium_restore(a, b);
}
