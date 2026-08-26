/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 22:20:00 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:15 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	parse_digits(const char *str, long *result, long limit)
{
	int	digit;

	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		digit = *str - '0';
		if (*result > (limit - digit) / 10)
			return (0);
		*result = *result * 10 + digit;
		str++;
	}
	return (1);
}

int	parse_int(const char *str, int *value)
{
	long	result;
	long	limit;
	int		sign;

	if (!str || !*str || !value)
		return (0);
	sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str)
		return (0);
	limit = INT_MAX;
	if (sign < 0)
		limit++;
	result = 0;
	if (!parse_digits(str, &result, limit))
		return (0);
	*value = (int)(result * sign);
	return (1);
}

int	validate_numbers(int argc, char **argv, int first)
{
	int	value;

	if (!ft_check_args(argc, argv, first))
		return (0);
	while (first < argc)
	{
		if (!parse_int(argv[first], &value))
			return (0);
		first++;
	}
	return (1);
}

int	ft_check_duplicates(t_stack *stack)
{
	t_stack	*compare;

	while (stack)
	{
		compare = stack->next;
		while (compare)
		{
			if (stack->value == compare->value)
				return (0);
			compare = compare->next;
		}
		stack = stack->next;
	}
	return (1);
}
