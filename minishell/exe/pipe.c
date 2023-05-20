/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mserrouk <mserrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:08:03 by mserrouk          #+#    #+#             */
/*   Updated: 2023/05/20 21:38:48 by mserrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_not_fork(t_cmd *cmd)
{
	if (cmd->cmd && ft_strnstr(cmd->cmd[0], "export", 6))
	{
		if (!cmd->cmd[1])
			return (0);
		if (cmd->cmd[1] && cmd->cmd[1][0] == '\0')
		{
			ft_putstr_fd("minishell: export: `': not a valid identifier\n", 2);
			g_status = 1;
			return (1);
		}
		if (check_export(cmd->env, cmd))
			return (0);
		return (1);
	}
	else if (cmd->cmd && cmd->cmd[0] && word_stop (cmd->cmd[0], "cd"))
		return (1);
	else if (cmd->cmd && word_stop(cmd->cmd[0], "unset") && cmd->cmd[1] == NULL)
		return (1);
	else if (cmd->cmd && word_stop(cmd->cmd[0], "unset") && cmd->cmd[1] != NULL)
	{
		cmd_unset(cmd->env, cmd);
		return (1);
	}
	return (0);
}

void	wait_cmd(t_stk y)
{
	while (y.i > 0)
	{
		if (waitpid(-1, &y.b, 0) == y.tmp2->pid)
		{
			if (WIFSIGNALED(y.b))
				g_status = 128 + WTERMSIG(y.b);
			if (WTERMSIG(y.b) == 3)
				ft_putstr_fd("Quit: 3\n", 2);
			else if (WIFEXITED(y.b))
				g_status = WEXITSTATUS(y.b);
		}
		y.i -= 1;
	}
}

void	pipe_core(t_stk y)
{
	while (y.tmp1)
	{
		if (y.tmp1->next)
			pipe(y.tmp1->fd);
		if (!is_not_fork(y.tmp1))
		{
			y.tmp1->pid = fork();
			if (y.tmp1->pid == 0)
			{
				signal(SIGQUIT, SIG_DFL);
				signal(SIGINT, SIG_DFL);
				if (y.tmp1->next)
					close(y.tmp1->fd[0]);
				ft_command(y.tmp1, y.tmp2, y.i, y);
			}
			if (y.tmp1->next)
				close(y.tmp1->fd[1]);
			if (y.tmp2)
				close(y.tmp2->fd[0]);
		}
		y.tmp2 = y.tmp1;
		y.tmp1 = y.tmp1->next;
		y.i += 1;
	}
	wait_cmd(y);
}

void	ft_pipe(t_cmd *cmd)
{
	t_stk	y;

	y.tmp2 = NULL;
	y.i = 0;
	y.tmp1 = cmd;
	while (y.tmp1)
	{
		y.tmp1 = y.tmp1->next;
		y.i += 1;
	}
	y.i = 0;
	y.tmp1 = cmd;
	pipe_core(y);
}
