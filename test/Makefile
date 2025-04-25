NAME=Minishell
LIBFT =libft/libft.a

HEADER = ./include

CC = cc 
CFLAGS = -Wall -Wextra -Werror -g3 -Ilibft -I$(HEADER) 


SRC = cd.c  dispatch_commande.c echo.c env.c execute_buildin.c exit.c expansion.c export.c init_env.c main.c \
	parse.c pwd.c tokenise.c unset.c pipex.c
OBJ = $(SRC:.c=.o)

all : $(LIBFT) $(NAME)

$(LIBFT) : 
	@make -C libft all > /dev/null && echo "✅ compile libft réussi"

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT)

clean : 
	rm -rf $(OBJ)
	make -C libft clean 

fclean : clean 
	rm -rf $(NAME)
	make -C libft fclean 

re : fclean all 

.PHONY : all libft clean fclean re 