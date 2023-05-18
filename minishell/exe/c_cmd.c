/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_cmd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mserrouk <mserrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:15:12 by mserrouk          #+#    #+#             */
/*   Updated: 2023/05/15 19:55:57 by mserrouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_command_path(t_cmd *cmd, char **path)
{
	int		i;
	int		j;
	char	*tmp;

	j = 0;
	while (cmd->cmd[j])
	{
		i = 0;
		if (!ft_command_path_norm(cmd, j, path))
			return ;
		while (path && path[i])
		{
			tmp = ft_strjoin_no_free(path[i], "/");
			cmd->cmd_path = ft_strjoin_no_free(tmp, cmd->cmd[j]);
			free(tmp);
			if (!access(cmd->cmd_path, X_OK))
				return ;
			free(cmd->cmd_path);
			cmd->cmd_path = NULL;
			i++;
		}
		j++;
	}
	error_message("➜ Minishell$: command not found\n", 127);
}

char	**envs_tab(t_env *envs)
{
	char	**tab;
	t_env	*tmp;
	int		i;

	i = 1;
	tmp = envs;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	tab = (char **)malloc(sizeof(char *) * i);
	if (!tab)
		error_message("error\n", 2);
	tmp = envs;
	i = 0;
	while (tmp)
	{
		tab[i++] = ft_strdup(tmp->env);
		tmp = tmp->next;
	}
	tab[i] = NULL;
	return (tab);
}

void	creat_files(t_cmd *cmd, t_cmd *tmp2, int i)
{
	if (cmd->fd_out == 1 && cmd->next)
		cmd->fd_out = cmd->fd[1];
	if (cmd->fd_input == 0 && i != 0)
		cmd->fd_input = tmp2->fd[0];
}

void	ft_command_norm(t_cmd *cmd)
{
	char	**tab;

	ft_command_path(cmd, cmd->paths);
	tab = envs_tab(*cmd->env);
	execve(cmd->cmd_path, cmd->cmd, tab);
	perror("Minishell");
	exit(126);
}

void	ft_command(t_cmd *cmd, t_cmd *tmp2, int i)
{	
	creat_files(cmd, tmp2, i);
	if (cmd->fd_input != 0)
		dup2(cmd->fd_input, STDIN_FILENO);
	if (cmd->fd_out != 1)
		dup2(cmd->fd_out, STDOUT_FILENO);
	if (cmd->next)
		close(cmd->fd[1]);
	if (cmd->fd_input != 0)
		close(cmd->fd_input);
	if (cmd->fd_out != 1)
		close(cmd->fd_out);
	if (word_stop(cmd->cmd[0], "echo"))
		cmd_echo(cmd);
	else if (word_stop(cmd->cmd[0], "pwd"))
		cmd_pwd(cmd);
	else if (word_stop (cmd->cmd[0], "export"))
		cmd_export_fork(*cmd->env);
	else if (word_stop (cmd->cmd[0], "env"))
		cmd_env(*cmd->env);
	else
		ft_command_norm(cmd);
}
