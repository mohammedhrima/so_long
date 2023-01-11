/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 23:58:56 by mhrima            #+#    #+#             */
/*   Updated: 2022/12/09 01:36:00 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*ptr;
	char			*s1;
	unsigned int	i;

	if (!s || start >= ft_strlen(s))
		return (NULL);
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	ptr = (char *)malloc(len * (sizeof(char) + 1));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	s1 = ft_strdup(s);
	while (i < len && s[start])
	{
		ptr[i] = s1[start];
		start++;
		i++;
	}
	free(s1);
	ptr[i] = '\0';
	return (ptr);
}
