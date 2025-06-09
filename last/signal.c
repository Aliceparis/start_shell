#include "minishell.h"
volatile sig_atomic_t g_signal_status = 0;

void	reset_signals_for_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_parent_signals(void)
{
	struct sigaction sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = handle_parent_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART; // Important pour que readline puisse reprendre
	//sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("sigaction");
	signal(SIGQUIT, SIG_IGN);
}

void	handle_parent_sigint(int sig)
{
	(void)sig;

	g_signal_status = 130; 
	write(STDOUT_FILENO, "^C\n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
void set_heredoc_sig(void)
{
    struct sigaction sa_heredoc;
    ft_bzero(&sa_heredoc, sizeof(sa_heredoc));
    sa_heredoc.sa_handler = handle_parent_heredoc;
    sigemptyset(&sa_heredoc.sa_mask);
    sa_heredoc.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa_heredoc, NULL) == -1)
        perror("sigaction heredoc failed");
    signal(SIGQUIT, SIG_IGN);
}
void	handle_parent_heredoc(int sig)
{
	(void)sig;

	g_signal_status = 130;
	write(STDOUT_FILENO, "\n", 1);
}
void restore_parent_signals(void)
{
    setup_parent_signals();
	g_signal_status = 0;
}
void setup_execution_signals(void)
{
    struct sigaction sa_ignore;
    
    // Set up signal action to ignore signals
    ft_bzero(&sa_ignore, sizeof(sa_ignore));
    sa_ignore.sa_handler = SIG_IGN;
    sigemptyset(&sa_ignore.sa_mask);
    sa_ignore.sa_flags = 0;
    
    // Ignore SIGINT in parent during child execution
    if (sigaction(SIGINT, &sa_ignore, NULL) == -1)
        perror("sigaction SIGINT ignore failed");
    
    // Ignore SIGQUIT in parent during child execution  
    if (sigaction(SIGQUIT, &sa_ignore, NULL) == -1)
        perror("sigaction SIGQUIT ignore failed");
}
void restore_shell_signals(void)
{
    struct sigaction sa;
    
    // Restore SIGINT handler for interactive shell
    ft_bzero(&sa, sizeof(sa));
    sa.sa_handler = handle_parent_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        perror("sigaction SIGINT restore failed");
    
    // Keep SIGQUIT ignored for interactive shell
    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
        perror("sigaction SIGQUIT restore failed");
}
