/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exection_saddik.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 08:40:36 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/18 02:10:02 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_close_fd(t_cmd *y, int *fd)
{
	close(y->fd_input);
	close(y->fd_out);
	close(fd[1]);
	close(fd[0]);
}

void	ft_puterror(char *s)
{
	ft_putstr_fd(s, 1);
	exit(1);
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


void	fork2(t_cmd *cmd, int *fd, t_stk *y)
{
	// y->cmd1 = ft_split(av[3], ' ');
	// y->check_cmdf = 0;
	y->i = 0;
	y->j = 0;
	while (cmd->path[y->i])
	{
		y->ss = ft_strjoin_exe(cmd->path[y->i], cmd->cmd[0]);
		if (!access(y->ss, X_OK))
		{
			y->j = 1;
			break ;
		}
		free(y->ss);
		y->i++;
	}
	if (!y->j)
		printf("cmd not fond !\n");
	// close(cmd->fd_out);
	dup2(fd[0], cmd->fd_input);
	close(fd[0]);
	// close(fd[cmd->fd_input]);
	// dup2(fd[1], cmd->fd_out);
	close(fd[1]);
	
	// close(cmd->fd_out);
	// if (dup2(fd[0], 0) < 0)
	// 	ft_puterror("Error in dup2 !\n");
	// if (dup2(cmd->fd_out, 1) < 0)
	// 	ft_puterror("Error in dup2 !\n");
	// close(fd[1]);
	// close(cmd->fd_out);
	// ft_close_fd(cmd, fd);
	// close(fd[1]);
	// close(fd[2]);
	if (execve(y->ss, cmd->cmd, NULL) < 0)
		ft_puterror("Error in execve !\n");
}

void	fork11(t_cmd *cmd, int *fd, t_stk *y)
{
	y->j = 0;
	y->i = 0;
	// close(fd[0]);
	// y->cmd1 = ft_split(av[2], ' ');
	while (cmd->path[y->i])
	{
		y->ss = ft_strjoin_exe(cmd->path[y->i], cmd->cmd[0]);
		if (!access(y->ss, X_OK))
		{
			y->j = 1;
			break ;
		}
		free(y->ss);
		y->i++;
	}
	if (!y->j)
		printf("cmd not fond !\n");
		// ft_puterror1("Error command path not fond !\n", av[4]);
	// printf("int -> %d && out -> %d \n",cmd->fd_input, cmd->fd_out);
	// close(fd[0]);
	// close(fd[1]);
	// close(cmd)
	// close(cmd->cmd)
	// printf("%s\n", y->ss);
	dup2(fd[0], cmd->fd_input);
	// if (cmd->fd_out != 1)
	// {
	// 	dup2(cmd->fd_out, 1);
	// 	close(cmd->fd_out);
	// }
	// else
	// 	dup2(fd[1], 1);
	close(fd[0]);
	close(fd[1]);
	// if (dup2(cmd->fd_input, 0) < 0)
	// 	ft_puterror("Error in dup2 !\n");
	// if (dup2(fd[1], 1) < 0)
	// 	ft_puterror("Error in dup2 !\n");
	// close(fd[1]);
	// close(cmd->fd_input);
	// ft_close_fd(cmd, fd);
	if (execve(y->ss, cmd->cmd, NULL) < 0)
		ft_puterror("Error in execve !\n");
}

int	pipex(t_cmd *cmd)
{
	int		fd[2];
	pid_t	fork1;
	t_stk	y;
	int		flg_n;
	int sd;
	// t_stk1	y;
	y.i = 1;
	flg_n = 0;
	// y.input = open(av[1], O_RDONLY, 0777);
	// if (y.input < 0)
	// 	ft_puterror("Error in file input\n");
	// y.output = open(av[4], O_CREAT | O_WRONLY | O_TRUNC, 0777);
	// if (y.output < 0)
	// 	ft_puterror("Error in file output\n");
	if (word_stop("echo", cmd->cmd[0]))
	{
		y.i = 1;
		if (cmd->fd_out != 1)
		{
			sd = dup(1);
			dup2(cmd->fd_out, 1);
		}
		while (cmd->cmd[y.i])
		{
			y.j = 0;
			if (word_stop("-n", cmd->cmd[y.i]))
			{
				flg_n = 1;
				y.i++;
			}
			if (cmd->cmd[y.i][y.j] == '\"')
			{
				while (cmd->cmd[y.i][y.j])
				{
					if (cmd->cmd[y.i][y.j] == '\"')
						y.j++;
					else
					{
						write(cmd->fd_out, &cmd->cmd[y.i][y.j], 1);
						y.j++;
					}
				}
			}
			else if (cmd->cmd[y.i][y.j] == 39)
			{
				while (cmd->cmd[y.i][y.j])
				{
					if (cmd->cmd[y.i][y.j] == 39)
						y.j++;
					else
					{
						write(cmd->fd_out, &cmd->cmd[y.i][y.j], 1);
						y.j++;
					}
				}
			}
			else
			{
				while (cmd->cmd[y.i][y.j])
				{
					// if (cmd->cmd[y.i][y.j] == 39)
					// 	y.j++;
					// else
					// {
						write(cmd->fd_out, &cmd->cmd[y.i][y.j], 1);
						y.j++;
					// }
				}
			}
			y.i++;
			// if (cmd->cmd[y.i])
			// 	write(1, " ", 1);
		}
		if (!flg_n)
			printf("\n");
		if (cmd->fd_out != 1)
			dup2(1, cmd->fd_out);
		cmd = cmd->next;
		if (!cmd)
			return (0);
		dup2(sd, cmd->fd_out);
		close(cmd->fd_out);
	}
	y.i = 1;
	if (pipe(fd) < 0)
		ft_puterror("Error in pipe !\n");
	// exit_notpath(av, path);
	// printf("i -> %d o -> %d | cmd -> %s | path -> %s ", cmd->fd_input, cmd->fd_out, cmd->cmd[0], cmd->path[0]);
	if (cmd)
	{
	fork1 = fork();
	if (fork1 < 0)
		ft_puterror("Error in fork1 !\n");
	if (!fork1)
		fork11(cmd, fd, &y);
	close(fd[1]);
	}
	// while (cmd->next)
	// {
		
	// }
	cmd = cmd->next;
	if (cmd)
	{
		y.i++;
		fork1 = fork();
		if (fork1 < 0)
			ft_puterror("Error in fork1 !\n");
		if (!fork1)
			fork2(cmd, fd, &y);
	}
	close(fd[1]);
	// char rd[100];
	// ssize_t bytes_read = read(fd[0], rd, 100);
	// if (bytes_read == -1) {
	// 	perror("read");
	// 	return (0);
	// }
	// rd[bytes_read] = '\0';
	// printf("%s | %ld\n\n ", rd, bytes_read);
	// close(fd[0]);
	// dup2(fd[0], 0);
	// close(fd[0]);
	// if (dup2(fd[0], 0) < 0)
	// 	printf("sfs\n");
	// fork1 = fork();
	// if (fork1 < 0)
	// 	ft_puterror("Error in fork2 !\n");
	// if (!fork1)
	// 	fork2(cmd, fd, &y);
	// close(fd[0]);
	// cmd = cmd->next;
	// printf("dfsdg\n");
	// if(cmd->fd_out != 1)
	// 	close(cmd->fd_out);
	// ft_close_fd(cmd, fd);
	return (wait(NULL), wait(NULL), 0);
}

void	exection(t_cmd *cmd)
{
	t_cmd *tmp;

	cmd->path = get_path(cmd->env);
	tmp = cmd;
	if (cmd->path == NULL)
		ft_puterror("Error in path !!\n");
	while (tmp)
	{
		// if(tmp->cmd == ' ')
		// 	tmp = tmp->next;
		tmp->path = cmd->path;
		tmp = tmp->next;
	}
	pipex(cmd);
}