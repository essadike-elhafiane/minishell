/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mserrouk <mserrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:26:19 by mserrouk          #+#    #+#             */
/*   Updated: 2023/05/15 19:54:56 by mserrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	cmd_echo_norm(t_cmd *cmd, t_stk	*y, int	*flg_n)
{
	while (cmd->cmd[y->i])
	{
		y->j = 0;
		while (cmd->cmd[y->i][y->j] && cmd->cmd[y->i][0] == '-')
		{
			y->j++;
			while (cmd->cmd[y->i][y->j] == 'n' && cmd->cmd[y->i][y->j])
				y->j++;
			if (!cmd->cmd[y->i][y->j])
			{	
				*flg_n = 1;
				y->i++;
			}
			else
				break ;
			y->j = 0;
		}
		while (cmd->cmd[y->i])
		{
			ft_putstr_fd(cmd->cmd[y->i], 1);
			y->i++;
			if (cmd->cmd[y->i])
				write(1, " ", 1);
		}
	}
}

void	cmd_echo(t_cmd *cmd)
{
	int		flg_n;
	t_stk	y;

	y.i = 1;
	flg_n = 0;
	if (word_stop("echo", cmd->cmd[0]))
	{
		flg_n = 0;
		cmd_echo_norm(cmd, &y, &flg_n);
	}
	if (!flg_n)
		printf("\n");
	cmd = cmd->next;
	g_status = 0;
	exit(0);
}
