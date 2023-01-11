/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 10:32:57 by mhrima            #+#    #+#             */
/*   Updated: 2022/10/20 00:48:08 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	size_t			len;
	char			*res;

	if (!s || !f)
		return (NULL);
	i = 0;
	len = ft_strlen(s);
	res = (char *)malloc((len + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	while (s && s[i])
	{
		res[i] = f(i, s[i]);
		i++;
	}
	res[i] = '\0';
	return (res);
}
