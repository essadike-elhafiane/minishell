/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 22:48:31 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/18 17:34:14 by eelhafia         ###   ########.fr       */
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

void    fun_free_cmd(t_cmd **a)
{
	t_cmd	*tmp;
	int		i;

	while (*a)
	{
		i = 0;
		tmp = *a;
		*a = (*a)->next;
		if (tmp->cmd)
		{
			while(tmp->cmd[i])
				free(tmp->cmd[i++]);
		}
		if (tmp->paths)
		{
			while(tmp->paths[i])
				free(tmp->paths[i++]);
		}
		free(tmp->cmd);
		free(tmp);
	}
	a = NULL;
}

int	check_douple_single(char *cmd)
{
	int flg;
	int	i;

	flg = 0;
	i = 0;
	while(cmd[i])
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
	t_stk y;
	char **spl;
	int	flg;

	flg = 0;
	y.ss = NULL;
	while(cmd)
	{
		y.i = 0;
		while(cmd->cmd[y.i])
		{
			y.b = 0;
			flg = check_douple_single(cmd->cmd[y.i]);
			// printf("%d\n", flg);
			// exit(1);
			if (flg == 1)
			{
				spl = ft_split(cmd->cmd[y.i], '\"');
				while (spl[y.b])
					y.ss = ft_strjoin(y.ss, spl[y.b++]);
				free(cmd->cmd[y.i]);
				cmd->cmd[y.i] = y.ss;
			}
			else if (flg == 2)
			{
				spl = ft_split(cmd->cmd[y.i], 39);
				while (spl[y.b])
					y.ss = ft_strjoin(y.ss, spl[y.b++]);
				free(cmd->cmd[y.i]);
				cmd->cmd[y.i] = y.ss;
			}
			else if (flg == 3)
			{
				spl = ft_split(cmd->cmd[y.i], 39);
				while (spl[y.b])
					y.ss = ft_strjoin_no_free(y.ss, spl[y.b++]);
				free(cmd->cmd[y.i]);
				cmd->cmd[y.i] = y.ss;
				y.b = 0;
				spl = ft_split(cmd->cmd[y.i], '\"');
				free(y.ss);
				y.ss = NULL;
				while (spl[y.b])
					y.ss = ft_strjoin_no_free(y.ss, spl[y.b++]);
				free(cmd->cmd[y.i]);
				cmd->cmd[y.i] = y.ss;
			}
			// printf("%s\n", cmd->cmd[y.i]);
			y.i++;
		}
		cmd = cmd->next;
	}
}
