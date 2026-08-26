NAME        = push_swap
LIBFT_DIR   = ./libft
LIBFT       = $(LIBFT_DIR)/libft.a
SRC_DIR     = src
INCLUDE_DIR = include

CC          = cc
CFLAGS      = -g -Wall -Wextra -Werror -I./libft -I$(INCLUDE_DIR)
RM          = rm -f

SRCS        = $(SRC_DIR)/main.c	\
			$(SRC_DIR)/stack/stack_manipulation_tools.c	\
			$(SRC_DIR)/stack/stack_creation_tools.c \
			$(SRC_DIR)/parsing/input_validation.c \
			$(SRC_DIR)/parsing/input_normalization.c \
			$(SRC_DIR)/parsing/flags.c \
			$(SRC_DIR)/operations/swap.c \
			$(SRC_DIR)/operations/push.c \
			$(SRC_DIR)/operations/rotate.c \
			$(SRC_DIR)/operations/reverse_rotate.c \
			$(SRC_DIR)/operations/operation_output.c \
			$(SRC_DIR)/metrics/disorder.c \
			$(SRC_DIR)/metrics/benchmark.c \
			$(SRC_DIR)/metrics/index.c \
			$(SRC_DIR)/sorting/simple_sort.c \
			$(SRC_DIR)/sorting/strategy.c \
			$(SRC_DIR)/sorting/complex_sort.c \
			$(SRC_DIR)/sorting/medium_blocks.c \
			$(SRC_DIR)/sorting/medium_restore.c \
			$(SRC_DIR)/sorting/small_sort.c

OBJS        = $(SRCS:.c=.o)

all: $(NAME)


$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
	$(MAKE)	-C	$(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
