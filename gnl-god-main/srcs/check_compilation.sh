#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    check_compilation.sh                               :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gemini-cli <gemini@tester.unit.com>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/23 18:26:33 by jtoty             #+#    #+#              #
#    Updated: 2026/06/03 12:00:00 by gemini-cli       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

compilation()
{
	local file_to_check=$1
	local bs=42
	[[ "$file_to_check" == *_bs_1 ]] && bs=1
	[[ "$file_to_check" == *_bs_42 ]] && bs=42
	[[ "$file_to_check" == *_bs_9999 ]] && bs=9999
	[[ "$file_to_check" == *_bs_10M ]] && bs=10000000

	local target=""
	if [[ "$file_to_check" == bonus* ]] || [ "$file_to_check" == "doom_multi_fd_stress" ]; then
		target="${PATH_LIBFT}/get_next_line_bonus.c ${PATH_LIBFT}/get_next_line_utils_bonus.c"
	else
		target="${PATH_LIBFT}/get_next_line.c ${PATH_LIBFT}/get_next_line_utils.c"
	fi

	printf "\033[${COMPIL_COL}G"
	
	cc -Wall -Wextra -Werror -D BUFFER_SIZE=$bs -c $target > /dev/null 2>&1
	COMPIL_RES=$?
	rm -f *.o
}

check_compilation()
{
	if [ $COMPIL_RES -eq 0 ]
	then
		printf "${COLOR_OK}✅ OK ${DEFAULT}"
		return 1
	else
		printf "${COLOR_FAIL}❌ ERR${DEFAULT}"
		return 0
	fi
}
