#include "minishell.h"

void	free_token(t_token **list)
{
	t_token *tmp;
	t_token	*next;

	tmp = *list;
	while(tmp)
	{
		free(tmp->value);
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	*list = NULL;
}
/*static void	free_redirection(s_redict *node)
{
	s_redict	*current;
	current = node;
	while (current)
	{
		if (current->delimiter)
			free(current->delimiter);
		free(current);
		current = current->next;
	}
}*/
void	free_redirection(s_redict *node)
{
	s_redict	*current;
	s_redict	*next;

	current = node;
	while (current)
	{
		next = current->next;
		if (current->delimiter)
			free(current->delimiter);
		free(current);
		current = next;
	}
}
void	free_ast(ASTnode *node)
{
	int	i;
	
	if (!node)
		return ;
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	if (node->args)
	{
		i = 0;
		while (node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	if (node->redirection)
		free_redirection(node->redirection);
	free(node);
}

void	free_ast_token(t_shell *shell_program)
{
	free_ast(shell_program->ast);
	shell_program->ast = NULL;
	free_token(&(shell_program->token_list));
}
void	free_envlist(t_env *env)
{
	t_env	*tmp;

	while (env)
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
	free_ast_token(shell_program);
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
	if (shell_program->stdin)
		close(shell_program->stdin);
	if (shell_program->stdout)
		close(shell_program->stdout);
	tcsetattr(STDIN_FILENO, TCSANOW, &shell_program->oldt);
}

void	exit_minishell(t_shell *shell_program)
{
	/*if (shell_program->stdin != -1)
		close(shell_program->stdin);
	if (shell_program->stdout != -1)
		close(shell_program->stdout);*/
	free_all(shell_program);
	//free_envlist(shell_program->envlist);
	exit(shell_program->exit_status);
}
