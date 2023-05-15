/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_unset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mserrouk <mserrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:28:54 by mserrouk          #+#    #+#             */
/*   Updated: 2023/05/14 22:39:41 by mserrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	hide_unset(t_env *tmp, char *cmd)
{
	if (!ft_strncmp(cmd, "HOME", 4) && !ft_strncmp (tmp->env, "HOME", 4))
	{
		tmp->p = 0;
		return (1);
	}
	else if (!ft_strncmp(cmd, "PWD", 3) && !ft_strncmp(tmp->env, "PWD", 3))
	{
		tmp->p = 0;
		return (1);
	}
	else if (!ft_strncmp(cmd, "OLDPWD", 6)
		&& !ft_strncmp(tmp->env, "OLDPWD", 6))
	{
		tmp->p = 0;
		return (1);
	}
	return (0);
}

void	cmd_unset_norm(t_env *tmp, int i, int len, t_cmd *cmd)
{
	t_env	*tmp1;

	while (tmp)
	{
		tmp1 = tmp;
		tmp = tmp->next;
		if (tmp && !hide_unset(tmp, cmd->cmd[i])
			&& ft_strnstr(tmp->env, cmd->cmd[i], len) && (tmp->env[len] == '='
				|| tmp->env[len] == '\0') && tmp->env[0] != '?')
		{
			tmp1->next = tmp->next;
			free(tmp->env);
			free(tmp);
			break ;
		}
	}
}

void	cmd_unset(t_env **envs, t_cmd *cmd)
{
	t_env	*tmp;
	t_env	*tmp1;
	int		i;
	int		len;

	i = 1;
	len = 0;
	while (cmd && cmd->cmd && cmd->cmd[i])
	{
		tmp = *envs;
		tmp1 = *envs;
		len = ft_strlen(cmd->cmd[i]);
		if (tmp && !hide_unset(tmp, cmd->cmd[i])
			&& ft_strnstr(tmp->env, cmd->cmd[i], len)
			&& (tmp->env[len] == '=' || tmp->env[len] == '\0')
			&& tmp->env[0] != '?')
		{
			*envs = (*envs)->next;
			free(tmp->env);
			free(tmp);
			tmp = NULL;
		}
		cmd_unset_norm(tmp, i, len, cmd);
		i++;
	}
}
