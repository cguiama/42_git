#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    check_norme.sh                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gemini-cli <gemini@tester.unit.com>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/23 18:26:01 by jtoty             #+#    #+#              #
#    Updated: 2026/06/03 12:00:00 by gemini-cli       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

check_norme()
{
	local file_to_check=$1
	local target=""

	if [[ "$file_to_check" == bonus* ]] || [ "$file_to_check" == "doom_multi_fd_stress" ]; then
		target="${PATH_LIBFT}/get_next_line_bonus.c ${PATH_LIBFT}/get_next_line_utils_bonus.c ${PATH_LIBFT}/get_next_line_bonus.h"
	else
		target="${PATH_LIBFT}/get_next_line.c ${PATH_LIBFT}/get_next_line_utils.c ${PATH_LIBFT}/get_next_line.h"
	fi

	printf "\033[${NORME_COL}G"
	
	local has_error=0
	for f in $target; do
		if [ -e "$f" ]; then
			NORME_VAR=$(norminette "$f" 2>&1)
			if echo "$NORME_VAR" | grep -qE '(Error|Warning)'; then
				has_error=1
				break
			fi
		fi
	done

	if [ $has_error -eq 1 ]; then
		printf "${COLOR_FAIL}❌ ERR${DEFAULT}"
		return 0
	else
		printf "${COLOR_OK}✅ OK ${DEFAULT}"
		return 1
	fi
}
