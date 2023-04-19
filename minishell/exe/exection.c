/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exection.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mserrouk <mserrouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 23:03:21 by mserrouk          #+#    #+#             */
/*   Updated: 2023/04/19 02:25:20 by mserrouk         ###   ########.fr       */
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
		if (ft_strchr(cmd->cmd[j] ,'/') && !access(cmd->cmd[j], X_OK))
		{
			cmd->cmd_path = cmd->cmd[j];
				return;
		}
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
void	cmd_echo(t_cmd *cmd)
{
	int		flg_n;

	t_stk	y;
	// t_stk1	y;
	y.i = 1;
	flg_n = 0;
	if (word_stop("echo", cmd->cmd[0]))
	{
		y.i = 1;
		// if (cmd->fd_out != 1)
		// {
		// 	sd = dup(1);
		// 	dup2(cmd->fd_out, 1);
		// }
		while (cmd->cmd[y.i])
		{
			y.j = 0;
			if (word_stop("-n", cmd->cmd[y.i]))
			{
				flg_n = 1;
				y.i++;
			}
			while (cmd->cmd[y.i][y.j])
			{
				write(cmd->fd_out, &cmd->cmd[y.i][y.j], 1);
				y.j++;
			}
			y.i++;
		}
			// if (cmd->cmd[y.i])
			// 	write(1, " ", 1);
		}
		if (!flg_n)
			printf("\n");
		// if (cmd->fd_out != 1)
		// 	dup2(1, cmd->fd_out);
		cmd = cmd->next;
		// if (!cmd)
		// 	return ;
		// dup2(sd, cmd->fd_out);
		close(cmd->fd_input);
		close(cmd->fd_out);
		
		exit(0);
}

void cmd_pwd(t_cmd *cmd)
{
	char cwd[1024];

	printf("%s\n",getcwd(cwd, sizeof(cwd)));
	exit(0);
}

void	ft_command(t_cmd *cmd )
{	
	if(cmd->fd_input != 0)
		dup2(cmd->fd_input , STDIN_FILENO);
	if(cmd->fd_out != 1)	
		dup2(cmd->fd_out , STDOUT_FILENO);
	if (cmd->next)
		close(cmd->fd[1]);
	if(cmd->fd_input != 0)
		close(cmd->fd_input);
	if(cmd->fd_out != 1)
		close(cmd->fd_out);
	if (ft_strnstr(cmd->cmd[0], "echo", 5))
		cmd_echo(cmd);
	if(ft_strnstr (cmd->cmd[0], "pwd", 4))
		cmd_pwd(cmd);
	else
	{
		execve(cmd->cmd_path, cmd->cmd, NULL);
		error_message("Error execve");
	}
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

void cmd_cd(t_cmd *cmd)
{
	char *ss;
	char *str;
	char cwd[1024];

	ss = NULL;
	while(cmd->env)
	{
		if (ft_strnstr(cmd->env->env, "PWD", 4))
		{
			printf(">>>>%s\n",cmd->env->env);
			ss = cmd->env->env + 4;
			break;
		}
		cmd->env = cmd->env->next;
	}
	if (!ss)
	{
		printf("no pwd\n");
		return ;
	}
	ss = ft_strjoin_no_free(ss ,"/");
	str = ss;
	ss = ft_strjoin_no_free(ss, cmd->cmd[1]);
	free(str);
	printf("ss == %s\n", ss);
	if (!chdir(ss))
	{
		getcwd(cwd, sizeof(cwd));
		printf("cwd>>>>%s\n",cwd);
		free(cmd->env->env);
		cmd->env->env = ft_strjoin_no_free("PWD=",cwd);
		// free(ss);
		// exit(0);
	}
	else
		printf("chdir fail\n");
}


void	exection(t_cmd *cmd)
{
	t_cmd *tmp;
	cmd->paths = get_path(cmd->env);
	tmp = cmd;
	// if (cmd->paths == NULL)
	// {
	// 	printf("hhhhh\n");
	// }
	while (tmp)
	{
		// if(tmp->cmd == ' ')
		// 	tmp = tmp->next;
		tmp->paths = cmd->paths;
		tmp = tmp->next;
	}
	if(!cmd->next && word_stop("cd", cmd->cmd[0]))
	{
		cmd_cd(cmd);
	}
	else
	ft_pipe(cmd);
}
