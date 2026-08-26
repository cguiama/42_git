/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_creation_tools.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 01:01:10 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:56:08 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_stack	*ft_new_node(int value, t_metrics *metrics)
{
	t_stack	*new_node;

	new_node = malloc(sizeof(t_stack));
	if (!new_node)
		return (NULL);
	new_node->value = value;
	new_node->index = -1;
	new_node->metrics = metrics;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

t_stack	*init_stack(int count, char **values, t_metrics *metrics)
{
	t_stack	*stack_a;
	t_stack	*new_node;
	int		value;
	int		i;

	stack_a = NULL;
	i = 0;
	while (i < count)
	{
		if (!parse_int(values[i], &value))
		{
			ft_freestack(&stack_a);
			return (NULL);
		}
		new_node = ft_new_node(value, metrics);
		if (!new_node)
		{
			ft_freestack(&stack_a);
			return (NULL);
		}
		ft_stackadd_back(&stack_a, new_node);
		i++;
	}
	return (stack_a);
}

t_stack	*init_stack_from_args(int argc, char **argv, int first,
		t_metrics *metrics)
{
	t_stack	*stack;
	t_input	input;

	if (!normalize_args(argc - first, argv + first, &input))
		return (NULL);
	if (!validate_numbers(input.count, input.values, 0))
	{
		free_input(&input);
		return (NULL);
	}
	stack = init_stack(input.count, input.values, metrics);
	free_input(&input);
	if (!stack || !ft_check_duplicates(stack))
	{
		ft_freestack(&stack);
		return (NULL);
	}
	return (stack);
}

void	ft_freestack(t_stack **stack)
{
	t_stack	*temp;

	if (!*stack)
		return ;
	while (*stack)
	{
		temp = (*stack)->next;
		free(*stack);
		*stack = temp;
	}
}

int	ft_stacksize(t_stack *stack)
{
	int	count;

	count = 0;
	while (stack)
	{
		count++;
		stack = stack->next;
	}
	return (count);
}
