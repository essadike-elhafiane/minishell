/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 20:18:01 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/13 23:54:21 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_double(char **spl)
{
	int	l;

	l = 0;
	while (spl[l])
		free(spl[l++]);
	free(spl);
}

void	clean_cmd_help(t_cmd *cmd, t_stk	*y, char **spl, int flg)
{
	if (flg == 2)
	{
		spl = ft_split(cmd->cmd[y->i], 39);
		while (spl[y->b])
			y->ss = ft_strjoin(y->ss, spl[y->b++]);
		free(cmd->cmd[y->i]);
		cmd->cmd[y->i] = y->ss;
	}
	else if (flg == 3)
	{
		spl = ft_split(cmd->cmd[y->i], 39);
		while (spl[y->b])
			y->ss = ft_strjoin_no_free(y->ss, spl[y->b++]);
		free(cmd->cmd[y->i]);
		cmd->cmd[y->i] = y->ss;
		y->b = 0;
		spl = ft_split(cmd->cmd[y->i], '\"');
		free(y->ss);
		y->ss = NULL;
		while (spl[y->b])
			y->ss = ft_strjoin_no_free(y->ss, spl[y->b++]);
		free(cmd->cmd[y->i]);
		cmd->cmd[y->i] = y->ss;
	}
}
