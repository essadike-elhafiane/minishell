/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mserrouk <mserrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:05:40 by mserrouk          #+#    #+#             */
/*   Updated: 2023/05/19 17:03:39 by mserrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	cmd_cd_norm1(t_stk *y, t_cmd *cmd, t_env *tmp)
{
	while (tmp)
	{
		if (ft_strnstr(tmp->env, "PWD", 3)
			&& (tmp->env[3] == '=' || tmp->env[3] == '\0'))
			y->ss = tmp->env + 4;
		if (ft_strnstr(tmp->env, "HOME", 4)
			&& (tmp->env[4] == '=' || tmp->env[4] == '\0'))
			y->back = tmp->env + 5;
		tmp = tmp->next;
	}
	if (cmd->cmd[1] == NULL || cmd->cmd[1][0] == '\0')
	{
		if (tmp == NULL)
			return (g_status = 1,
				ft_putstr_fd("minishell: cd: HOME not set\n", 2), 0);
		chdir(y->back);
		g_status = 0;
	}
	else
	{
		if (chdir(cmd->cmd[1]))
			return (g_status = 1, perror("Minishell"), 0);
	}
	return (1);
}

void	cmd_cd_norm2(t_stk *y, int *flg, t_cmd *cmd)
{
	ft_putstr_fd("cd: error retrieving current directory: getcwd: \
			cannot access parent directories: No such file or directory\n", 2);
	g_status = 0;
	y->front = ft_strjoin(ft_strdup("OLDPWD="), ft_strdup(y->ss));
	y->ss = ft_strjoin(ft_strdup("PWD="), ft_strdup(y->ss));
	y->ss = ft_strjoin(y->ss, ft_strdup("/"));
	y->ss = ft_strjoin(y->ss, ft_strdup(cmd->cmd[1]));
	flg++;
}

void	cmd_cd_norm3(t_stk *y, t_env *tmp, int flg)
{
	while (tmp)
	{
		if (ft_strnstr(tmp->env, "PWD", 3))
		{
			free(tmp->env);
			tmp->env = y->ss;
		}
		if (ft_strnstr(tmp->env, "OLDPWD=", 8) && y->front != NULL)
		{
			if (!flg)
			{
				free(tmp->env);
				tmp->env = y->front;
			}
			else
				free(y->front);
		}
		tmp = tmp->next;
	}
}

void	cmd_cd(t_cmd *cmd)
{
	t_stk		y;
	char		cwd[1024];
	t_env		*tmp;
	static int	flg;

	tmp = *cmd->env;
	y.ss = NULL;
	y.back = NULL;
	if (!cmd_cd_norm1(&y, cmd, tmp))
		return ;
	if (!getcwd(cwd, sizeof(cwd)))
		cmd_cd_norm2(&y, &flg, cmd);
	else
	{
		y.front = ft_strjoin(ft_strdup("OLDPWD="), ft_strdup(y.ss));
		y.ss = ft_strjoin_no_free("PWD=", getcwd(cwd, sizeof(cwd)));
		flg = 0;
		g_status = 0;
	}
	cmd_cd_norm3(&y, tmp, flg);
	return ;
}
