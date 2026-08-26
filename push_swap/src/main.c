/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 00:18:03 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:09 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	print_error(void)
{
	ft_putstr_fd("Error\n", 2);
	return (1);
}

static int	finish_program(t_stack **a, t_stack **b, t_metrics *metrics,
		int success)
{
	if (success && metrics->bench)
		print_benchmark(metrics);
	ft_freestack(a);
	ft_freestack(b);
	if (!success)
		return (print_error());
	return (0);
}

int	main(int argc, char **argv)
{
	t_stack		*stack_a;
	t_stack		*stack_b;
	t_metrics	metrics;
	int			first;

	if (argc < 2)
		return (0);
	ft_bzero(&metrics, sizeof(t_metrics));
	first = parse_flags(argc, argv, &metrics.strategy, &metrics.bench);
	if (first < 0 || first == argc)
		return (print_error());
	stack_a = init_stack_from_args(argc, argv, first, &metrics);
	if (!stack_a)
		return (print_error());
	stack_b = NULL;
	metrics.disorder = compute_disorder(stack_a);
	return (finish_program(&stack_a, &stack_b, &metrics,
			execute_strategy(&stack_a, &stack_b, &metrics)));
}
