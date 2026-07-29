/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilherme <guilherme@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 18:25:22 by guilherme         #+#    #+#             */
/*   Updated: 2026/07/28 18:25:23 by guilherme        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*  main_gnl_test.c — arquivo de teste (NÃO ENTREGAR, apenas para testar)    */
/*                                                                            */
/*  COMO COMPILAR:                                                            */
/*    cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \                           */
/*       get_next_line.c get_next_line_utils.c main_gnl_test.c -o gnl_test   */
/*                                                                            */
/*  COMO TESTAR COM BUFFER SIZE DIFERENTE:                                   */
/*    cc ... -D BUFFER_SIZE=1 ...    (extremo: 1 byte por vez)               */
/*    cc ... -D BUFFER_SIZE=9999 ... (extremo: buffer gigante)               */
/*                                                                            */
/*  COMO RODAR:                                                               */
/*    ./gnl_test arquivo.txt         (lê de um arquivo)                      */
/*    ./gnl_test                     (lê do stdin — digita e pressiona Enter) */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;
	int		i;

	i = 1;
	if (argc == 1)
	{
		fd = 0;
		printf("Lendo do stdin (Ctrl+D para encerrar):\n");
		while ((line = get_next_line(fd)) != NULL)
		{
			printf("[linha]: %s", line);
			free(line);
		}
		return (0);
	}
	while (argv[i])
	{
		fd = open(argv[i], O_RDONLY);
		if (fd < 0)
		{
			printf("Erro ao abrir: %s\n", argv[i]);
			i++;
			continue ;
		}
		printf("=== Arquivo: %s ===\n", argv[i]);
		while ((line = get_next_line(fd)) != NULL)
		{
			printf("[linha]: %s", line);
			free(line);
		}
		printf("=== EOF ===\n\n");
		close(fd);
		i++;
	}
	return (0);
}
