#ifndef TOKENISING_H
# define TOKENISING_H

/*括号是否是一个token的分隔符号？ */

typedef struct s_shell t_shell;
typedef enum e_token_type
{
	IS_WORD,
	IS_PIPE, // |
	REDICT_OUT, // >
	REDICT_IN, // < 
	REDICT_APPEND, // >>
	IS_HEREDOC, // <<
	IS_EOF,
	//( ???
	//) ???
}token_type;

typedef struct s_token
{
	token_type		type;
	char			*value;
	struct s_token	*next;
}t_token;

/******************tokenise.c****************/
int			ft_isspace(char c );
void		skip_space(char **input);
int			is_quote(char c);
int			is_quote_close(char *str);

char		*get_between_quote(char **input);
int			is_token_operator(char *input);
char		*get_token_operator(char **input);
char		*get_token_word(char **input);
void		ft_token(char *line, t_shell *shell_program);
token_type	get_type_of_token(char *str);
void		update_token_value(token_type type, char *value, t_token **token_list);

char    *get_next_token(char **line);

#endif