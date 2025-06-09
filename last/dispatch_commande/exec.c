#include "minishell.h"

void	ft_execute_node(t_shell *shell_program, ASTnode *node)
{
	if (!node)
		return ;
	if (node->type == PIPE)
		execute_pipeline(shell_program, node);
	else
		execute_simple_command(shell_program, node);
}

int execute_simple_command(t_shell *shell_program, ASTnode *node)
{
    if (node->expanded_args == NULL)
        return (execute_redirection(shell_program, node));
    if (is_builtin(node->expanded_args[0]))
        return (execute_builtin_in_shell(shell_program, node));
    else
    {
        execute_externe_command(shell_program, node);
        return (shell_program->exit_status);
    }
}

int execute_redirection(t_shell *shell_program, ASTnode *node)
{
    int saved_stdin;
    int saved_stdout;
    int redir_status;

    saved_stdin = -1;
    saved_stdout = -1;
    if (check_redirection(node))
    {
        saved_stdin = dup(STDIN_FILENO);
        saved_stdout = dup(STDOUT_FILENO);
        if (saved_stdin == -1 || saved_stdout == -1)
        {
            shell_program->exit_status = 1;
            return (perror("Dup failed in redirection"), 1);
        }
        redir_status = handle_redirection(shell_program, node);
        if (saved_stdout != -1)
			close_and_restore(saved_stdout, STDOUT_FILENO);
		if (saved_stdin != -1)
			close_and_restore(saved_stdin, STDIN_FILENO);
		if (redir_status != 0)
			return (shell_program->exit_status = 1);
    }
    shell_program->exit_status = 0;
    return (0);
}

int execute_builtin_in_shell(t_shell *shell_program, ASTnode *cmd_node)
{
    int saved_stdin;
    int saved_stdout;
    int redir_status;

    saved_stdin = -1;
    saved_stdout = -1;
    if (check_redirection(cmd_node))
    {
        saved_stdin = dup(STDIN_FILENO);
        saved_stdout = dup(STDOUT_FILENO);
        if (saved_stdin == -1 || saved_stdout == -1)
        {
            shell_program->exit_status = 1;
            return (perror("Dup failed in redirection"), 1);
        }
        redir_status = handle_redirection(shell_program, cmd_node);
        if (redir_status != 0)
		{
			if (saved_stdout != -1)
				close_and_restore(saved_stdout, STDOUT_FILENO);
			if (saved_stdin != -1)
				close_and_restore(saved_stdin, STDIN_FILENO);
			shell_program->exit_status = 1;
			close(saved_stdin);
			close(saved_stdout);
			return (1);
		}
    }
    shell_program->exit_status = excute_builtin(shell_program, cmd_node->expanded_args);
    if (saved_stdout != -1) 
        close_and_restore(saved_stdout, STDOUT_FILENO);
    if (saved_stdin != -1) 
        close_and_restore(saved_stdin, STDIN_FILENO);
    return (shell_program->exit_status);
}

static void handle_child_process_execution(t_shell *shell_program, ASTnode *cmd_node, char *path)
{
    // Reset signals to default or specific child behavior
    // This is crucial to avoid inheriting parent's signal handlers that might terminate the shell.
    reset_signals_for_child();

    // Handle I/O redirections if any are present for this command
    if (check_redirection(cmd_node) == 1)
    {
        if (handle_redirection(shell_program, cmd_node) != 0)
        {
            // Redirection failed; print error, free child's specific resources, and exit.
            // shell_program->exit_status would have been set by handle_redirection.
            free(path); // Free the path string allocated by find_path
            free_all(shell_program); // Free all shell-wide resources that child might have inherited
            exit(shell_program->exit_status);
        }
    }

    // Execute the command using execve
    // If execve succeeds, the process image is replaced, and memory is automatically reclaimed by the OS.
    if (execve(path, cmd_node->expanded_args, shell_program->environ) == -1)
    {
        // execve failed; this means the new program couldn't be loaded or executed.
        perror("minishell: execve failed"); // Print a system error message
        free(path); // IMPORTANT: Free the 'path' string since execve failed and didn't replace the process.
        free_all(shell_program); // Free all shell-wide resources in the child before exiting.
        exit(127); // Standard exit status for command not found or execve failure (126 for permission denied, 127 for not found).
    }
    // Code below this line is unreachable if execve succeeds.
}

/**
 * @brief Handles waiting for the child process and updating the shell's exit status.
 * This runs in the parent process after forking.
 * @param shell_program The shell context.
 * @param pid The PID of the child process.
 */
static void handle_parent_process_wait(t_shell *shell_program, pid_t pid)
{
    int status = -1; // To store the child's exit status information

    setup_execution_signals();
    // Wait for the specific child process identified by 'pid'
    // '0' as the options argument means wait for any state change.
    waitpid(pid, &status, 0);
    restore_shell_signals();
    // Check how the child process terminated and set the shell's exit status accordingly.
    if (WIFEXITED(status))
    {
        // Child exited normally (e.g., via 'exit()' call in its main function)
        shell_program->exit_status = WEXITSTATUS(status); // Get the actual exit code
    }
    else if (WIFSIGNALED(status))
    {
        // Child was terminated by a signal (e.g., SIGINT, SIGQUIT, SIGSEGV)
        shell_program->exit_status = 128 + WTERMSIG(status); // Shell standard for signal termination
        if (WTERMSIG(status) == SIGINT)
        {
            // If terminated by SIGINT (Ctrl+C), set a flag for the shell to handle
            write(STDERR_FILENO, "\r", 1); // Print a newline to clean up the prompt after Ctrl+C
        }
        else if (WTERMSIG(status) == SIGQUIT)
        {
            // If terminated by SIGQUIT (Ctrl+\), print "Quit (core dumped)"
            write(STDERR_FILENO, "Quit (core dumped)\n", 19);
        }
    }
    // Additional checks can be added for WIFSTOPPED (child stopped by signal)
    // or WIFCONTINUED (child resumed), if implementing job control.
}
void	execute_externe_command(t_shell *shell_program, ASTnode *cmd_node)
{
	pid_t	pid;
	char	*path;

	if (!cmd_node)
		error_message(shell_program, "command split failed", 1);
	path = find_path(cmd_node->expanded_args[0], shell_program->environ);
	if (!path)
	{
        free_array(&path);
		error_message(shell_program, "command not found", 127);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		free(path);
		shell_program->exit_status = 1;
		return ;
	}
	if (pid == 0)
	{
		handle_child_process_execution(shell_program, cmd_node, path);
	}
	else
	{
		handle_parent_process_wait(shell_program, pid);
	}
}
