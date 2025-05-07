#include "minishell.h"

void free_token(t_token **list)
{
    t_token *tmp;

    while(*list)
    {

		tmp = (*list)->next;
		free((*list)->value);   // value 是 strdup 出来的
		free(*list);          // 释放结构体本身
		*list = tmp;
    }
}

/*void free_ast(ASTnode *node)
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
	if (node->file)
    	free(node->file);
	if (node->operator)
    	free(node->operator);
    free(node);
}*/
void free_ast(t_shell *shell_program)
{
	ASTnode	*node;

	node = shell_program->ast;
	if (!node)
		return ;
	if (node->type == CMD)
		free_cmd_node(node);
	else
	{
		if (node->left)
			free_ast(shell_program);
		if (node->right)
			free_ast(shell_program);
		if (node->file)
			free_ast(shell_program);
		if (node->operator)
			free(shell_program);
	}
	free(node);
	free_token(&(shell_program->token_list));
}
void	free_cmd_node(ASTnode *node)
{
	int	i;

	i = 0;
	if (node->args)
	{
		while (node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
}
void free_envlist(t_env *env)
{
    t_env *tmp;

    while(env)
    {
        tmp = env;
        env = env->next;
        free(tmp);
    }
	env = NULL;
}

void	free_all(t_shell *shell_program)
{
	clean_old_content(NULL, true);
	printf("free1\n");
	free_token(&(shell_program->token_list));
	shell_program->token_list = NULL;
	if (shell_program)
	{
		printf("free2\n");
		free_ast(shell_program);
		shell_program->ast = NULL;
	}
	if (shell_program->envlist)
	{
		free_envlist(shell_program->envlist);
		shell_program->envlist = NULL;
	}
	if (shell_program->line)
	{
		free(shell_program->line);
		shell_program->line = NULL;
	}
	rl_clear_history();
	tcsetattr(STDIN_FILENO, TCSANOW, &shell_program->oldt);
	//if (shell_program)
		//free(shell_program);
}
void    exit_minishell(t_shell shell_program)
{
    if (shell_program.stdin != -1)
        close(shell_program.stdin);
    if (shell_program.stdout != -1)
        close(shell_program.stdout);
    free_envlist(shell_program.envlist);
    exit(shell_program.exit_status);
}