/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yujin <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:42:47 by yujin             #+#    #+#             */
/*   Updated: 2025/06/14 14:42:49 by yujin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Global variable used for heredoc SIGINT
volatile sig_atomic_t	g_heredoc_sigint = 0;

void	handle_heredoc_sigint_child(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "^C\n", 3);
	g_heredoc_sigint = 1;
	close(0);
}

void	setup_heredoc_child(void)
{
	struct sigaction	sa_heredoc;

	ft_bzero(&sa_heredoc, sizeof(sa_heredoc));
	sa_heredoc.sa_handler = handle_heredoc_sigint_child;
	sigemptyset(&sa_heredoc.sa_mask);
	sa_heredoc.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_heredoc, NULL) == -1)
		perror("sigaction heredoc child SIGINT failed");
	signal(SIGQUIT, SIG_IGN);
}
