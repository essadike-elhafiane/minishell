/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exection_c.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 23:03:21 by mserrouk          #+#    #+#             */
/*   Updated: 2023/04/18 02:27:14 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>

void word(t_shell *tmp,t_cmd *cmd)
{
	int i;
	t_shell *tmp2;

	i = 0;
	tmp2 = tmp;
	
	while (tmp && tmp->type != 'PIPE')
	// tmp2 = (*tmp);
	// while(tmp && *tmp && ((*tmp)->type == 'W' || (*tmp)->type == 'D' || (*tmp)->type == 'N'))
	// {
	// 	// printf("fs\n");
	// 		i++;
	// 		(*tmp) = (*tmp)->next;
	// 	while (tmp && *tmp && (*tmp)->type == 'S')
    // 		(*tmp) = (*tmp)->next;
	// }
	// cmd->cmd =(char **) malloc(i + 1);
	// i = 0;
	// while(tmp2 && (tmp2->type == 'W' || tmp2->type == 'D' || tmp2->type == 'N'))
	// {
	// 	// printf("fs;;\n");

	// 		cmd->cmd[i]= tmp2->s;
	// 		i++;
	// 		tmp2 = tmp2->next;
	// 	while (tmp2 && tmp2->type == 'S')
    // 		tmp2 = tmp2->next;
	// }
	cmd->cmd[i] = NULL;
}



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
			tmp = ft_strjoin1(path[i], "/");
			cmd->path = ft_strjoin1(tmp, cmd->cmd[j]);
			free(tmp);
			if (!access(cmd->path, X_OK))
				return ;
			free(cmd->path);
			cmd->path = NULL;
			i++;
		}
		j++;
	}
	error_message("Error command not found\n");
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
	if(ft_strncmp (cmd->cmd[0], "pwd", 5))
		cmd_pwd(cmd);
	else
		execve(cmd->path, cmd->cmd, NULL);
	error_message("Error execve");
}

void creat_files(t_cmd *cmd, t_cmd *tmp2, int i)
{
	cmd->fd_input = 0;
	cmd->fd_out = 1;
	if(cmd->fd_out == 1 && cmd->next)
		cmd->fd_out = cmd->fd[1];
	if(cmd->fd_input == 0 && i != 0)
		cmd->fd_input = tmp2->fd[0];
}

void ft_pipe(t_cmd *cmd , char **path)
{
	t_cmd	*tmp;
	int i;
	t_cmd *tmp2;
	tmp2 = NULL;
	cmd->i = 0;
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
			ft_command_path(tmp ,path);
			creat_files(tmp ,tmp2,cmd->i);
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
		cmd->i +=1;
	}
	while (cmd->i > 0)
	{
		wait(NULL);
		cmd->i -= 1;
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

void    exection(t_shell *data , t_env *enve ,char **env)
{
    t_shell *tmp;
    t_shell *tmp2;
    t_cmd   *cmd;
	t_stk  y;
	t_cmd	*tmpc;
	char **path;
    int i;

	

	path = ft_all_paths(enve);
	if(!path)
		return;
	cmd = (t_cmd *) malloc(sizeof(t_cmd));
	cmd->next = NULL;
	cmd->head = cmd;
	cmd->data = data;
	tmpc = cmd;
	cmd->cmd = (char **)malloc((2 + 1) * sizeof(char *));
	cmd->cmd[0] = (char *)malloc(ft_strlen("ls") + 1);
	cmd->cmd[0] = ft_memcpy(cmd->cmd[0] , "ls", ft_strlen("ls"));
	cmd->cmd[1] = (char *)malloc(ft_strlen("-la") + 1);
	cmd->cmd[1] = ft_memcpy(cmd->cmd[1] , "-la", ft_strlen("-la"));
	cmd->cmd[2] = NULL;
	cmd->enve = env;

	cmd->next = (t_cmd *) malloc(sizeof(t_cmd));
	cmd->next->data = data;
	cmd->next->cmd = (char **)malloc((2 + 1) * sizeof(char *));
	cmd->next->cmd[0] = ft_strdup("cd");
	cmd->next->cmd[1] = ft_strdup("libft");
	cmd->next->cmd[2] = NULL;
	cmd->next->enve = env;
	cmd->next->next = NULL;
	
	
	cmd->next->next = (t_cmd *) malloc(sizeof(t_cmd));
	cmd->next->next->data = data;
	cmd->next->next->cmd = (char **)malloc((2 ) * sizeof(char *));
	cmd->next->next->cmd[0] = ft_strdup("pwd");
	cmd->next->next->cmd[1] = NULL;
	cmd->next->next->enve = env;
	// cmd->next->next->cmd[2] = ft_strdup("");
	cmd->next->next->next = NULL;
	
	ft_pipe(cmd , path);
}
