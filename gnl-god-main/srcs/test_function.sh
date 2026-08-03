/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_function.sh                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemini-cli <gemini@tester.unit.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 18:27:09 by jtoty             #+#    #+#              #
/*   Updated: 2026/06/12 12:00:00 by gemini-cli       ###   ########.fr        #
/*                                                                            */
/* ************************************************************************** */

#!/bin/bash

# Nova barra de progresso global que não polui as tabelas
update_global_bar()
{
	local current=$1
	local total=$2
	local width=60
	local percent=0
	[ $total -gt 0 ] && percent=$((current * 100 / total))
	local filled=0
	[ $total -gt 0 ] && filled=$((current * width / total))
	local empty=$((width - filled))
	
	printf "\r\033[K${COLOR_TITLE}👑 TOTAL PROGRESS: ["
	for ((j=0; j<filled; j++)); do printf "█"; done
	for ((j=0; j<empty; j++)); do printf "░"; done
	printf "] %d%%${DEFAULT}" $percent
}

check_turned_in_file_gnl()
{
	local scenario=$1
	local target=""
	if [[ "$scenario" == bonus* ]] || [ "$scenario" == "doom_multi_fd_stress" ]; then
		target="${PATH_LIBFT}/get_next_line_bonus.c"
	else
		target="${PATH_LIBFT}/get_next_line.c"
	fi

	if [ -e "$target" ]; then
		return 1
	else
		printf "\033[${NORME_COL}G${COLOR_FAIL}  NTI   ${DEFAULT}"
		printf "\033[${CHEAT_COL}G${COLOR_FAIL}      NTI      ${DEFAULT}"
		printf "\033[${COMPIL_COL}G${COLOR_FAIL}  NTI  ${DEFAULT}"
		printf "\033[${LEAKS_COL}G${COLOR_FAIL}  NTI  ${DEFAULT}"
		printf "\033[${TEST_COL}G${COLOR_FAIL}      NTI      ${DEFAULT}"
		printf "\033[${RESULT_COL}G${COLOR_FAIL}  NTI   ${DEFAULT}│"
		return 0
	fi
}

