/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 20:32:20 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:18 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_rotate(t_stack **stack)
{
	t_stack	*first;
	t_stack	*last;

	if (!stack || !*stack || !(*stack)->next)
		return (0);
	first = *stack;
	last = ft_stacklast(*stack);
	*stack = first->next;
	(*stack)->prev = NULL;
	last->next = first;
	first->prev = last;
	first->next = NULL;
	return (1);
}

void	ra(t_stack **stack_a)
{
	if (ft_rotate(stack_a))
		emit_operation(*stack_a, OP_RA);
}

void	rb(t_stack **stack_b)
{
	if (ft_rotate(stack_b))
		emit_operation(*stack_b, OP_RB);
}

static t_stack	*operation_stack(t_stack **first, t_stack **second)
{
	if (first && *first)
		return (*first);
	if (second && *second)
		return (*second);
	return (NULL);
}

void	rr(t_stack **stack_a, t_stack **stack_b)
{
	if (ft_rotate(stack_a) | ft_rotate(stack_b))
		emit_operation(operation_stack(stack_a, stack_b), OP_RR);
}
