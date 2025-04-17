#ifdef TOKENISING_H
# define TOKENISING_H

/*括号是否是一个token的分隔符号？ */

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
    token_type  type;
    char    *value;
    struct s_token  *next;
}t_token;

#endif