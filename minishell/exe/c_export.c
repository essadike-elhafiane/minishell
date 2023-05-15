/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mserrouk <mserrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:30:42 by mserrouk          #+#    #+#             */
/*   Updated: 2023/05/15 21:21:05 by mserrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_export_norm(t_cmd **cmd, t_stk *y)
{
	if (y->i < 0 && y->plus)
			y->b = -1;
	if (!(*cmd)->cmd[y->j + 1] && (*cmd)->cmd[y->j][0] == '\0')
		return (1);
	while ((*cmd)->cmd[y->j]
		&& (*cmd)->cmd[y->j][y->b] && (*cmd)->cmd[y->j][y->b] != '=')
	{
		if ((*cmd)->cmd[y->j][y->b] == '+'
			&& (*cmd)->cmd[y->j][y->b + 1] == '=' && y->b > 0)
			y->b++;
		else if (ft_isdigit((*cmd)->cmd[y->j][0])
			|| (!ft_isalpha((*cmd)->cmd[y->j][y->b])
			&& !ft_isdigit((*cmd)->cmd[y->j][y->b])
			&& (*cmd)->cmd[y->j][y->b] != '_' ))
		{
			ft_putstr_fd("minishell: not a valid identifier\n", 2);
			g_status = 1;
			y->b = -1;
			break ;
		}
		else
			y->b++;
	}
	return (0);
}

int	check_export_norm_1(t_env *tmp, t_cmd *cmd, t_stk *y)
{
	if (y->i > 0 && ft_strnstr(tmp->env, y->ss, ft_strlen(y->ss))
		&& y->i != y->plus)
	{
		free(tmp->env);
		tmp->p = 1;
		tmp->env = ft_strdup(cmd->cmd[y->j]);
		y->flg = 1;
	}
	else if (y->i > 0 && ft_strnstr(tmp->env, y->ss, ft_strlen(y->ss))
		&& y->i == y->plus)
	{
		if (ft_strcchr(tmp->env, '=') < 0)
			tmp->env = ft_strjoin(tmp->env, ft_strdup("="));
		tmp->p = 1;
		tmp->env = ft_strjoin(tmp->env, ft_strdup(cmd->cmd[y->j] + y->i + 2));
		y->flg = 1;
	}
	else if (ft_strnstr(tmp->env, cmd->cmd[y->j], ft_strlen(cmd->cmd[y->j])))
	{
		if (tmp->env[ft_strlen(y->ss)] != '\0'
			&& tmp->env[ft_strlen(y->ss)] != '=')
			return (1);
		y->flg = 1;
	}
	return (0);
}

void	check_export_norm_2(t_env **tmp, t_cmd **cmd, t_stk *y)
{
	while ((*tmp) && (*cmd)->cmd[y->j] && y->b > -1)
	{
		if (check_export_norm_1((*tmp), *cmd, y))
			break ;
		(*tmp) = (*tmp)->next;
	}
	if ((*cmd)->cmd[y->j] && y->i == y->plus && y->ss && y->i > 0 && !y->flg)
	{
		free((*cmd)->cmd[y->j]);
		(*cmd)->cmd[y->j] = ft_strjoin(ft_strdup(y->ss),
				ft_strdup((*cmd)->cmd[y->j] + y->i + 1));
	}
	free(y->ss);
	if (!y->flg && y->b > -1)
		cmd_export((*cmd)->env, (*cmd)->cmd[y->j]);
	y->j++;
}

int	check_export(t_env **envs, t_cmd *cmd)
{
	t_env	*tmp;
	t_stk	y;

	y.j = 1;
	g_status = 0;
	while (cmd && cmd->cmd[y.j])
	{
		y.flg = 0;
		y.i = -1;
		y.b = 0;
		y.plus = 0;
		tmp = *envs;
		y.i = ft_strcchr(cmd->cmd[y.j], '=');
		if (y.i > 0)
		{
			y.plus = ft_strcchr(cmd->cmd[y.j], '+');
			if (y.plus + 1 == y.i)
				y.i = y.plus;
		}
		y.ss = ft_substr(cmd->cmd[y.j], 0, y.i);
		if (check_export_norm(&cmd, &y))
			return (1);
		check_export_norm_2(&tmp, &cmd, &y);
	}
	return (0);
}

void	cmd_export_fork(t_env *tmp)
{
	int	i;
	int	flg;

	while (tmp)
	{
		i = -1;
		flg = 0;
		if (tmp->env[0] != '?' && tmp->p == 1)
			write(1, "declare -x ", 11);
		while (tmp->env[++i])
		{
			if (tmp->env[0] == '?' || tmp->p == 0)
				break ;
			write(1, &tmp->env[i], 1);
			if (tmp->env[i] == '=' && !flg++)
				write(1, "\"", 1);
		}
		if (flg && i != 0)
			write(1, "\"\n", 2);
		else if (i != 0)
			write(1, "\n", 1);
		tmp = tmp->next;
	}
	exit(0);
}
