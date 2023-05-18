/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exection.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mserrouk <mserrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 23:03:21 by mserrouk          #+#    #+#             */
/*   Updated: 2023/05/15 19:58:47 by mserrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_command_path_norm(t_cmd *cmd, int j, char **path)
{
	if (!path)
	{
		ft_putstr_fd("Minishell: ", 2);
		error_message(": No such file or directory\n", 127);
	}
	if (ft_strchr(cmd->cmd[j], '/'))
	{
		if (!access(cmd->cmd[j], X_OK))
		{
			cmd->cmd_path = ft_strdup (cmd->cmd[j]);
			return (0);
		}
		if (errno == EACCES)
		{
			perror("Minishell");
			exit(126);
		}
		else if (errno == ENOENT)
		{
			perror("Minishell");
			exit(127);
		}
	}
	return (1);
}

char	**get_path(t_env *env)
{
	int	i;

	i = -1;
	while (env)
	{
		if (ft_strnstr(env->env, "PATH", 6) && env->p == 1)
			return (ft_split(env->env +5, ':'));
		env = env->next;
	}
	return (NULL);
}

char	**ft_all_paths(t_env *envp)
{
	while (envp)
	{
		if (ft_strnstr(envp->env, "PATH", 4))
			return (ft_split(envp->env + 5, ':'));
		envp = envp->next;
	}
	return (NULL);
}

void	exection(t_cmd	*cmd)
{
	t_cmd	*tmp;
	int		i;

	cmd->paths = get_path(*cmd->env);
	tmp = cmd;
	while (tmp)
	{
		tmp->paths = cmd->paths;
		tmp = tmp->next;
	}
	if (cmd->cmd && !cmd->next && word_stop("cd", cmd->cmd[0]))
		cmd_cd(cmd);
	else
		ft_pipe(cmd);
	i = 0;
	while (cmd->paths && cmd->paths[i])
	{
		free(cmd->paths[i]);
		i++;
	}
	free(cmd->paths);
}
