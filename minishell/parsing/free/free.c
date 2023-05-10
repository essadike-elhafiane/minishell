/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 22:48:31 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/10 02:42:04 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	fun_free(t_shell **a)
{
	t_shell	*tmp;

	while (*a)
	{
		tmp = *a;
		*a = (*a)->next;
		free(tmp->s);
		free(tmp);
	}
	a = NULL;
}

void	fun_free_env(t_env **a)
{
	t_env	*tmp;
	while (*a)
	{
		tmp = *a;
		*a = (*a)->next;
		free(tmp->env);
		free(tmp);
	}
	a = NULL;
}

void	fun_free_cmd(t_cmd **a)
{
	t_cmd	*tmp;
	int		i;

	if (!a || !*a)
		return ;
	while (*a)
	{
		i = 0;
		tmp = *a;
		*a = (*a)->next;
		if (tmp->cmd && tmp->cmd[0])
		{
			while (tmp->cmd[i])
				free(tmp->cmd[i++]);
			free(tmp->cmd);
			tmp->cmd = NULL;
		}
		free(tmp);
		// if (tmp->paths)
		// {
		// 	i = 0;
		// 	while(tmp->paths[i])
		// 		free(tmp->paths[i++]);
		// }
		// free(tmp);
	}
	a = NULL;
}

int	check_douple_single(char *cmd)
{
	int	flg;
	int	i;

	flg = 0;
	i = 0;
	while (cmd[i])
	{
		if (flg == 0 && cmd[i] == '\"')
			flg = 1;
		if (flg == 0 && cmd[i] == 39)
			flg = 2;
		if (flg == 1 && cmd[i] == 39)
			flg = 3;
		if (flg == 2 && cmd [i] == '\"')
			flg = 3;
		i++;
	}
	return (flg);
}

void	clean_cmd(t_cmd *cmd)
{
	t_stk	y;
	char	**spl;
	int		flg;

	flg = 0;
	y.ss = NULL;
	while (cmd)
	{
		y.i = -1;
		while (cmd->cmd[++y.i])
		{
			y.b = 0;
			flg = check_douple_single(cmd->cmd[y.i]);
			if (flg == 1)
			{
				spl = ft_split(cmd->cmd[y.i], '\"');
				while (spl[y.b])
					y.ss = ft_strjoin(y.ss, spl[y.b++]);
				free(cmd->cmd[y.i]);
				cmd->cmd[y.i] = y.ss;
			}
			clean_cmd_help(cmd, &y, spl, flg);
		}
		cmd = cmd->next;
	}
}
