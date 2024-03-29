/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 20:18:01 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/15 23:28:03 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_double(char **spl)
{
	int	l;

	l = 0;
	while (spl[l])
		free(spl[l++]);
	free(spl);
}

void	clean_cmd_help(t_cmd *cmd, t_stk	*y, char **spl, int flg)
{
	if (flg == 2)
	{
		spl = ft_split(cmd->cmd[y->i], 39);
		while (spl[y->b])
			y->ss = ft_strjoin(y->ss, spl[y->b++]);
		free(cmd->cmd[y->i]);
		cmd->cmd[y->i] = y->ss;
	}
	else if (flg == 3)
	{
		spl = ft_split(cmd->cmd[y->i], 39);
		while (spl[y->b])
			y->ss = ft_strjoin_no_free(y->ss, spl[y->b++]);
		free(cmd->cmd[y->i]);
		cmd->cmd[y->i] = y->ss;
		y->b = 0;
		spl = ft_split(cmd->cmd[y->i], '\"');
		free(y->ss);
		y->ss = NULL;
		while (spl[y->b])
			y->ss = ft_strjoin_no_free(y->ss, spl[y->b++]);
		free(cmd->cmd[y->i]);
		cmd->cmd[y->i] = y->ss;
	}
}

int	word_stop(char *word, char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (word[i] == str[i])
	{
		if (word[i] == '\0' && str[i] == '\0')
			return (1);
		i++;
	}
	return (0);
}

int	check_is_token(int type)
{
	if (type == APPEND || type == HER
		|| type == OUT || type == IN || type == PIPE)
		return (1);
	return (0);
}

void	creat_word_cmd_h_1(t_cmd *tmp_cmd, t_shell **r, t_stk *y)
{
	while (y->j < y->i && (*r))
	{
		if ((*r) && ((*r)->type == WORD
				|| (*r)->type == DOUBLE || (*r)->type == SINGLE))
		{
			tmp_cmd->cmd[y->j] = ft_strdup((*r)->s);
			(*r) = (*r)->next;
			while ((*r) && ((*r)->type == WORD
					|| (*r)->type == DOUBLE || (*r)->type == SINGLE))
			{
				tmp_cmd->cmd[y->j]
					= ft_strjoin(tmp_cmd->cmd[y->j], ft_strdup((*r)->s));
				(*r) = (*r)->next;
			}
			y->j++;
		}
		else
			(*r) = (*r)->next;
	}
}
