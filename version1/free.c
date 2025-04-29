#include "minishell.h"

void free_token(t_token *token)
{
    t_token *tmp;

    while(token)
    {
        tmp = token;
        token = token->next;
        free(tmp->value);
        //Besoin de free token->type??
        free(tmp);
    }
}

void free_ast(ASTnode *node)
{
    int i;

    i = 0;
    if(!node)
        return ;
    free_ast(node->left);
    free_ast(node->right);
    if (node->args)
    {
        while (node->args[i])
        {
            free(node->args[i]);
            i++;
        }
        free(node->args);
    }
    free(node->file);
    free(node->operator);
    free(node);
}

void free_envlist(t_env *env)
{
    t_env *tmp;

    while(env)
    {
        tmp = env;
        env = env->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}