/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:26:55 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/26 19:09:16 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_frame(void);
void	init_window(void);
int		init_map_tmp(void);

void	init_frame(void)
{
	t_mlx	*mlx;
	t_img	*img;

	mlx = &g()->mlx;
	img = &g()->frame;
	img->width = mlx->width;
	img->height = mlx->height;
	img->ptr = mlx_new_image(mlx->ptr, img->width, img->height);
	img->data = mlx_get_data_addr(img->ptr, &img->bpp, &img->sl, &img->endian);
}

void	init_window(void)
{
	t_mlx	*mlx;

	mlx = &g()->mlx;
	mlx->width = 800;
	mlx->height = 600;
	mlx->title = "Cub3D";
	mlx->win = mlx_new_window(mlx->ptr, mlx->width, mlx->height, mlx->title);
}

void	load_img(char *path, t_img *img)
{
	img->ptr = NULL;
	if (ft_strstr(path, ".xpm"))
		img->ptr = mlx_xpm_file_to_image(g()->mlx.ptr, path, &img->width,
				&img->height);
	else
		write(2, "Error: Image type not implemented!\n", 35);
	if (!img->ptr)
	{
		write(2, "Error: Unable to load image!\n", 29);
		event_quit();
	}
	img->data = mlx_get_data_addr(img->ptr, &img->bpp, &img->sl, &img->endian);
}

int	init_map_tmp(void)
{
	const int		size = 8;
	const int		map_tmp[] = {
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 0, 0, 1,
		1, 0, 0, 1, 1, 1, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1};
	const size_t	map_size = sizeof(map_tmp) / sizeof(int);
	int *const		map_data = ft_calloc(map_size, sizeof(int));

	if (!map_data)
		return (0);
	g()->map.data = map_data;
	g()->map.width = size;
	g()->map.height = size;
	g()->usr.posx = 5.5;
	g()->usr.posy = 5.5;
	g()->usr.dirx = 1.0;
	g()->usr.ply = 0.66;
	load_img("north.xpm", &g()->map.north);
	load_img("north.xpm", &g()->map.south);
	load_img("south.xpm", &g()->map.east);
	load_img("south.xpm", &g()->map.west);
	ft_memmove(g()->map.data, map_tmp, sizeof(map_tmp));
	return (1);
}
