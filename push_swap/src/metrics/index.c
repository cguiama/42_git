/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 21:05:00 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:22 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	assign_index(t_stack **stack_a)
{
	t_stack	*current;
	t_stack	*compare;
	int		index;

	if (!stack_a || !*stack_a)
		return ;
	current = *stack_a;
	while (current)
	{
		index = 0;
		compare = *stack_a;
		while (compare)
		{
			if (compare->value < current->value)
				index++;
			compare = compare->next;
		}
		current->index = index;
		current = current->next;
	}
}
