/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 20:13:57 by eelhafia          #+#    #+#             */
/*   Updated: 2023/05/14 02:28:04 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		x;
	char	*str;

	i = 0;
	x = 0;
	if (!s1 && !s2)
		return (NULL);
	if (!s2)
		s2 = ft_strdup("");
	if (!s1)
		s1 = ft_strdup("");
	str = malloc((ft_strlen(s1) + ft_strlen(s2) +1) * sizeof(char));
	if (!str)
		return (0);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[x])
		str[i++] = s2[x++];
	str[i] = '\0';
	return (free(s1), s1 = NULL, free(s2), s2 = NULL, str);
}
