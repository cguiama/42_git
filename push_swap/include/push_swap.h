/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilamar <guilamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 21:58:46 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/18 15:55:28 by guilamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft.h"
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>

# define STRATEGY_INVALID -1
# define STRATEGY_SIMPLE 0
# define STRATEGY_MEDIUM 1
# define STRATEGY_COMPLEX 2
# define STRATEGY_ADAPTIVE 3

# define OP_SA 0
# define OP_SB 1
# define OP_SS 2
# define OP_PA 3
# define OP_PB 4
# define OP_RA 5
# define OP_RB 6
# define OP_RR 7
# define OP_RRA 8
# define OP_RRB 9
# define OP_RRR 10
# define OP_COUNT 11

typedef struct s_metrics
{
	double		disorder;
	int			strategy;
	int			bench;
	long		total;
	long		operations[OP_COUNT];
}				t_metrics;

typedef struct s_stack
{
	int				value;
	int				index;
	t_metrics		*metrics;
	struct s_stack	*prev;
	struct s_stack	*next;
}					t_stack;

typedef struct s_input
{
	char			**values;
	int				count;
}					t_input;

/* ================= FUNÇÕES BASE ================= */

t_stack				*ft_new_node(int value, t_metrics *metrics);
t_stack				*ft_stacklast(t_stack *stack);
void				ft_stackadd_back(t_stack **stack, t_stack *new_node);
void				ft_stackadd_front(t_stack **stack, t_stack *new_node);
int					ft_stacksize(t_stack *stack);
int					ft_stackis_sorted(t_stack *stack);
void				ft_freestack(t_stack **stack);

/* ================ PARSING & ERROS ================ */
int					ft_check_args(int argc, char **argv, int first);
int					parse_int(const char *str, int *value);
int					validate_numbers(int argc, char **argv, int first);
int					ft_check_duplicates(t_stack *stack);
int					normalize_args(int argc, char **argv, t_input *input);
void				free_input(t_input *input);
t_stack				*init_stack(int count, char **values, t_metrics *metrics);
t_stack				*init_stack_from_args(int argc, char **argv, int first,
						t_metrics *metrics);

/* ================ MOVIMENTOS (OPERATIONS) ================ */
/* Swap */
int					ft_swap(t_stack **stack);
void				sa(t_stack **stack_a);
void				sb(t_stack **stack_b);
void				ss(t_stack **stack_a, t_stack **stack_b);

/* Push */
int					ft_push(t_stack **src, t_stack **dest);
void				pa(t_stack **stack_a, t_stack **stack_b);
void				pb(t_stack **stack_a, t_stack **stack_b);

/* Rotate */
int					ft_rotate(t_stack **stack);
void				ra(t_stack **stack_a);
void				rb(t_stack **stack_b);
void				rr(t_stack **stack_a, t_stack **stack_b);

/* Reverse Rotate */
int					ft_rev_rotate(t_stack **stack);
void				rra(t_stack **stack_a);
void				rrb(t_stack **stack_b);
void				rrr(t_stack **stack_a, t_stack **stack_b);

/* ================ MÉTRICAS E ÍNDICES ================ */
double				compute_disorder(t_stack *stack_a);
void				assign_index(t_stack **stack_a);
void				print_benchmark(t_metrics *metrics);

/* ============ SELEÇÃO DE ESTRATÉGIAS (FLAGS) ============ */
int					parse_flags(int argc, char **argv, int *strategy,
						int *bench);

/* ================ ALGORITMOS DE ORDENAÇÃO ================ */
/* 1. Simples O(n^2) */
void				simple_sort(t_stack **a, t_stack **b);
int					execute_strategy(t_stack **a, t_stack **b,
						t_metrics *metrics);

/* 2. Médio O(n*sqrt(n)) */
void				medium_sort(t_stack **a, t_stack **b);
int					medium_block_size(int size);
void				medium_push_blocks(t_stack **a, t_stack **b,
						int block_size);
void				medium_restore(t_stack **a, t_stack **b);

/* 3. Complexo O(n log n) - Ex: Radix */
void				complex_sort(t_stack **a, t_stack **b);

/* 4. Adaptativo */
void				adaptive_sort(t_stack **a, t_stack **b, double disorder);
int					adaptive_strategy(double disorder);

/* 5. Ordenação Pequena (Auxiliares) */
void				sort_3(t_stack **a);
void				sort_5(t_stack **a, t_stack **b);

void				emit_operation(t_stack *stack, int operation);

#endif
