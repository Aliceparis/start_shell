/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmen <jmen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:39:36 by yujin             #+#    #+#             */
/*   Updated: 2025/06/16 10:59:27 by jmen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*ctrlc handle : 
1. write a new line in output;
2. tell readline to move to a new line;
3. clear the current input; 
4. re display the pompt
*/
void	handle_parent_sigint(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "^C\n", 3);
	g_heredoc_sigint = 1;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*setup_signe : at pompt + child or heredoc finish
sigint : handle by handle_parent_sigint
sigquit : ignored*/
void	setup_signe_parent(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handle_parent_sigint;
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("sigaction SIGINT failed");
	signal(SIGQUIT, SIG_DFL);
}

void	setup_parent_for_child_execution(void)
{
	struct sigaction	sa_ignore;

	ft_bzero(&sa_ignore, sizeof(sa_ignore));
	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_ignore, NULL) == -1)
		perror("sigaction SIGINT ignore for child execution failed");
	if (sigaction(SIGQUIT, &sa_ignore, NULL) == -1)
		perror("sigaction SIGQUIT ignore for child exection failed");
}

void	reset_signals_for_child(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("sigaction SIGINT for child default failed");
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		perror("sigaction SIGQUIT for child default failed");
}
