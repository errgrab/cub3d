/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:22:34 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/24 16:38:50 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s = (char *)src;

	if (!dest || !src)
		return (NULL);
	if (dest == src)
		return (dest);
	d = (char *)dest;
	if (d < s)
		while (n--)
			*d++ = *s++;
	else
		while (n--)
			d[n] = s[n];
	return (dest);
}

char	*ft_strstr(char const *s1, char const *s2)
{
	const char	*h;
	const char	*n;

	if (!*s2)
		return ((char *)s1);
	while (*s1)
	{
		h = s1;
		n = s2;
		while (*n && *h == *n)
		{
			h++;
			n++;
		}
		if (!*n)
			return ((char *)s1);
		s1++;
	}
	return (NULL);
}
