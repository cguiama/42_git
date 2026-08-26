/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 18:10:00 by guilamar          #+#    #+#             */
/*   Updated: 2026/08/18 18:10:00 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static char	*operation_name(int operation)
{
	char	*names[OP_COUNT];

	names[OP_SA] = "sa";
	names[OP_SB] = "sb";
	names[OP_SS] = "ss";
	names[OP_PA] = "pa";
	names[OP_PB] = "pb";
	names[OP_RA] = "ra";
	names[OP_RB] = "rb";
	names[OP_RR] = "rr";
	names[OP_RRA] = "rra";
	names[OP_RRB] = "rrb";
	names[OP_RRR] = "rrr";
	return (names[operation]);
}

static char	*strategy_name(int strategy)
{
	if (strategy == STRATEGY_SIMPLE)
		return ("simple (O(n^2))");
	if (strategy == STRATEGY_MEDIUM)
		return ("medium (O(n*sqrt(n)))");
	return ("complex (O(n log n))");
}

static void	print_disorder(double disorder)
{
	long	value;

	value = (long)(disorder * 10000 + 0.5);
	ft_putstr_fd("Disorder: ", 2);
	ft_putnbr_fd(value / 100, 2);
	ft_putchar_fd('.', 2);
	if (value % 100 < 10)
		ft_putchar_fd('0', 2);
	ft_putnbr_fd(value % 100, 2);
	ft_putstr_fd("%\n", 2);
}

static void	print_operation(t_metrics *metrics, int operation)
{
	ft_putstr_fd(operation_name(operation), 2);
	ft_putstr_fd(": ", 2);
	ft_putnbr_fd(metrics->operations[operation], 2);
	ft_putchar_fd('\n', 2);
}

void	print_benchmark(t_metrics *metrics)
{
	int	operation;

	if (!metrics)
		return ;
	print_disorder(metrics->disorder);
	ft_putstr_fd("Strategy: ", 2);
	ft_putendl_fd(strategy_name(metrics->strategy), 2);
	ft_putstr_fd("Operations: ", 2);
	ft_putnbr_fd(metrics->total, 2);
	ft_putchar_fd('\n', 2);
	operation = 0;
	while (operation < OP_COUNT)
		print_operation(metrics, operation++);
}
