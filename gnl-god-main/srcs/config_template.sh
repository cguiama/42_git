#!/bin/bash

###################################
#     Mandatory configuration     #
###################################

# Fallback path if auto-detection fails. Edit this if needed.
FALLBACK_PATH="../"

if [ -f "${PATH_TEST}/../get_next_line.c" ] || [ -f "${PATH_TEST}/../get_next_line_bonus.c" ]; then
	PATH_LIBFT="${PATH_TEST}/.."
elif [ -f "${PATH_TEST}/../GNL/get_next_line.c" ] || [ -f "${PATH_TEST}/../GNL/get_next_line_bonus.c" ]; then
	PATH_LIBFT="${PATH_TEST}/../GNL"
else
	PATH_LIBFT="${FALLBACK_PATH}"
fi

#If you use a relative path, don't put double quotes.
#For example :
#PATH_LIBFT=~/gnl		==> Right
#PATH_LIBFT="~/gnl"	==> Wrong

#If you encounter some problems with a relative path, use an absolute path.
#You can put double quotes with the absolute path.
#For example :
#PATH_LIBFT=/home/user/gnl	==> Right
#PATH_LIBFT="/home/user/gnl"	==> Right


##############
# HEADER DIR #
##############
# Edit with the name of your header's directory (usually empty for GNL)
# Keep it empty if you have your header file in your main directory
HEADER_DIR="."

###########
# SRC DIR #
###########
# Edit with the name of your sources' directory (usually empty for GNL)
# Keep it empty if you have your header file in your main directory
SRC_DIR="."


###################################
#     Optional configuration      #
###################################


#By default, the deepthought file will be generated in the grademe directory.
#Edit the next line with an absolute path if you want to change it.
PATH_DEEPTHOUGHT="${PATH_TEST}"

#You can chose color and text effect.
COLOR_OK="${GREEN}"
COLOR_FAIL="${RED}"
COLOR_WARNING="${YELLOW}"
COLOR_TITLE="${BOLD}${BLUE}"
COLOR_FUNC="${CYAN}"
COLOR_PART="${UNDERLINE}${PURPLE}"
COLOR_TOTAL="${BOLD}${YELLOW}"
COLOR_DEEPTHOUGHT_PATH="${BOLD}${PURPLE}"
