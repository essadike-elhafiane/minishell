/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_files_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 20:08:08 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/16 21:17:59 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void creat_files(t_shell **tmp ,t_cmd *cmd)
{
	cmd->fd_input = 0;
	cmd->fd_out = 1;
	
	while((*tmp) && (*tmp)->type != PIPE)
	{
		if ((*tmp)->type == IN)
		{
			while((*tmp)->type != WORD && tmp)
				(*tmp) = (*tmp)->next;
			cmd->fd_input = open((*tmp)->s, O_RDONLY);
			if (cmd->fd_input < 0)
			{
				printf("No such file or directory\n");
			}
		}
		if ((*tmp)->type == OUT)
		{
			while((*tmp)->type != WORD && tmp)
				(*tmp) = (*tmp)->next;
			cmd->fd_out = open((*tmp)->s,O_CREAT | O_RDWR, 777);
			if (cmd->fd_out < 0)
			{
				printf("No such file or directory\n");
			}
		}
		(*tmp) = (*tmp)->next;
	}
	if((*tmp)  && (*tmp)->type == PIPE)
	{
		(*tmp) = (*tmp)->next;
		return;
	}
}

t_cmd    *creat_cmd(t_shell *data)
{
	t_shell *tmp;
	t_stk   y;
	t_cmd	*cmds;
	t_cmd	*tmp_cmd;

	tmp = data;
	cmds = (t_cmd *)malloc(sizeof(t_cmd));
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
					tmp_cmd->fd_input = 0;
				tmp = tmp->next;
			}
			if (tmp && tmp->type == OUT)
			{
				tmp = tmp->next;
				if (tmp && tmp->type == SPACE)
					tmp = tmp->next;
				tmp_cmd->fd_out = open(tmp->s, O_CREAT | O_RDWR | O_TRUNC, 777);
				tmp = tmp->next;
				// write (tmp_cmd->fd_out, "sdfgsfgasf\n", 11);
			}
			if (tmp && tmp->type == APPEND)
			{
				tmp = tmp->next;
				if (tmp && tmp->type == SPACE)
					tmp = tmp->next;
				tmp_cmd->fd_out = open(tmp->s,O_CREAT | O_RDWR, 777);
				tmp = tmp->next;
			}
			if (tmp && tmp->type == HER)
			{
				tmp = tmp->next;
				if (tmp && tmp->type == SPACE)
					tmp = tmp->next;
				// tmp_cmd->fd_out == open(tmp->next->s,O_CREAT | O_RDWR, 777);
				tmp = tmp->next;
			}
			if (tmp && tmp->type == SPACE)
				tmp = tmp->next;
			t_shell *r;
			r = tmp;
			while(tmp && (tmp->type == WORD || tmp->type == DOUBLE || tmp->type == SINGLE))
			{
				while(tmp && (tmp->type == WORD || tmp->type == DOUBLE || tmp->type == SINGLE || tmp->type == SPACE))
				{
					printf("%s\n\n", tmp->s);
					if (tmp && tmp->type == SPACE)
						tmp = tmp->next;
					if (tmp && (tmp->type == WORD || tmp->type == DOUBLE || tmp->type == SINGLE || tmp->type == SPACE))
					{
						tmp = tmp->next;
						y.i++;
					}
					y.j = -1;
					tmp_cmd->cmd = (char **)malloc(sizeof(char *) * (y.i + 1));
					tmp_cmd->cmd[y.i] = NULL;
					while (++y.j < y.i)
					{
						
					}
					
				}
				printf("%d\n\n", y.i);
			}
				
		}
	}
	return (cmds);
}