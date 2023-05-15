/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_inp_out_a.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:50:21 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/15 19:41:49 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	creat_inp_1(t_shell **tmp, t_cmd **tmp_cmd)
{
	(*tmp_cmd)->fd_input = open((*tmp)->s, O_RDONLY);
	if ((*tmp_cmd)->fd_input < 0)
	{
		while ((*tmp) && (*tmp)->type != PIPE && (*tmp)->type != HER)
			(*tmp) = (*tmp)->next;
		if (((*tmp) && (*tmp)->type != HER) || !(*tmp))
			return (1);
	}
	if ((*tmp)->type != HER)
		(*tmp) = (*tmp)->next;
	return (0);
}

int	creat_inp(t_shell **tmp)
{
	(*tmp) = (*tmp)->next;
	if ((*tmp) && (*tmp)->type == WSPACE)
		(*tmp) = (*tmp)->next;
	while ((*tmp) && (*tmp)->next && (*tmp)->next->type != WSPACE
		&& !check_is_token((*tmp)->next->type))
	{
		(*tmp)->next->s = ft_strjoin_no_free((*tmp)->s, (*tmp)->next->s);
		if ((*tmp)->type != WORD)
			(*tmp)->next->type = SINGLE;
		(*tmp) = (*tmp)->next;
	}
	if ((*tmp) && (*tmp)->var_re
		&& (*tmp)->len_spl > 1 && (*tmp)->type != DOUBLE)
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", 2);
		g_status = 1;
		while ((*tmp) && (*tmp)->type != PIPE && (*tmp)->type != HER)
			(*tmp) = (*tmp)->next;
		if (((*tmp) && (*tmp)->type != HER) || !(*tmp))
			return (1);
	}
	return (0);
}

int	creat_inp_out(t_shell **tmp, t_cmd **tmp_cmd)
{
	if ((*tmp) && (*tmp)->type == IN)
	{
		if (creat_inp(tmp))
			return (1);
		if (creat_inp_1(tmp, tmp_cmd))
			return (1);
	}
	if ((*tmp) && (*tmp)->type == OUT)
	{
		(*tmp) = (*tmp)->next;
		if ((*tmp) && (*tmp)->type == WSPACE)
			(*tmp) = (*tmp)->next;
		while ((*tmp) && (*tmp)->next && (*tmp)->next->type != WSPACE
			&& !check_is_token((*tmp)->next->type))
		{
			(*tmp)->next->s = ft_strjoin_no_free((*tmp)->s, (*tmp)->next->s);
			if ((*tmp)->type != WORD)
				(*tmp)->next->type = SINGLE;
			(*tmp) = (*tmp)->next;
		}
		(*tmp_cmd)->fd_out = open((*tmp)->s, O_CREAT | O_RDWR | O_TRUNC, 0777);
		(*tmp) = (*tmp)->next;
	}
	return (0);
}

void	creat_append(t_shell **tmp, t_cmd **tmp_cmd)
{
	if ((*tmp) && (*tmp)->type == APPEND)
	{
		(*tmp) = (*tmp)->next;
		if ((*tmp) && (*tmp)->type == WSPACE)
			(*tmp) = (*tmp)->next;
		while ((*tmp) && (*tmp)->next && (*tmp)->next->type != WSPACE
			&& !check_is_token((*tmp)->next->type))
		{
			(*tmp)->next->s = ft_strjoin_no_free((*tmp)->s, (*tmp)->next->s);
			if ((*tmp)->type != WORD)
				(*tmp)->next->type = SINGLE;
			(*tmp) = (*tmp)->next;
		}
		(*tmp_cmd)->fd_out = open((*tmp)->s, O_CREAT
				| O_RDWR | O_APPEND, 0777);
		(*tmp) = (*tmp)->next;
	}
}
