/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 20:50:00 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:17 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_swap(t_stack **stack)
{
	int	value;

	if (!stack || !*stack || !(*stack)->next)
		return (0);
	value = (*stack)->value;
	(*stack)->value = (*stack)->next->value;
	(*stack)->next->value = value;
	return (1);
}

void	sa(t_stack **stack_a)
{
	if (ft_swap(stack_a))
		emit_operation(*stack_a, OP_SA);
}

void	sb(t_stack **stack_b)
{
	if (ft_swap(stack_b))
		emit_operation(*stack_b, OP_SB);
}

static t_stack	*operation_stack(t_stack **first, t_stack **second)
{
	if (first && *first)
		return (*first);
	if (second && *second)
		return (*second);
	return (NULL);
}

void	ss(t_stack **stack_a, t_stack **stack_b)
{
	if (ft_swap(stack_a) | ft_swap(stack_b))
		emit_operation(operation_stack(stack_a, stack_b), OP_SS);
}
