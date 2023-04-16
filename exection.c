/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exection.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 23:03:21 by mserrouk          #+#    #+#             */
/*   Updated: 2023/04/16 20:20:01 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


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


void	ft_command(t_cmd *cmd)
{
	dup2(cmd->fd_input , 0);
	dup2(cmd->fd_out , 1);

	// if (cmd->fd_input != 0)
	// 	close(cmd->fd_input);
	// if (cmd->fd_input != 1)
	// 	close(cmd->fd_out);
	execve(cmd->path, cmd->cmd, NULL);
	error_message("Error execve");
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
			printf("%s\n", cmd->cmd[j]);
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

void ft_pipe(t_cmd *cmd , char **path)
{
	t_cmd *tmp;
	t_shell *tmp2;
	int i;

	i = 0;
	tmp = cmd;
	
	tmp2 = cmd->data;
	while(tmp)
	{
		tmp->pid = fork();
		if(tmp->pid == 0)
		{
			// exit (1);
			ft_command_path(tmp ,path);	
			creat_files(&tmp2, cmd);
			ft_command(tmp);
		}
		tmp = tmp->next;
		i++;
	}
	i = 1;
	while(i > 0)
	{
		wait(NULL);
		i--;
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






void    exection(t_shell *data , t_env *enve)
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
    tmp = data;
	// while (tmp)
	// {
	// 	printf("%s\n", tmp->s);
	// 	tmp = tmp->next;
	// }
	// tmp = data;
	// while (tmp)
	// {
	// 	printf("%s\n", tmp->s);
	// 	tmp = tmp->next;
	// }
    while (tmp && tmp->type == 'S')
    tmp = tmp->next;
	
    cmd = (t_cmd *) malloc(sizeof(t_cmd));
	cmd->next = NULL;
	cmd->head = cmd;
	cmd->data = data;
	tmpc = cmd;
	y.i = 0;
	y.ss = NULL;
	tmp2 = data;
    while (tmp2)
    {
		while (tmp && tmp->type != PIPE)
		{
			if (tmp->type == SPACE)
				tmp = tmp->next;
			while (tmp && (tmp->type == WORD || tmp->type  == DOUBLE || tmp->type == SINGLE || tmp->type == SPACE))
			{
				if (tmp->type == SPACE)
					tmp = tmp->next;
				else
				{
					y.i++;
					tmp = tmp->next;
				}
			}
			cmd->cmd = (char **)malloc(sizeof(char *) * (i + 1));
			cmd->cmd[i] = NULL;
			i = 0;
			while (i <= y.i)
			{
				cmd->cmd[i] = tmp2->s;
				// printf("%s\n", tmp2->s);
				tmp2 = tmp2->next;
				i++;
			}
		}
		if (tmp2)
		{
			tmp2 = tmp2->next;
			tmpc->next = malloc(sizeof(t_cmd));
			tmpc = tmpc ->next;
			tmpc->head = cmd;
			tmpc->next = NULL;
		}
		
    }
	// ft_pipe(cmd , path);
}
