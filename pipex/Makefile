NAME = pipex
SRCS = srcs/pipex.c srcs/utils.c
OBJS = ${SRCS:.c=.o}
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
HEADER = -Iincludes
LIBFT_DIR = ./libft

all: ${NAME}

${NAME}: ${OBJS}
	@make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(HEADER) ${OBJS} libft/libft.a -o ${NAME}

%.o: %.c
	$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

clean:
	@make clean -C $(LIBFT_DIR)
	rm -f ${OBJS}

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re