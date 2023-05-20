/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_cmd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mserrouk <mserrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:15:12 by mserrouk          #+#    #+#             */
/*   Updated: 2023/05/20 22:12:25 by mserrouk         ###   ########.fr       */
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

void	envs_tab_norm(t_env	*tmp, t_cmd *cmd, int i, char **tab)
{
	int	j;

	while (tmp)
	{
		if (!ft_strncmp(tmp->env, "SHLVL", 5))
		{
			j = 0;
			while (cmd->cmd && cmd->cmd[j])
			{
				if (word_stop("./minishell", cmd->cmd[j++]))
				{
					if (tmp->p == 1)
						tab[i++] = ft_strjoin(ft_strdup("SHLVL="),
								ft_itoa(ft_atoi(tmp->env + 6) + 1));
					else
						tab[i++] = ft_strjoin(ft_strdup("SHLVL="), ft_itoa(2));
					break ;
				}
			}
		}
		else
			tab[i++] = ft_strdup(tmp->env);
			tmp = tmp->next;
	}
	tab[i] = NULL;
}

char	**envs_tab(t_env *envs, t_cmd *cmd)
{
	char	**tab;
	t_env	*tmp;
	int		i;

	i = 0;
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
	envs_tab_norm(tmp, cmd, i, tab);
	return (tab);
}

void	ft_command_norm(t_cmd *cmd)
{
	char	**tab;

	if (word_stop(cmd->cmd[0], "pwd"))
		cmd_pwd(cmd);
	else if (word_stop (cmd->cmd[0], "export"))
		cmd_export_fork(*cmd->env);
	else if (word_stop (cmd->cmd[0], "env") && cmd->cmd[1] == NULL)
		cmd_env(*cmd->env);
	ft_command_path(cmd, cmd->paths);
	tab = envs_tab(*cmd->env, cmd);
	execve(cmd->cmd_path, cmd->cmd, tab);
	perror("Minishell");
	exit(126);
}

void	ft_command(t_cmd *cmd, t_cmd *tmp2, int i, t_stk y)
{	
	if (y.tmp1->fd_input == -1)
		error_message("Minishell$: No such file or directory\n", 1);
	if (cmd->fd_out == 1 && cmd->next)
		cmd->fd_out = cmd->fd[1];
	if (cmd->fd_input == 0 && i != 0)
		cmd->fd_input = tmp2->fd[0];
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
	else
		ft_command_norm(cmd);
}
