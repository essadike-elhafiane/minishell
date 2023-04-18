/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exection.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 23:03:21 by mserrouk          #+#    #+#             */
/*   Updated: 2023/04/18 02:30:24 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void	error_message(char *str)
{
	write(2, str, ft_strlen(str));
	exit(1);
}


void	ft_command_path(t_cmd *cmd , char **path)
{
	int	i;
	int j;
	char *tmp;
	j = 0;
	while(cmd->cmd[j])
	{
		i = 0;
		while (path[i])
		{
			tmp = ft_strjoin_exe(path[i], "/");
			cmd->cmd_path = ft_strjoin_exe(tmp, cmd->cmd[j]);
			free(tmp);
			if (!access(cmd->cmd_path, X_OK))
				return ;
			free(cmd->cmd_path);
			cmd->cmd_path = NULL;
			i++;
		}
		j++;
	}
	error_message("Error command not found\n");
}

char	**get_path(t_env *env)
{
	int	i;

	i = -1;
	while (env)
	{
		if (ft_strnstr(env->env, "PATH", 6))
			return (ft_split(env->env +5, ':'));
		env = env->next;
	}
	return (NULL);
}
// void cmd_pwd(t_cmd *cmd)
// {
// 	int i;

// 	i = 0;
// 	while(cmd->enve[i])
// 	{
// 		if(ft_strncmp(cmd->enve[i++], "pwd" ,5))
// 			printf("%s", cmd->enve[i] + 4);
// 	}	
// }


void	ft_command(t_cmd *cmd )
{	
	if(cmd->fd_input != 0)
		dup2(cmd->fd_input , STDIN_FILENO);
	if(cmd->fd_out != 1)	
		dup2(cmd->fd_out , STDOUT_FILENO);
	if (cmd->next)
		close(cmd->fd[1]);
	// if(ft_strncmp (cmd->cmd[0], "pwd", 5))
	// 	cmd_pwd(cmd);
	// else
		execve(cmd->cmd_path, cmd->cmd, NULL);
	error_message("Error execve");
}

void creat_files(t_cmd *cmd, t_cmd *tmp2, int i)
{	
	if(cmd->fd_out == 1 && cmd->next)
		cmd->fd_out = cmd->fd[1];
	if(cmd->fd_input == 0 && i != 0)
		cmd->fd_input = tmp2->fd[0];
}

void ft_pipe(t_cmd *cmd)
{
	t_cmd	*tmp;
	t_stk	y;

	t_cmd *tmp2;
	tmp2 = NULL;
	y.i = 0;
	tmp = cmd;
	while (tmp)
	{
		if (tmp->next)
			pipe(tmp->fd);
		tmp->pid = fork();
		if (tmp->pid == 0)
		{
			if (tmp->next)
				close(tmp->fd[0]);
			ft_command_path(tmp ,tmp->paths);
			creat_files(tmp ,tmp2, y.i);
			ft_command(tmp);
		}
		else
		{
			if (tmp->next)
				close(tmp->fd[1]);
			if (tmp2)
				close(tmp2->fd[0]);
		}
		tmp2 = tmp;
		tmp = tmp->next;		
		y.i +=1;
	}
	while (y.i > 0)
	{
		wait(NULL);
		y.i -= 1;
	}
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

void	exection(t_cmd *cmd)
{
	t_cmd *tmp;

	cmd->paths = get_path(cmd->env);
	tmp = cmd;
	if (cmd->paths == NULL)
		printf("hhhhh\n");
	while (tmp)
	{
		// if(tmp->cmd == ' ')
		// 	tmp = tmp->next;
		tmp->paths = cmd->paths;
		tmp = tmp->next;
	}
	ft_pipe(cmd);
}
