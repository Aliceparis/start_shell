NAME=minishell
LIBFT =libft/libft.a

HEADER = ./include

CC = cc 
CFLAGS = -Wall -Wextra -Werror -g -O0 -Ilibft -I$(HEADER) 

VAL_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=supp.supp

SRC = main.c \
	pars/parse.c pars/parse_redirection.c pars/parse_simple_commande.c pars/parse2.c pars/parse_simple_commande2.c\
 	builtins/cd.c builtins/cd_end.c builtins/init_env.c builtins/echo.c builtins/env.c  builtins/unset.c builtins/exit.c  builtins/pwd.c builtins/export.c  \
 	dispatch_commande/exec2.c dispatch_commande/execute_buildin.c dispatch_commande/execute_simple_builtin.c dispatch_commande/heredoc2.c \
	dispatch_commande/pipeline2.c dispatch_commande/redirection.c dispatch_commande/exec.c dispatch_commande/execute_child.c dispatch_commande/gestion_fds.c \
	dispatch_commande/heredoc.c dispatch_commande/pipeline.c \
	utils/utils.c utils/free.c utils/free2.c signal/signal.c signal/signal_heredoc.c\
	token/tokenise.c token/token_utils.c  token/token_value.c \
	expansion/expand_variabe.c expansion/expansion.c expansion/expansion2.c
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