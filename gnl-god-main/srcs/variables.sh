#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    variables.sh                                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gemini-cli <gemini@tester.unit.com>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/23 18:27:13 by jtoty             #+#    #+#              #
#    Updated: 2026/06/03 12:00:00 by gemini-cli       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###################################################
#                 Part1 functions                 #
###################################################

Part1_func=('mandatory_bs_1' 'mandatory_bs_42' 'mandatory_bs_9999' 'mandatory_bs_10M')

# Authorized bits: 1=malloc, 2=free, 4=write
Part1_func_authorized=('1' '1' '1' '1')

Part1_func_activation=('0' '0' '0' '0')

###################################################
#                 Part2 functions                 #
###################################################

Part2_func=('bonus_bs_1' 'bonus_bs_42' 'bonus_bs_9999' 'bonus_bs_10M')

# Authorized bits: 1=malloc, 2=free, 4=write
Part2_func_authorized=('1' '1' '1' '1')

Part2_func_activation=('0' '0' '0' '0')

###################################################
#                 Part3 functions                 #
###################################################

Part3_func=('doom_bs_1' 'doom_bs_2' 'doom_bs_3' 'doom_bs_41' 'doom_bs_43' 'doom_bs_1000' 'doom_directory' 'doom_multi_fd_stress')

# Authorized bits: 1=malloc, 2=free, 4=write
Part3_func_authorized=('1' '1' '1' '1' '1' '1' '1' '1')

Part3_func_activation=('0' '0' '0' '0' '0' '0' '0' '0')

###################################################
#                 Other variables                 #
###################################################

tab_all_part=('Part1_func' 'Part2_func' 'Part3_func')

num_sys_func=('1' '2' '4')
system_func=('void' 'malloc' 'free' 'printf' 'write')

# COORDENADAS RECALCULADAS PARA TABELA LARGA (WIDE)
NORME_COL=23
CHEAT_COL=34
COMPIL_COL=51
LEAKS_COL=61
TEST_COL=71
RESULT_COL=133
TITLE_LENGTH=142
CHAR_LENGTH="-"
CHAR_WIDTH="|"

DIRECTORY=0

OPT_NO_SEARCH=0
OPT_NO_COLOR=0
OPT_NO_FORBIDDEN=0
OPT_NO_NORMINETTE=0
OPT_NO_UPDATE=0
OPT_FAST=0
OPT_NO_PART1=0
OPT_NO_PART2=0
OPT_NO_PART3=1
ACTIVATE_PART1=0
ACTIVATE_PART2=0
ACTIVATE_PART3=0
CHECK_IN_PART1=1
CHECK_IN_PART2=1
CHECK_IN_PART3=1
CUSTOM_DIRECTORY=0