test_function()
{
	local part_name=$(echo ${part} | cut -d _ -f 1)
	printf "\n${COLOR_PART}─── Testing ${part_name} functions ───${DEFAULT}\n"
	
	# Cabeçalho da Tabela
	printf "${BOLD}┌───────────────────┬─────────┬───────────────┬────────┬────────┬──────────────────────────────────────────────────────────────┬────────┐${DEFAULT}\n"
	printf "${BOLD}│ SCENARIO          │ NORME   │ FORBIDDEN     │ COMPIL │ LEAKS  │ TESTS                                                        │ RESULT │${DEFAULT}\n"
	printf "${BOLD}├───────────────────┼─────────┼───────────────┼────────┼────────┼──────────────────────────────────────────────────────────────┼────────┤${DEFAULT}\n"

	local tab_part=$(echo ${part}[*])
	local func_list=(${!tab_part})
	local active_funcs=()
	local active_indices=()
	
	# Identifica cenários ativos
	local i=0
	for function_raw in "${func_list[@]}"
	do
		if [ $(( ${part}_activation[$i] )) -eq 1 ]
		then
			active_funcs+=("$function_raw")
			active_indices+=($i)
		fi
		let "i += 1"
	done
	
	# 1. Pre-renderiza a Tabela com status PENDING...
	for function_raw in "${active_funcs[@]}"
	do
		local func_name=$(echo "$function_raw")
		printf "│ %-17s │         │               │        │        │ %-60s │        │\n" "${func_name}" "PENDING..."
	done
	printf "${BOLD}└───────────────────┴─────────┴───────────────┴────────┴────────┴──────────────────────────────────────────────────────────────┴────────┘${DEFAULT}\n"
	
	# Inicializa a barra de progresso global
	update_global_bar $GLOBAL_CURRENT $GLOBAL_TOTAL
	printf "\n"

	# 2. Move o cursor de volta para cima, na primeira linha de teste
	local num_active=${#active_funcs[@]}
	local move_up=$((num_active + 2))
	printf "\033[${move_up}A"

	# 3. Executa os testes de cada cenário e atualiza in-place
	local success=0
	local idx=0
	for function_raw in "${active_funcs[@]}"
	do
		local i=${active_indices[$idx]}
		local func_name=$function_raw
		
		# Move para o início da linha e limpa
		printf "\r\033[K│ ${COLOR_FUNC}%-17s${DEFAULT} │" "${func_name}"
		
		check_turned_in_file_gnl $function_raw
		if [ $? -eq 1 ]
		then
			local func_result=1
			
			# Norme
			if [ ${OPT_NO_NORMINETTE} -eq 0 ]
			then
				check_norme $function_raw
				[ $? -eq 0 ] && func_result=0
			else
				printf "\033[${NORME_COL}G  SKIP  "
			fi
			
			# Forbidden functions (Cheat check)
			if [ ${OPT_NO_FORBIDDEN} -eq 0 ]
			then
				check_cheating $function_raw
				[ $? -eq 1 ] && func_result=0
			else
				printf "\033[${CHEAT_COL}G    SKIP     "
			fi

			# Compilation
			compilation $function_raw
			check_compilation
			if [ $? -eq 1 ]
			then
				# Run Tests
				run_gnl_tests $function_raw
				if [ $? -eq 0 ]; then
					func_result=0
				fi
			else
				printf "\033[${LEAKS_COL}G${COLOR_FAIL} FAIL ${DEFAULT}"
				printf "\033[${TEST_COL}G${COLOR_FAIL}     FAIL      ${DEFAULT}"
				func_result=0
			fi
			
			# Resultado Final
			printf "\033[${RESULT_COL}G"
			if [ $func_result -eq 0 ]
			then
				printf "${COLOR_FAIL}  KO  ${DEFAULT}│"
			else
				printf "${COLOR_OK}  OK  ${DEFAULT}│"
				let "success += 1"
			fi
		fi
		
		# Atualiza a barra global no rodapé
		let "GLOBAL_CURRENT += 1"
		local lines_to_bar=$((num_active - idx + 1))
		printf "\033[${lines_to_bar}B"
		update_global_bar $GLOBAL_CURRENT $GLOBAL_TOTAL
		printf "\033[${lines_to_bar}A"
		
		# Avança para a próxima linha
		printf "\n"
		let "idx += 1"
		[ -e "${PATH_TEST}"/user_exe ] && rm -f "${PATH_TEST}"/user_exe
	done
	
	# Move o cursor após a tabela e a barra global
	printf "\033[1B\n"
	local color_summary="${COLOR_OK}"
	[ $success -lt ${#active_funcs[@]} ] && color_summary="${COLOR_FAIL}"
	printf "${BOLD}${part_name} Summary: ${color_summary}${success}/${#active_funcs[@]}${DEFAULT} scenarios passed.\n"
}

run_gnl_tests()
{
	local scenario=$1
	local bs=42
	[[ "$scenario" == *_bs_1 ]] && bs=1
	[[ "$scenario" == *_bs_2 ]] && bs=2
	[[ "$scenario" == *_bs_3 ]] && bs=3
	[[ "$scenario" == *_bs_41 ]] && bs=41
	[[ "$scenario" == *_bs_42 ]] && bs=42
	[[ "$scenario" == *_bs_43 ]] && bs=43
	[[ "$scenario" == *_bs_1000 ]] && bs=1000
	[[ "$scenario" == *_bs_9999 ]] && bs=9999
	[[ "$scenario" == *_bs_10M ]] && bs=10000000

	local test_file="mandatory.cpp"
	local bonus_flag=""
	local is_bonus=0

	if [[ "$scenario" == bonus* ]]; then
		test_file="bonus.cpp"
		bonus_flag="-D GNL_BONUS"
		is_bonus=1
	elif [ "$scenario" == "doom_directory" ]; then
		test_file="doom_directory.cpp"
	elif [ "$scenario" == "doom_multi_fd_stress" ]; then
		test_file="doom_multi_fd_stress.cpp"
		bonus_flag="-D GNL_BONUS"
		is_bonus=1
	fi

	local utils_path="${PATH_TEST}/utils"
	local utils_src="${utils_path}/sigsegv.cpp ${utils_path}/color.cpp ${utils_path}/check.cpp ${utils_path}/gnl.cpp ${utils_path}/leaks.cpp"
	local cppflags="-g3 -ldl -std=c++11 -I ${utils_path} -I ${PATH_LIBFT} -Wno-everything"
	
	printf "\n= %s ================================================================\n" "${scenario}" >> "${PATH_DEEPTHOUGHT}"/deepthought

	# 1. Compila os arquivos C do estudante separadamente como C puro usando 'cc'
	if [ $is_bonus -eq 1 ]; then
		cc -Wall -Wextra -Werror -D BUFFER_SIZE=$bs -c ${PATH_LIBFT}/get_next_line_bonus.c -o "${PATH_TEST}/get_next_line.o" >> "${PATH_DEEPTHOUGHT}"/deepthought 2>&1
		local res_c1=$?
		cc -Wall -Wextra -Werror -D BUFFER_SIZE=$bs -c ${PATH_LIBFT}/get_next_line_utils_bonus.c -o "${PATH_TEST}/get_next_line_utils.o" >> "${PATH_DEEPTHOUGHT}"/deepthought 2>&1
		local res_c2=$?
	else
		cc -Wall -Wextra -Werror -D BUFFER_SIZE=$bs -c ${PATH_LIBFT}/get_next_line.c -o "${PATH_TEST}/get_next_line.o" >> "${PATH_DEEPTHOUGHT}"/deepthought 2>&1
		local res_c1=$?
		cc -Wall -Wextra -Werror -D BUFFER_SIZE=$bs -c ${PATH_LIBFT}/get_next_line_utils.c -o "${PATH_TEST}/get_next_line_utils.o" >> "${PATH_DEEPTHOUGHT}"/deepthought 2>&1
		local res_c2=$?
	fi
	
	if [ $res_c1 -ne 0 ] || [ $res_c2 -ne 0 ]; then
		printf "\033[${TEST_COL}G  ${COLOR_FAIL}❌ COMPILATION ERROR${DEFAULT}"
		rm -f "${PATH_TEST}"/*.o
		return 0
	fi

	# 2. Compila e vincula (link) os arquivos de teste C++ com os objetos C gerados
	clang++ -D BUFFER_SIZE=$bs -D TIMEOUT_US=1000000 $bonus_flag $cppflags $utils_src "${PATH_TEST}/tests/${test_file}" "${PATH_TEST}/get_next_line.o" "${PATH_TEST}/get_next_line_utils.o" -o "${PATH_TEST}/user_exe" >> "${PATH_DEEPTHOUGHT}"/deepthought 2>&1
	local res_cpp=$?
	rm -f "${PATH_TEST}"/*.o
	
	if [ $res_cpp -ne 0 ]; then
		printf "\033[${TEST_COL}G  ${COLOR_FAIL}❌ COMPILATION ERROR${DEFAULT}"
		return 0
	fi

	# Execução
	local output=$("${PATH_TEST}/user_exe" < "${PATH_TEST}/files/alternate_line_nl_with_nl" 2>&1)

	# Tratamento do resultado
	local total=$(echo "$output" | grep -o "[0-9]\+\.[OK|KO|MOK|MKO]" | wc -l)
	local passed=$(echo "$output" | grep -o "[0-9]\+\.OK" | wc -l)
	local leaks=$(echo "$output" | grep "LEAKS.KO")
	
	# Renderização da barra de teste in-place
	local bar_width=40
	local percent=0
	[ $total -gt 0 ] && percent=$((passed * 100 / total))
	local filled=0
	[ $total -gt 0 ] && filled=$((passed * bar_width / total))
	local empty=$((bar_width - filled))
	
	local color_bar="${COLOR_OK}"
	[ $percent -lt 100 ] && color_bar="${COLOR_FAIL}"
	
	printf "\033[${TEST_COL}G ${COLOR_INFO}[%03d/%03d]${DEFAULT} ${color_bar}[" $passed $total
	for ((j=0; j<filled; j++)); do printf "█"; done
	for ((j=0; j<empty; j++)); do printf "░"; done
	printf "] %3d%%${DEFAULT}" $percent

	# Leaks
	printf "\033[${LEAKS_COL}G"
	if [ -z "$leaks" ]; then
		printf " ${COLOR_OK}OK${DEFAULT}  "
	else
		printf " ${COLOR_FAIL}KO${DEFAULT}  "
	fi

	# Log detalhado no deepthought (limpando sequências de escape ANSI e formatando visualmente)
	local clean_output=$(echo "$output" | sed $'s/\e\\[[0-9;]*[a-zA-Z]//g' | sed 's/\.OK/.OK ✅/g' | sed 's/\.KO/.KO ❌/g' | sed 's/\.MOK/.MOK 👑/g' | sed 's/\.MKO/.MKO 🚨/g')
	printf "%s\n" "$clean_output" >> "${PATH_DEEPTHOUGHT}"/deepthought

	if [ $percent -eq 100 ] && [ -z "$leaks" ]; then
		return 1
	else
		return 0
	fi
}
