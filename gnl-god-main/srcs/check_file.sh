/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file.sh                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemini-cli <gemini@tester.unit.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 18:26:01 by jtoty             #+#    #+#              #
/*   Updated: 2026/06/12 12:00:00 by gemini-cli       ###   ########.fr        #
/*                                                                            */
/* ************************************************************************** */

#!/bin/bash



check_header_gnl()
{
	local header_name=$1
	local display_name=$2
	printf "${BOLD}│ %-17s │${DEFAULT}" "$display_name"
	if [ -e "${PATH_LIBFT}"/${HEADER_DIR}/"$header_name" ]
	then
		if [ ${OPT_NO_NORMINETTE} -eq 1 ]
		then
			printf " Norme: ${COLOR_WARNING}SKIP${DEFAULT}                   │\n"
		else
			NORME_VAR=$(norminette "${PATH_LIBFT}"/${HEADER_DIR}/"$header_name" 2>&1)
			if echo "$NORME_VAR" | grep -q command
			then
				printf " Norme: ${COLOR_WARNING}⚠️  NT${DEFAULT}                 │\n"
			elif echo "$NORME_VAR" | grep -qE '(Error|Warning)'
			then
				printf " Norme: ${COLOR_FAIL}❌ ERR${DEFAULT}                 │\n"
			else
				printf " Norme: ${COLOR_OK}✅ OK${DEFAULT}                  │\n"
			fi
		fi
	else
		if [ "$header_name" == "get_next_line.h" ]; then
			printf " ${COLOR_FAIL}❌ MISSING${DEFAULT}                     │\n"
		else
			printf " ${COLOR_WARNING}⚠️  OPTIONAL (MISSING)${DEFAULT}          │\n"
		fi
	fi
}

check_readme()
{
	printf "${BOLD}│ README.md         │${DEFAULT}"
	if [ -e "${PATH_LIBFT}"/README.md ]
	then
		printf " ${COLOR_OK}✅ FOUND${DEFAULT}                       │\n"
	else
		printf " ${COLOR_FAIL}❌ MISSING${DEFAULT}                     │\n"
	fi
}

func_check_file()
{
	printf "\n${COLOR_PART}─── Preliminary File Checks ───${DEFAULT}\n"
	printf "${BOLD}┌───────────────────┬───────────────────────────────────┐${DEFAULT}\n"

	check_header_gnl "get_next_line.h" "get_next_line.h"
	check_header_gnl "get_next_line_bonus.h" "get_next_line_b.h"
	check_readme
	printf "${BOLD}└───────────────────┴───────────────────────────────────┘${DEFAULT}\n"
}
