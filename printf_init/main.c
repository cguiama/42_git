/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 16:34:41 by guilherme         #+#    #+#             */
/*   Updated: 2026/07/23 16:39:54 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <limits.h>
#include <stdio.h>

int	main(void)
{
	int	ret_ft;
	int	ret_std;

	printf("--- TESTE 1: Strings e Chars ---\n");
	ret_ft = ft_printf("FT : %s | %c\n", "Guilherme 42", 'Z');
	ret_std = printf("STD: %s | %c\n", "Guilherme 42", 'Z');
	printf("Retornos -> FT: %d | STD: %d\n\n", ret_ft, ret_std);
	printf("--- TESTE 2: Limites Matemáticos (INT_MIN e Hexadecimais) ---\n");
	ret_ft = ft_printf("FT : %d | %x | %X\n", INT_MIN, 255, 255);
	ret_std = printf("STD: %d | %x | %X\n", INT_MIN, 255, 255);
	printf("Retornos -> FT: %d | STD: %d\n\n", ret_ft, ret_std);
	printf("--- TESTE 3: Ponteiros e Casos Nulos ---\n");
	ret_ft = ft_printf("FT : %p | %s\n", NULL, NULL);
	ret_std = printf("STD: %p | %s\n", NULL, NULL);
	printf("Retornos -> FT: %d | STD: %d\n\n", ret_ft, ret_std);
	printf("--- TESTE 4: Porcentagem Isolada ---\n");
	ret_ft = ft_printf("FT : %%%% e uma falha no fim %");
	printf("\n");
	ret_std = printf("STD: %%%% e uma falha no fim %");
	printf("\nRetornos -> FT: %d | STD: %d\n", ret_ft, ret_std);
	return (0);
}
/*
	cc main.c libftprintf.a -I./libft -o teste_printf
*/
