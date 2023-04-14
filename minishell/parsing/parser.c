/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:06:57 by eelhafia          #+#    #+#             */
/*   Updated: 2023/04/14 20:43:35 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    parser(t_shell *data)
{
	t_stk	y;
	t_shell	*tmp;

	y.i = 0;
	y.j = 0;
	tmp = data;
	while(tmp)
	{
		if (tmp->type == 'D')
			y.i++;
		if (tmp->type == 'N')
			y.j++;
		tmp = tmp->next;
	}
	if(y.i % 2 != 0 || y.j % 2 != 0)
	{
		printf("minishell: syntax error \n");
		return ;
		printf("%s", y.ss);
	}
}