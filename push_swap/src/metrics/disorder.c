/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disorder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhony <jhony@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 00:00:00 by jhony             #+#    #+#             */
/*   Updated: 2026/08/17 00:00:00 by jhony            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

double	compute_disorder(t_stack *stack_a)
{
	t_stack	*current;
	t_stack	*next;
	long	mistakes;
	long	total_pairs;

	mistakes = 0;
	total_pairs = 0;
	current = stack_a;
	while (current)
	{
		next = current->next;
		while (next)
		{
			if (current->value > next->value)
				mistakes++;
			total_pairs++;
			next = next->next;
		}
		current = current->next;
	}
	if (total_pairs == 0)
		return (0.0);
	return ((double)mistakes / (double)total_pairs);
}
