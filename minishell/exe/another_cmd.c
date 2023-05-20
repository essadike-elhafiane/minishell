/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   another_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mserrouk <mserrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:37:20 by mserrouk          #+#    #+#             */
/*   Updated: 2023/05/19 17:30:43 by mserrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	cmd_pwd(t_cmd *cmd)
{
	int		i;
	char	cd[1024];

	i = 0;
	while (*cmd->env)
	{
		if (ft_strnstr((*cmd->env)->env, "PWD=", 4))
		{
			i = 1;
			printf("%s\n", (*cmd->env)->env + 4);
			break ;
		}
		*cmd->env = (*cmd->env)->next;
	}
	if (i == 0)
		printf("%s\n", getcwd(cd, 1024));
	exit(0);
}

void	cmd_env(t_env *envs)
{
	t_env	*tmp;

	tmp = envs;
	while (tmp)
	{
		if (ft_strchr(tmp->env, '=') && tmp->env[0] != '?' && tmp->p != 0)
			printf("%s\n", tmp->env);
		tmp = tmp->next;
	}
	exit(0);
}

void	cmd_export(t_env **envs, char *cmd)
{
	t_env		*tmp;
	t_env		*tmp2;

	if (!envs)
		return ;
	tmp = *envs;
	tmp2 = *envs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = (t_env *) malloc(sizeof(t_env));
	tmp = tmp->next;
	tmp->p = 1;
	tmp->next = NULL;
	tmp->env = ft_strdup(cmd);
}
