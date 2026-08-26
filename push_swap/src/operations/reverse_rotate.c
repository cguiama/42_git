/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 20:32:14 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:19 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_rev_rotate(t_stack **stack)
{
	t_stack	*last;
	t_stack	*new_last;

	if (!stack || !*stack || !(*stack)->next)
		return (0);
	last = ft_stacklast(*stack);
	new_last = last->prev;
	new_last->next = NULL;
	last->prev = NULL;
	last->next = *stack;
	(*stack)->prev = last;
	*stack = last;
	return (1);
}

void	rra(t_stack **stack_a)
{
	if (ft_rev_rotate(stack_a))
		emit_operation(*stack_a, OP_RRA);
}

void	rrb(t_stack **stack_b)
{
	if (ft_rev_rotate(stack_b))
		emit_operation(*stack_b, OP_RRB);
}

static t_stack	*operation_stack(t_stack **first, t_stack **second)
{
	if (first && *first)
		return (*first);
	if (second && *second)
		return (*second);
	return (NULL);
}

void	rrr(t_stack **stack_a, t_stack **stack_b)
{
	if (ft_rev_rotate(stack_a) | ft_rev_rotate(stack_b))
		emit_operation(operation_stack(stack_a, stack_b), OP_RRR);
}
