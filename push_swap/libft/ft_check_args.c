/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 00:10:38 by guilherme         #+#    #+#             */
/*   Updated: 2026/08/13 00:14:58 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_check_args(int argc, char **argv, int first)
{
	if (first >= argc)
		return (0);
	while (first < argc)
	{
		if (!ft_str_is_digit(argv[first]))
			return (0);
		first++;
	}
	return (1);
}
