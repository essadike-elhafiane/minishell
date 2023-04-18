/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 22:48:31 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/18 02:26:01 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	fun_free(t_shell **a)
{
	t_shell	*tmp;

	while (*a)
	{
		tmp = *a;
		*a = (*a)->next;
		free(tmp->s);
		free(tmp);
	}
	a = NULL;
}

void	fun_free_env(t_env **a)
{
	t_env	*tmp;

	while (*a)
	{
		tmp = *a;
		*a = (*a)->next;
		free(tmp->env);
		free(tmp);
	}
	a = NULL;
}

void    fun_free_cmd(t_cmd **a)
{
	t_cmd	*tmp;
	int		i;

	while (*a)
	{
		i = 0;
		tmp = *a;
		*a = (*a)->next;
		if (tmp->cmd)
		{
			while(tmp->cmd[i])
				free(tmp->cmd[i++]);
		}
		if (tmp->paths)
		{
			while(tmp->paths[i])
				free(tmp->paths[i++]);
		}
		free(tmp->cmd);
		free(tmp);
	}
	a = NULL;
}
