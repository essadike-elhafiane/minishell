/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 18:28:36 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/10 02:50:39 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

// void	envadd_back(t_env **lst, t_env *new)
// {
// 	t_env	*llst;

// 	if (!new)
// 		return ;
// 	if (!*lst)
// 	{
// 		llst = new;
// 	}
// 	else
// 	{	
// 		llst = (*lst);
// 		while (llst->next)
// 		{
// 			llst = llst->next;
// 		}
// 		if(!llst->next)
// 		{
// 			llst->next = new;
// 			llst =llst->next;
// 		}
// 	}
// 	return ;
// }

// void add_env (t_env*envs, t_cmd *cmd)
// {
// 	t_env *tmp;
// 	t_export *tmpex;
// 	int flg;
// 	t_env *new;

// 	if(!envs)
// 		return ;
// 	tmp = envs;

// 	tmpex = envs->export;
// 	while(tmpex)
// 	{
// 		tmp = cmd->env;
// 		flg = 0;
// 		while(tmp)
// 		{
// 				if(ft_strcchr(tmp->env, '=') > -1)
// 				{
// 					if(!ft_strncmp(tmpex->export,tmp->env, ft_strlen(tmpex->export)))
// 					{
// 						flg = 1;
// 						break;
// 					}		
// 					else if (!ft_strncmp(tmp->env , tmpex->export , ft_strcchr( tmp->env, '=') + 1))
// 					{
// 						flg = 1;
// 						free(tmp->env);
// 						tmp->env = ft_strdup(tmpex->export);
// 						break;
// 					}	
// 				}
// 			tmp = tmp->next;
// 		}
// 		// if(!ft_strncmp(tmpex->export,"m=a", ft_strlen(tmpex->export)))

// 		if(!flg && tmpex && ft_strcchr(tmpex->export, '=') > -1 )
// 		{
// 			new = malloc(sizeof(t_env));
// 			new->env = ft_strdup(tmpex->export);
// 			new->export = cmd->env->export;
// 			new->next = NULL;
// 			envadd_back(&envs, new);
// 		}		
// 		tmpex = tmpex->next;
// 	}
// }
