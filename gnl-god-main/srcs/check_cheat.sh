#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    check_cheat.sh                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gemini-cli <gemini@tester.unit.com>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/23 18:26:13 by jtoty             #+#    #+#              #
#    Updated: 2026/06/05 08:00:00 by gemini-cli       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

check_cheating()
{
	local file_to_check=$1
	printf "\033[${CHEAT_COL}G"
	
	local target=""
	if [[ "$file_to_check" == bonus* ]] || [ "$file_to_check" == "doom_multi_fd_stress" ]; then
		target="${PATH_LIBFT}/get_next_line_bonus.c ${PATH_LIBFT}/get_next_line_utils_bonus.c"
	else
		target="${PATH_LIBFT}/get_next_line.c ${PATH_LIBFT}/get_next_line_utils.c"
	fi

	local allowed="read|malloc|free|__stack_chk_fail|dyld_stub_binder|exit"
	
	# Compile to object file first
	cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 -c $target > /dev/null 2>&1
	
	local forbidden=""
	for obj in *.o; do
		if [ -e "$obj" ]; then
			forbidden+="$(nm -u "$obj" 2>/dev/null | sed 's/^[[:space:]]*U[[:space:]]*//' | sed 's/^_//' | grep -vE "^($allowed)$" | grep -v "^ft_")"
		fi
	done
	rm -f *.o

	if [ "${forbidden}" != "" ]
	then
		printf "  ${COLOR_FAIL}❌ FORBIDDEN${DEFAULT}"
		printf "External forbidden functions detected in %s:\n" "$file_to_check" >> "${PATH_DEEPTHOUGHT}"/deepthought
		printf "${forbidden}\n\n" >> "${PATH_DEEPTHOUGHT}"/deepthought
		return 1
	else
		printf "  ${COLOR_OK}✅ CLEAN    ${DEFAULT}"
		return 0
	fi
}
