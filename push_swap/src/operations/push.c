/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 20:45:00 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:21 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_push(t_stack **src, t_stack **dest)
{
	t_stack	*node;

	if (!src || !dest || !*src)
		return (0);
	node = *src;
	*src = node->next;
	if (*src)
		(*src)->prev = NULL;
	node->next = NULL;
	node->prev = NULL;
	ft_stackadd_front(dest, node);
	return (1);
}

void	pa(t_stack **stack_a, t_stack **stack_b)
{
	if (ft_push(stack_b, stack_a))
		emit_operation(*stack_a, OP_PA);
}

void	pb(t_stack **stack_a, t_stack **stack_b)
{
	if (ft_push(stack_a, stack_b))
		emit_operation(*stack_b, OP_PB);
}
