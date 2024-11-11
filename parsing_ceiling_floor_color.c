/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_ceiling_floor_color.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvichi <lvichi@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 17:44:53 by lvichi            #+#    #+#             */
/*   Updated: 2024/10/19 17:44:53 by lvichi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_ceiling_floor_color(t_parsing *parsing_data);
int	parse_int(char *line);
int	parse_color(char *line);

int	get_ceiling_floor_color(t_parsing *parsing_data)
{
	char	**buffer_split;

	buffer_split = parsing_data->buffer_split;
	while (*buffer_split)
	{
		if (ft_strncmp(*buffer_split, "C ", 2) == 0)
		{
			parsing_data->map->ceil_color = parse_color(*buffer_split + 2);
			if (parsing_data->map->ceil_color == -1)
				return (1);
			parsing_data->ceiling_color++;
		}
		if (ft_strncmp(*buffer_split, "F ", 2) == 0)
		{
			parsing_data->map->floor_color = parse_color(*buffer_split + 2);
			if (parsing_data->map->floor_color == -1)
				return (1);
			parsing_data->floor_color++;
		}
		buffer_split++;
	}
	if (parsing_data->ceiling_color != 1 || parsing_data->floor_color != 1)
		return (1);
	return (0);
}

int	parse_int(char *line)
{
	int	res;

	res = 0;
	while ((*line) >= '0' && (*line) <= '9')
	{
		res = (res * 10) + (*line - '0');
		line++;
	}
	if (*line != '\0')
		return (-1);
	if (res < 0 || res > 255)
		return (-1);
	return (res);
}

int	parse_color(char *line)
{
	char	**split;
	int		rgb;
	int		r;
	int		g;
	int		b;

	split = ft_split(line, ',');
	if (!split || !split[0] || !split[1] || !split[2] || split[3])
		return (ft_array_free(split), -1);
	r = parse_int(split[0]);
	g = parse_int(split[1]);
	b = parse_int(split[2]);
	if (r == -1 || g == -1 || b == -1)
		return (ft_array_free(split), -1);
	rgb = ((0xFF & 0xff) << 24) | ((r & 0xff) << 16)
		| ((g & 0xff) << 8) | (b & 0xff);
	return (ft_array_free(split), rgb);
}
