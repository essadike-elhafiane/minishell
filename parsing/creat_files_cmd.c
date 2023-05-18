/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_files_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 20:08:08 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/15 23:27:55 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	signal_her(int signal)
{
	if (signal == SIGINT)
	{
		if (waitpid(-1, NULL, WNOHANG))
			exit(3);
	}
}

void	creat_word_cmd_h(t_cmd *tmp_cmd, t_shell **r, t_stk *y)
{
	int		k;
	char	**old_cmd;

	k = 0;
	y->j = 0;
	if (tmp_cmd->cmd)
	{
		while (tmp_cmd->cmd[k])
			k++;
		old_cmd = tmp_cmd->cmd;
		y->i += k;
	}
	tmp_cmd->cmd = (char **)malloc(sizeof(char *) * (y->i + 1));
	if (k)
	{
		while (y->j < k)
		{
			tmp_cmd->cmd[y->j] = ft_strdup(old_cmd[y->j]);
			y->j++;
		}
		free_double(old_cmd);
	}
	tmp_cmd->cmd[y->i] = NULL;
	creat_word_cmd_h_1(tmp_cmd, r, y);
}

void	creat_word_cmd(t_cmd *tmp_cmd, t_shell **tmp, t_stk *y)
{
	t_shell	*r;

	if ((*tmp) && (*tmp)->type == WSPACE)
		(*tmp) = (*tmp)->next;
	r = (*tmp);
	while ((*tmp) && ((*tmp)->type == WORD
			|| (*tmp)->type == DOUBLE || (*tmp)->type == SINGLE))
	{
		y->i = 0;
		while ((*tmp) && ((*tmp)->type == WORD || (*tmp)->type == DOUBLE
				|| (*tmp)->type == SINGLE || (*tmp)->type == WSPACE))
		{
			if ((*tmp) && (*tmp)->type == WSPACE)
				(*tmp) = (*tmp)->next;
			if ((*tmp) && ((*tmp)->type == WORD
					|| (*tmp)->type == DOUBLE || (*tmp)->type == SINGLE))
			{
				y->i++;
				while ((*tmp) && ((*tmp)->type == WORD
						|| (*tmp)->type == DOUBLE || (*tmp)->type == SINGLE))
					(*tmp) = (*tmp)->next;
			}
		}
		creat_word_cmd_h(tmp_cmd, &r, y);
	}
}

int	creat_cmd_1(t_cmd **cmds, t_shell **tmp, t_stk *y, t_cmd **tmp_cmd)
{
	while ((*tmp) && (*tmp)->type != PIPE)
	{
		if ((*tmp) && (*tmp)->type == WSPACE)
			(*tmp) = (*tmp)->next;
		if (creat_inp_out(tmp, tmp_cmd))
			break ;
		creat_append(tmp, tmp_cmd);
		if (creat_heredoc(cmds, tmp, y, *tmp_cmd))
			return (1);
		creat_word_cmd(*tmp_cmd, tmp, y);
	}
	if ((*tmp))
	{
		(*tmp) = (*tmp)->next;
		(*tmp_cmd)->next = (t_cmd *)malloc(sizeof(t_cmd));
		(*tmp_cmd) = (*tmp_cmd)->next;
		(*tmp_cmd)->cmd = NULL;
		(*tmp_cmd)->fd_input = 0;
		(*tmp_cmd)->env = (*cmds)->env;
		(*tmp_cmd)->fd_out = 1;
		(*tmp_cmd)->next = NULL;
	}
	return (0);
}

t_cmd	*creat_cmd(t_shell *data)
{
	t_shell	*tmp;
	t_stk	y;
	t_cmd	*cmds;
	t_cmd	*tmp_cmd;

	tmp = data;
	cmds = (t_cmd *)malloc(sizeof(t_cmd));
	cmds->fd_input = 0;
	cmds->env = data->env;
	cmds->cmd = NULL;
	cmds->fd_out = 1;
	cmds->next = NULL;
	tmp_cmd = cmds;
	y.i = 0;
	while (tmp)
	{
		if (creat_cmd_1(&cmds, &tmp, &y, &tmp_cmd))
			return (NULL);
	}
	return (cmds);
}
