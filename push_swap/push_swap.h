/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 21:58:46 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/13 00:58:11 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft.h"
# include <stdlib.h>
# include <unistd.h>

typedef struct s_stack
{
	int				value;
	int				index;
	struct s_stack	*prev;
	struct s_stack	*next;
}					t_stack;
t_stack				*ft_new_node(int value);
t_stack				*ft_stacklast(t_stack *stack);
t_stack				*init_stack(int argc, char **argv);
void				ft_stackadd_back(t_stack **stack, t_stack *new_node);
void				ft_stackadd_front(t_stack **stack, t_stack *new_node);
int					ft_stacksize(t_stack *stack);
void				ft_freestack(t_stack **stack);

#endif
