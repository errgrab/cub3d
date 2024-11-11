/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvichi <lvichi@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by lvichi            #+#    #+#             */
/*   Updated: 2024/10/22 20:03:08 by lvichi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	remove_first_line(char *buffer);
char	**ft_split(char *str, char c);
void	ft_array_free(char **array);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

void	remove_first_line(char *buffer)
{
	char	*line_end;
	int		remainder_length;

	line_end = ft_strchr(buffer, '\n');
	if (line_end != NULL)
	{
		remainder_length = ft_strlen(line_end + 1);
		ft_memmove(buffer, line_end + 1, remainder_length + 1);
	}
	else
		buffer[0] = '\0';
}

char	**ft_split(char *str, char c)
{
	int		i;
	int		j;
	int		k;
	char	**str_split;

	i = -1;
	j = -1;
	k = 0;
	str_split = ft_calloc2(ft_strlen(str) + 1, sizeof(char *));
	while (str_split && str[++i])
	{
		if (str[i] == c || str[i + 1] == '\0')
		{
			str_split[++j] = ft_calloc2(i - k + 2, sizeof(char));
			if (!str_split[j])
				return (ft_array_free(str_split), NULL);
			if (str[i + 1] == '\0' && str[i] != c)
				ft_memmove(str_split[j], str + k, i - k + 1);
			else
				ft_memmove(str_split[j], str + k, i - k);
			k = i + 1;
		}
	}
	return (str_split);
}

void	ft_array_free(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
