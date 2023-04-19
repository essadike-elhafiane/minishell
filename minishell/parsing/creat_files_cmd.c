/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_files_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 20:08:08 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/18 21:09:35 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	word_stop(char *word, char *str)
{
	int	i;

	i = 0;
	while(word[i] == str[i])
	{
		if(word[i] == '\0' && str[i] == '\0')
			return (1);
		i++;
	}
	return (0);
}

t_cmd    *creat_cmd(t_shell *data)
{
	t_shell *tmp;
	t_stk   y;
	t_cmd	*cmds;
	t_cmd	*tmp_cmd;

	tmp = data;
	cmds = (t_cmd *)malloc(sizeof(t_cmd));
	cmds->fd_input = 0;
	cmds->env = data->env;
	cmds->cmd = NULL;
	cmds->fd_out = 1;
	cmds->next = NULL;
	tmp_cmd = cmds;
	y.i = 0;
	while(tmp)
	{
		while(tmp && tmp->type != PIPE)
		{
			if (tmp && tmp->type == SPACE)
				tmp = tmp->next;
			if (tmp && tmp->type == IN)
			{
				tmp = tmp->next;
				if (tmp && tmp->type == SPACE)
					tmp = tmp->next;
				tmp_cmd->fd_input = open(tmp->s, O_RDONLY);
				if (tmp_cmd->fd_input < 0)
				{
					printf("Minishell$: %s: No such file or directory\n", tmp->s);
					// return (NULL);
				}
				tmp = tmp->next;
			}
			if (tmp && tmp->type == OUT)
			{
				tmp = tmp->next;
				if (tmp && tmp->type == SPACE)
					tmp = tmp->next;
				tmp_cmd->fd_out = open(tmp->s, O_CREAT | O_RDWR | O_TRUNC, 0777);
				tmp = tmp->next;
				// printf("%s", tmp->s);
				// write (tmp_cmd->fd_out, "sdfgsfgasf\n", 11);
			}
			if (tmp && tmp->type == APPEND)
			{
				tmp = tmp->next;
				if (tmp && tmp->type == SPACE)
					tmp = tmp->next;
				tmp_cmd->fd_out = open(tmp->s, O_CREAT | O_RDWR | O_APPEND, 0777);
				tmp = tmp->next;
			}
			// if (tmp && tmp->type == HER)
			// {
			// 	tmp = tmp->next;
			// 	if (tmp && tmp->type == SPACE)
			// 		tmp = tmp->next;
			// 	// tmp_cmd->fd_input = open("src", O_CREAT | O_RDWR | O_TRUNC, 777);
			// 	// if (tmp_cmd->fd_input < 0)
			// 	// {
			// 	// 	printf("Minishell$: %s; error in file herdoc\n", tmp->s);
			// 	// 	return (NULL);
			// 	// }
			// 	int fd[2];
				
			// 	// pipe(fd);
			// 	fd[1] = open("src", O_CREAT | O_RDWR | O_TRUNC, 777);
				
			// 	// pid_t forr = fork();
			// 	// if (!forr)
			// 	// {
			// 	// 	y.ss = readline("> ");
					
			// 	// 	while(y.ss && y.ss[0] != 'l')
			// 	// 	{
			// 	// 		// dup2(fd, 1);
			// 	// 		// dup2(fd[1], 1);
			// 	// 		// printf("%s\n", y.ss);
			// 	// 		ft_putstr_fd(y.ss, fd[1]);
			// 	// 		write(fd[1], "\n", 1);
			// 	// 		// close(fd[1]);
			// 	// 		// kill(forr, SIGTERM);
			// 	// 		free(y.ss);
			// 	// 		y.ss = readline("> ");
			// 	// 	}
			// 	// 	close(fd[1]);
			// 	// 	exit(0);
					
			// 	// }
			// 	// wait(0);
			// 	// close(fd[1]);
			// 	printf ("%d\n",fd[1]);
			// 	// write(fd[1], "fhdhhshsh\n", 10);
			// 	ft_putstr_fd("saddik\n", fd[1]);
			// 	// close(fd[1]);
			// 	// tmp_cmd->fd_input = fd[1];
			// 	char	*rd;
			// 	rd = malloc((4) * sizeof(char));
			// 	if (!rd)
			// 		return (NULL);
		
			// 	size_t i = read(fd[1], rd, 3);
			// 	if (i < 0)
			// 		printf("dfsfs\n");
			// 	rd[3] = '\0';
				
			// 	printf("%c | %ld\n\n ", rd[0],i);
				
			// 	// close(tmp_cmd->fd_input)
			// 	tmp = tmp->next;
			// }


			
			if (tmp && tmp->type == HER)
			{
				tmp = tmp->next;
				if (tmp && tmp->type == SPACE)
					tmp = tmp->next;
				int fd[2];
				if (pipe(fd) == -1)
				{
					perror("pipe");
					return NULL;
				}
			
				y.pid = fork();
				if (y.pid  == -1)
				{
					perror("fork");
					return NULL;
				} 
				else if (y.pid == 0)
				{
					close(fd[0]); // close read end of pipe
					// Send input to write end of pipe
					y.ss = readline("> ");
					while (y.ss && !word_stop(tmp->s, y.ss))
					{
						write(fd[1], y.ss, ft_strlen(y.ss));
						write(fd[1], "\n", 1);
						free(y.ss);
						y.ss = readline("> ");
					}
					close(fd[1]); // close write end of pipe
					exit(0);
				} 
				else 
				{
					close(fd[1]); // close write end of pipe
					wait(0);
					tmp_cmd->fd_input = fd[0];
					tmp = tmp->next;
				}
		}




		
			if (tmp && tmp->type == SPACE)
				tmp = tmp->next;
			t_shell *r;
			r = tmp;
			while(tmp && (tmp->type == WORD || tmp->type == DOUBLE || tmp->type == SINGLE))
			{
				y.i = 0;
				while(tmp && (tmp->type == WORD || tmp->type == DOUBLE || tmp->type == SINGLE || tmp->type == SPACE))
				{
					
					if (tmp && tmp->type == SPACE)
						tmp = tmp->next;
					if (tmp && (tmp->type == WORD || tmp->type == DOUBLE || tmp->type == SINGLE ))
					{
						// printf("%s\n\n", tmp->s);
						y.i++;
						while (tmp && (tmp->type == WORD || tmp->type == DOUBLE || tmp->type == SINGLE ))
							tmp = tmp->next;
					}
				}
				y.j = 0;
				// printf("%d\n\n", y.i);
				tmp_cmd->cmd = (char **)malloc(sizeof(char *) * (y.i + 1));
				tmp_cmd->cmd[y.i] = NULL;
				while (y.j < y.i && r)
				{
					if (r && (r->type == WORD || r->type == DOUBLE || r->type == SINGLE))
					{
						tmp_cmd->cmd[y.j] = ft_strdup(r->s);
						// printf("%s\n\n", r->s);
						r = r->next;
						while (r && (r->type == WORD || r->type == DOUBLE || r->type == SINGLE ))
						{
							// printf("%s\n", tmp_cmd->cmd[y.j]);
							tmp_cmd->cmd[y.j] = ft_strjoin(tmp_cmd->cmd[y.j], ft_strdup(r->s));
							r = r->next;
						}
						// printf("%s\n", tmp_cmd->cmd[y.j]);
						// if (r->next && r->next->type != SPACE)
						// 	tmp_cmd->flg_space = 1;
						y.j++;
					}
					else
						r = r->next;
				}
				// if (tmp && tmp->type == SPACE)
				// 		tmp = tmp->next;
			}
				// exit(0);
		}
		if (tmp)
		{
			tmp = tmp->next;
			tmp_cmd->next = (t_cmd *)malloc(sizeof(t_cmd));
			tmp_cmd = tmp_cmd->next;
			tmp_cmd->fd_input = 0;
			tmp_cmd->env = data->env;
			tmp_cmd->fd_out = 1;
			tmp_cmd->next = NULL;
		}
	}
	return (cmds);
}