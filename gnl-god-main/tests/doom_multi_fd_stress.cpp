/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_multi_fd_stress.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 12:00:00 by student           #+#    #+#             */
/*   Updated: 2026/06/12 12:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

extern "C"
{
#define new tripouille
#include "get_next_line_bonus.h"
#undef new
}

#include <fcntl.h>
#include <unistd.h>
#include "sigsegv.hpp"
#include "check.hpp"
#include "gnl.hpp"

int iTest = 1;
int main(void)
{
	signal(SIGSEGV, sigsegv);
	title("Multi-FD Stress Test: " << ENDL)
	
	int fds[50];
	for (int i = 0; i < 50; i++)
	{
		fds[i] = open("files/41_with_nl", O_RDONLY);
	}
	
	// Ler a primeira linha de todos os 50 FDs concorrentemente
	for (int i = 0; i < 50; i++)
	{
		/* 1..50 */ gnl(fds[i], "0123456789012345678901234567890123456789\n");
	}
	
	// Ler a segunda linha de todos os 50 FDs
	for (int i = 0; i < 50; i++)
	{
		/* 51..100 */ gnl(fds[i], "0");
	}
	
	// Ler EOF de todos os 50 FDs
	for (int i = 0; i < 50; i++)
	{
		/* 101..150 */ gnl(fds[i], NULL);
	}
	
	for (int i = 0; i < 50; i++)
	{
		close(fds[i]);
	}
	
	showLeaks();
	write(1, "\n", 1);
	return (0);
}
