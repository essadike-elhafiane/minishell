/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_files_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 20:08:08 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/13 22:43:29 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	word_stop(char *word, char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while(word[i] == str[i])
	{
		if(word[i] == '\0' && str[i] == '\0')
			return (1);
		i++;
	}
	return (0);
}

int	check_is_token(int type)
{
	if (type == APPEND || type == HER || type == OUT || type == IN || type == PIPE)
		return (1);
	return (0);
}

void	signal_her(int signal)
{
	if (signal == SIGINT)
	{
		if (waitpid(-1, NULL, WNOHANG))
		{
			
			exit(3);
		}
    }
}

t_cmd    *creat_cmd(t_shell *data)
{
	t_shell *tmp;
	t_stk   y;
	t_cmd	*cmds;
	t_cmd	*tmp_cmd;
	void	*tmp_free;

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
			if (tmp && tmp->type == WSPACE)
				tmp = tmp->next;
			if (tmp && tmp->type == IN)
			{
				tmp = tmp->next;
				if (tmp && tmp->type == WSPACE)
					tmp = tmp->next;
				while (tmp && tmp->next && tmp->next->type != WSPACE && !check_is_token(tmp->next->type))
				{
					tmp->next->s = ft_strjoin_no_free(tmp->s, tmp->next->s);
					if (tmp->type != WORD)
						tmp->next->type = SINGLE;
					tmp = tmp->next;
				}
				if (tmp && tmp->var_re && tmp->len_spl > 1 && tmp->type != DOUBLE)
				{
					ft_putstr_fd("minishell: ambiguous redirect\n", 2);
					status = 1;
					while (tmp && tmp->type != PIPE && tmp->type != HER)
						tmp = tmp->next;
					if ((tmp && tmp->type != HER) || !tmp)
						break;
				}
				tmp_cmd->fd_input = open(tmp->s, O_RDONLY);
				if (tmp_cmd->fd_input < 0)
				{
					while (tmp && tmp->type != PIPE && tmp->type != HER)
						tmp = tmp->next;
					if ((tmp && tmp->type != HER) || !tmp)
						break;
				}
				if (tmp->type != HER)
					tmp = tmp->next;
			}
			if (tmp && tmp->type == OUT)
			{
				tmp = tmp->next;
				if (tmp && tmp->type == WSPACE)
					tmp = tmp->next;
				while (tmp && tmp->next && tmp->next->type != WSPACE && !check_is_token(tmp->next->type))
				{
					tmp->next->s = ft_strjoin_no_free(tmp->s, tmp->next->s);
					if (tmp->type != WORD)
						tmp->next->type = SINGLE;
					tmp = tmp->next;
				}
				tmp_cmd->fd_out = open(tmp->s, O_CREAT | O_RDWR | O_TRUNC, 0777);
				tmp = tmp->next;
			}
			if (tmp && tmp->type == APPEND)
			{
				tmp = tmp->next;
				if (tmp && tmp->type == WSPACE)
					tmp = tmp->next;
				while (tmp && tmp->next && tmp->next->type != WSPACE && !check_is_token(tmp->next->type))
				{
					tmp->next->s = ft_strjoin_no_free(tmp->s, tmp->next->s);
					if (tmp->type != WORD)
						tmp->next->type = SINGLE;
					tmp = tmp->next;
				}
				tmp_cmd->fd_out = open(tmp->s, O_CREAT | O_RDWR | O_APPEND, 0777);
				tmp = tmp->next;
			}
			

			
			if (tmp && tmp->type == HER)
			{
				signal(SIGINT, signal_her);
				signal(SIGTERM, signal_her);
				y.ss = NULL;
				tmp = tmp->next;
				if (tmp && tmp->type == WSPACE)
					tmp = tmp->next;
				while (tmp && tmp->next && tmp->next->type != WSPACE && !check_is_token(tmp->next->type))
				{
					tmp->next->s = ft_strjoin_no_free(tmp->s, tmp->next->s);
					if (tmp->type != WORD)
						tmp->next->type = SINGLE;
					tmp = tmp->next;
				}
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
					close(fd[0]); 
					while (1 && !word_stop(tmp->s, y.ss))
					{
						y.ss = readline("> ");
						if (!y.ss)
							exit(0);	
						if (y.ss && tmp->type != SINGLE && tmp->type != DOUBLE)
						{
							char *ex;
							
							y.i = 0;
							while(y.ss && y.ss[y.i])
							{
								if (y.ss[y.i] == '$' && (y.ss[y.i + 1] == '$'))
									y.i++;
								if (y.ss[y.i] == '$' && y.ss[y.i +1] != '\0' && (y.ss[y.i + 1] != '$' && y.ss[y.i + 1] != ' '))
								{
									y.i++;
									y.b = y.i;
									while((ft_isalpha(y.ss[y.i]) || ft_isdigit(y.ss[y.i])))
										y.i++;
									ex = ft_substr(y.ss, y.b, y.i - y.b);
									ex = expand(ex, *data->env);
									y.back = ft_strdup(y.ss + y.i);
									y.front = ft_substr(y.ss, 0, y.b -1);
									free(y.ss);
									if (y.front)
										y.ss = ft_strjoin(y.front, ex);
									y.ss = ft_strjoin(y.ss, y.back);
								}
								y.i++;
							}
						}
						write(fd[1], y.ss, ft_strlen(y.ss));
						write(fd[1], "\n", 1);
						free(y.ss);
					}
					close(fd[1]);
					exit(0);
				}
				else
				{
					int ha;
					close(fd[1]);
					wait(&ha);
					status = WEXITSTATUS(ha);
					if (WEXITSTATUS(ha) == 3)
						return (fun_free_cmd(&cmds), NULL);
					tmp_cmd->fd_input = fd[0];
					tmp = tmp->next;
				}
		}




		
			if (tmp && tmp->type == WSPACE)
				tmp = tmp->next;
			t_shell *r;
			r = tmp;
			while(tmp && (tmp->type == WORD || tmp->type == DOUBLE || tmp->type == SINGLE))
			{
				y.i = 0;
				while(tmp && (tmp->type == WORD || tmp->type == DOUBLE || tmp->type == SINGLE || tmp->type == WSPACE))
				{
					if (tmp && tmp->type == WSPACE)
						tmp = tmp->next;
					if (tmp && (tmp->type == WORD || tmp->type == DOUBLE || tmp->type == SINGLE ))
					{
						y.i++;
						while (tmp && (tmp->type == WORD || tmp->type == DOUBLE || tmp->type == SINGLE ))
							tmp = tmp->next;
					}
				}
				y.j = 0;
				tmp_cmd->cmd = (char **)malloc(sizeof(char *) * (y.i + 1));
				tmp_cmd->cmd[y.i] = NULL;
				while (y.j < y.i && r)
				{
					if (r && (r->type == WORD || r->type == DOUBLE || r->type == SINGLE))
					{
						tmp_cmd->cmd[y.j] = ft_strdup(r->s);
						r = r->next;
						while (r && (r->type == WORD || r->type == DOUBLE || r->type == SINGLE ))
						{
							tmp_cmd->cmd[y.j] = ft_strjoin(tmp_cmd->cmd[y.j], ft_strdup(r->s));
							r = r->next;
						}
						y.j++;
					}
					else
						r = r->next;
				}
			}
		}
		if (tmp)
		{
			tmp = tmp->next;
			tmp_cmd->next = (t_cmd *)malloc(sizeof(t_cmd));
			tmp_cmd = tmp_cmd->next;
			tmp_cmd->cmd = NULL;
			tmp_cmd->fd_input = 0;
			tmp_cmd->env = data->env;
			tmp_cmd->fd_out = 1;
			tmp_cmd->next = NULL;
		}
	}
	signal(SIGINT, signal_handler);
	return (cmds);
}