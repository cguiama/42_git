/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_directory.cpp                                 :+:      :+:    :+:   */
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
#include "get_next_line.h"
#undef new
}

#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "sigsegv.hpp"
#include "check.hpp"
#include "gnl.hpp"

int iTest = 1;
int main(void)
{
	signal(SIGSEGV, sigsegv);
	title("Directory Read Stress Test: " << ENDL)
	
	int fd = open(".", O_RDONLY);
	TEST(/* 1 */ gnl(fd, NULL);)
	close(fd);
	
	showLeaks();
	write(1, "\n", 1);
	return (0);
}
