/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_output.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 18:00:00 by guilamar          #+#    #+#             */
/*   Updated: 2026/08/18 18:00:00 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	emit_operation(t_stack *stack, int operation)
{
	char	*names[OP_COUNT];

	if (!stack || operation < 0 || operation >= OP_COUNT)
		return ;
	names[OP_SA] = "sa\n";
	names[OP_SB] = "sb\n";
	names[OP_SS] = "ss\n";
	names[OP_PA] = "pa\n";
	names[OP_PB] = "pb\n";
	names[OP_RA] = "ra\n";
	names[OP_RB] = "rb\n";
	names[OP_RR] = "rr\n";
	names[OP_RRA] = "rra\n";
	names[OP_RRB] = "rrb\n";
	names[OP_RRR] = "rrr\n";
	stack->metrics->operations[operation]++;
	stack->metrics->total++;
	write(1, names[operation], ft_strlen(names[operation]));
}
