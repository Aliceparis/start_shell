NAME=Minishell
LIBFT =libft/libft.a

HEADER = ./include

CC = cc 
CFLAGS = -Wall -Wextra -Werror -g3 -Ilibft -I$(HEADER) 

VAL_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=supp.supp

SRC = main.c expansion.c  pars/parse.c pars/parse_redirection.c pars/parse_simple_commande.c \
 builtins/cd.c builtins/init_env.c builtins/echo.c builtins/env.c  builtins/unset.c builtins/exit.c  builtins/pwd.c builtins/export.c  \
 dispatch_commande/redirection.c dispatch_commande/exec.c  dispatch_commande/execute_buildin.c\
 dispatch_commande/execute_simple_builtin.c dispatch_commande/gestion_fds.c dispatch_commande/pipeline.c \
	 tokenise.c utils.c  heredoc.c free.c signal.c
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