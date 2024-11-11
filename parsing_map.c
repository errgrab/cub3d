/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_usr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvichi <lvichi@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:30:34 by lvichi            #+#    #+#             */
/*   Updated: 2024/10/19 18:30:34 by lvichi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		get_map(t_parsing *p_data);
void	get_map_dimensions(t_parsing *p_data);
int		set_map_temp(t_parsing *p_data);
int		check_map(t_parsing *p_data);
int		check_path(t_parsing *p_data, int i, int j);

int	get_map(t_parsing *p_data)
{
	get_map_dimensions(p_data);
	if (set_map_temp(p_data))
		return (1);
	if (check_map(p_data))
		return (1);
	return (0);
}

void	get_map_dimensions(t_parsing *p_data)
{
	int		num_lines;
	char	**buffer;

	num_lines = 0;
	buffer = p_data->buffer_split;
	while (buffer[num_lines] != NULL)
		num_lines++;
	p_data->map_end = num_lines - 1;
	while (p_data->map_end >= 0 && ft_strlen(buffer[p_data->map_end]) == 0)
		p_data->map_end--;
	p_data->map->width = ft_strlen(buffer[p_data->map_end]);
	p_data->map_start = p_data->map_end;
	while (p_data->map_start >= 0 && ft_strlen(buffer[p_data->map_start]) > 0)
	{
		if (ft_strlen(buffer[p_data->map_start]) > p_data->map->width)
			p_data->map->width = ft_strlen(buffer[p_data->map_start]);
		p_data->map_start--;
	}
	p_data->map_start++;
	p_data->map->height = p_data->map_end - p_data->map_start + 1;
}

int	set_map_temp(t_parsing *p_data)
{
	int	i;
	int	j;
	int	z;

	p_data->map_temp = ft_calloc(p_data->map->height + 1, sizeof(char *));
	if (!p_data->map_temp)
		return (1);
	i = -1;
	while (++i < p_data->map->height)
	{
		p_data->map_temp[i] = ft_calloc(p_data->map->width + 1, sizeof(char));
		if (!p_data->map_temp[i])
			return (1);
	}
	i = -1;
	z = p_data->map_start - 1;
	while (++i < p_data->map->height && ++z <= p_data->map_end)
	{
		j = -1;
		while (p_data->buffer_split[z][++j])
			p_data->map_temp[i][j] = p_data->buffer_split[z][j];
		while (j < p_data->map->width)
			p_data->map_temp[i][j++] = ' ';
	}
	return (0);
}

int	check_map(t_parsing *p_data)
{
	int	i;
	int	j;

	i = -1;
	while (p_data->map_temp[++i])
	{
		j = -1;
		while (p_data->map_temp[i][++j])
		{
			if (!ft_strchr(ALLOWED_CHARS, p_data->map_temp[i][j]))
				return (1);
			if (ft_strchr(MOVE_AREA USER_POS, p_data->map_temp[i][j]))
			{
				if (i == 0 || j == 0 || i == p_data->map->height - 1
					|| j == p_data->map->width - 1)
					return (1);
				if (check_path(p_data, i, j))
					return (1);
			}
		}
	}
	return (0);
}

int	check_path(t_parsing *p_data, int i, int j)
{
	if (ft_strchr(EMPTY_SPACE, p_data->map_temp[i - 1][j]))
		return (1);
	if (ft_strchr(EMPTY_SPACE, p_data->map_temp[i + 1][j]))
		return (1);
	if (ft_strchr(EMPTY_SPACE, p_data->map_temp[i][j - 1]))
		return (1);
	if (ft_strchr(EMPTY_SPACE, p_data->map_temp[i][j + 1]))
		return (1);
	if (ft_strchr(EMPTY_SPACE, p_data->map_temp[i - 1][j - 1]))
		return (1);
	if (ft_strchr(EMPTY_SPACE, p_data->map_temp[i - 1][j + 1]))
		return (1);
	if (ft_strchr(EMPTY_SPACE, p_data->map_temp[i + 1][j - 1]))
		return (1);
	if (ft_strchr(EMPTY_SPACE, p_data->map_temp[i + 1][j + 1]))
		return (1);
	return (0);
}
