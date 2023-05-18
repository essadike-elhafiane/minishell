/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:36:35 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/15 19:40:34 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	heredoc_fork_h_1(t_stk *y, char *ex, t_cmd **cmds)
{
	if (y->ss[y->i] == '$' && y->ss[y->i +1] != '\0'
		&& (y->ss[y->i + 1] != '$' && y->ss[y->i + 1] != ' '))
	{
		y->i++;
		y->b = y->i;
		while ((ft_isalpha(y->ss[y->i]) || ft_isdigit(y->ss[y->i])))
			y->i++;
		ex = ft_substr(y->ss, y->b, y->i - y->b);
		ex = expand(ex, *(*cmds)->env);
		y->back = ft_strdup(y->ss + y->i);
		if (!ex && y->ss[y->i] && y->back)
			y->i = 0;
		y->front = ft_substr(y->ss, 0, y->b -1);
		free(y->ss);
		y->ss = ft_strjoin(y->front, ex);
		if (y->ss[y->i - 1] != '$')
			y->i--;
		y->ss = ft_strjoin(y->ss, y->back);
	}
}

void	heredoc_fork_h(t_stk *y, t_shell **tmp, char *ex, t_cmd **cmds)
{
	if (y->ss && (*tmp)->type != SINGLE && (*tmp)->type != DOUBLE)
	{
		y->i = 0;
		while (y->ss && y->ss[y->i])
		{
			if (y->ss[y->i] == '$' && (y->ss[y->i + 1] == '$'))
				y->i++;
			heredoc_fork_h_1(y, ex, cmds);
			y->i++;
		}
	}
}

void	heredoc_fork(t_stk *y, t_shell **tmp, int *fd, t_cmd **cmds)
{
	char	*ex;

	ex = NULL;
	close(fd[0]);
	while (1)
	{
		y->ss = readline("> ");
		if (!y->ss)
			exit (0);
		if (word_stop((*tmp)->s, y->ss))
			break ;
		heredoc_fork_h(y, tmp, ex, cmds);
		write(fd[1], y->ss, ft_strlen(y->ss));
		write(fd[1], "\n", 1);
		free(y->ss);
	}
	close(fd[1]);
	exit(0);
}

int	creat_heredoc_help(t_cmd **cmds, t_shell **tmp, t_stk *y, t_cmd *tmp_cmd)
{
	int	fd[2];
	int	ha;

	if (pipe(fd) == -1)
		return (perror("pipe"), 1);
	y->pid = fork();
	if (y->pid == -1)
		return (perror("fork"), 1);
	else if (y->pid == 0)
		heredoc_fork(y, tmp, fd, cmds);
	else
	{
		close(fd[1]);
		wait(&ha);
		g_status = WEXITSTATUS(ha);
		if (WEXITSTATUS(ha) == 3)
			return (fun_free_cmd(cmds), 1);
		signal(SIGINT, signal_handler);
		tmp_cmd->fd_input = fd[0];
		(*tmp) = (*tmp)->next;
	}
	return (0);
}

int	creat_heredoc(t_cmd **cmds, t_shell **tmp, t_stk *y, t_cmd *tmp_cmd)
{
	if ((*tmp) && (*tmp)->type == HER)
	{
		signal(SIGINT, signal_her);
		signal(SIGTERM, signal_her);
		y->ss = NULL;
		(*tmp) = (*tmp)->next;
		if ((*tmp) && (*tmp)->type == WSPACE)
			(*tmp) = (*tmp)->next;
		while ((*tmp) && (*tmp)->next && (*tmp)->next->type
			!= WSPACE && !check_is_token((*tmp)->next->type))
		{
			(*tmp)->next->s = ft_strjoin_no_free((*tmp)->s, (*tmp)->next->s);
			if ((*tmp)->type != WORD)
				(*tmp)->next->type = SINGLE;
			(*tmp) = (*tmp)->next;
		}
		if (creat_heredoc_help(cmds, tmp, y, tmp_cmd))
			return (1);
	}
	return (0);
}
