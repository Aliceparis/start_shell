NAME=Minishell
LIBFT =libft/libft.a

HEADER = ./include

CC = cc 
CFLAGS = -Wall -Wextra -Werror -g3 -Ilibft -I$(HEADER) 

VAL_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=supp.supp

SRC = builtins/cd.c dispatch_commande.c builtins/echo.c builtins/env.c execute_buildin.c builtins/exit.c expansion.c builtins/export.c init_env.c main.c \
	parse.c builtins/pwd.c tokenise.c builtins/unset.c utils.c  heredoc.c free.c
OBJ = $(SRC:.c=.o)

all : $(LIBFT) $(NAME)
	
$(LIBFT) : 
	@make -C libft bonus > /dev/null && echo "✅ compile libft réussi"

$(NAME) : $(OBJ)

	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline 

clean : 
	rm -rf $(OBJ)
	make -C libft clean 

fclean : clean 
	rm -rf $(NAME)
	make -C libft fclean 

re : fclean all 

valgrind: all
	make re 
	@valgrind $(VAL_FLAGS) ./$(NAME)


.PHONY : all libft clean fclean re 