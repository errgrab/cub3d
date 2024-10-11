/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:54:28 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/11 12:55:45 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CUB3D_IMPL
#define ALLOC_IMPL
#include "cub3d.h"

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
	mlx->height = 800;
	mlx->title = "Cub3D";
	mlx->win = mlx_new_window(mlx->ptr, mlx->width, mlx->height, mlx->title);
}

int	event_quit(void)
{
	t_mlx	*mlx;
	t_img	*img;

	mlx = &g()->mlx;
	img = &g()->frame;
	if (mlx->win)
		mlx_destroy_window(mlx->ptr, mlx->win);
	if (img->ptr)
		mlx_destroy_image(mlx->ptr, img->ptr);
	if (mlx->ptr)
		free(mlx->ptr);
	ft_clean();
	exit(g()->exit_status);
	return (0);
}

int	event_keydown(int keycode)
{
//	printf("keydown: ");
	if (keycode <= 177)
	{
		g()->key_pressed[keycode & 0xff] = 1;
//		printf("(%c) ", keycode & 0xff);
	}
//	printf("%d\n", keycode);
	return (0);
}

int	event_keyup(int keycode)
{
	if (keycode <= 177)
		g()->key_pressed[keycode & 0xff] = 0;
	if (keycode == 65307)
		event_quit();
	return (0);
}

void	put_pixel(int x, int y, int color)
{
	t_img	*img;
	char	*dst;

	img = &g()->frame;
	if (x >= 0 && x < img->width && y >= 0 && y < img->height)
	{
		dst = img->data + (y * img->sl + x * (img->bpp / 8));
		*(int *)dst = color;
	}
}

void	draw_vertical_line(int x, int ystart, int yend, int color)
{
	t_img	*win;

	win = &g()->frame;
	if (ystart > yend)
	{
		ystart ^= yend;
		yend ^= ystart;
		ystart ^= yend;
	}
	if (ystart < 0)
		ystart = 0;
	else if (ystart >= win->height)
		ystart = win->height;
	if (yend < 0)
		yend = 0;
	else if (yend >= win->height)
		yend = win->height;
	while (ystart < yend)
		put_pixel(x, ystart++, color);
}

int	get_map_value(int x, int y)
{
	if (x < 0 || x > g()->map.width || y < 0 || y > g()->map.height)
		return (-1);
	return (g()->map.data[y * g()->map.width + x]);
}

void perform_raycast(int screenHeight) {
	for (int x = 0; x < g()->frame.width; x++) {
		double cameraX = 2 * x / (double)g()->frame.width - 1;
		double rayDirX = g()->usr.dirx + g()->usr.plx * cameraX;
		double rayDirY = g()->usr.diry + g()->usr.ply * cameraX;
		int mapX = (int)g()->usr.posx;
		int mapY = (int)g()->usr.posy;
		double deltaDistX = fabs(1 / rayDirX);
		double deltaDistY = fabs(1 / rayDirY);
		int stepX, stepY;
		double sideDistX, sideDistY;
		if (rayDirX < 0) {
			stepX = -1;
			sideDistX = (g()->usr.posx - mapX) * deltaDistX;
		} else {
			stepX = 1;
			sideDistX = (mapX + 1.0 - g()->usr.posx) * deltaDistX;
		}
		if (rayDirY < 0) {
			stepY = -1;
			sideDistY = (g()->usr.posy - mapY) * deltaDistY;
		} else {
			stepY = 1;
			sideDistY = (mapY + 1.0 - g()->usr.posy) * deltaDistY;
		}
		int hit = 0;
		int side;
		while (hit == 0) {
			if (sideDistX < sideDistY) {
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			} else {
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}

			if (get_map_value(mapX, mapY) > 0) hit = 1;
			else if (get_map_value(mapX, mapY) < 0) break;
		}
		double perpWallDist;
		if (side == 0) perpWallDist = (mapX - g()->usr.posx + (1 - stepX) / 2) / rayDirX;
		else perpWallDist = (mapY - g()->usr.posy + (1 - stepY) / 2) / rayDirY;
		int lineHeight = (int)(screenHeight / perpWallDist);
		int drawStart = -lineHeight / 2 + screenHeight / 2;
		if (drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + screenHeight / 2;
		if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;
		if (side == 0) {
			draw_vertical_line(x, 0, drawStart, 0);
			draw_vertical_line(x, drawStart, drawEnd, 0xFFFFFF);
			draw_vertical_line(x, drawEnd, screenHeight, 0);
		} else {
			draw_vertical_line(x, 0, drawStart, 0);
			draw_vertical_line(x, drawStart, drawEnd, 0xAAAAAA);
			draw_vertical_line(x, drawEnd, screenHeight, 0);
		}
	}
	mlx_put_image_to_window(g()->mlx.ptr, g()->mlx.win, g()->frame.ptr, 0, 0);
}

int	loop(void)
{
	float ver_dir = g()->key_pressed['w'] - g()->key_pressed['s'];
	float rot_dir = g()->key_pressed['e'] - g()->key_pressed['q'];
	time_update();
	printf("FPS: %f\n", g()->time.fps);
	perform_raycast(800);
	g()->usr.posx += ver_dir * 3 * g()->usr.dirx * g()->time.dt;
	g()->usr.posy += ver_dir * 3 * g()->usr.diry * g()->time.dt;
	float rotvel = rot_dir * 3 * g()->time.dt;
	float odx = g()->usr.dirx;
	g()->usr.dirx = odx * cos(rotvel) - g()->usr.diry * sin(rotvel);
	g()->usr.diry = odx * sin(rotvel) + g()->usr.diry * cos(rotvel);
	float opx = g()->usr.plx;
	g()->usr.plx = opx * cos(rotvel) - g()->usr.ply * sin(rotvel);
	g()->usr.ply = opx * sin(rotvel) + g()->usr.ply * cos(rotvel);
	return (0);
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
	_memmove(g()->map.data, map_tmp, sizeof(map_tmp));
	return (1);
}

// mlx_do_key_autorepeatoff(mlx->ptr); //> TODO: test it later
int	main(int argc, char **argv)
{
	t_mlx	*mlx;

	g()->argc = argc;
	g()->argv = argv;
	if (!init_map_tmp())
		return (write(2, "Error: Could not initialize Map!\n", 33), 1);
	mlx = &g()->mlx;
	mlx->ptr = mlx_init();
	if (!mlx->ptr)
		return (write(2, "Error: Could not initialize MLX!\n", 33), 1);
	init_window();
	init_frame();
	if (!mlx->win || !g()->frame.ptr)
		return (write(2, "Error: Could not initialize window!\n", 36), 1);
	mlx_hook(mlx->win, ON_DESTROY, 0, event_quit, NULL);
	mlx_hook(mlx->win, ON_KEYDOWN, 1, event_keydown, NULL);
	mlx_hook(mlx->win, ON_KEYUP, 2, event_keyup, NULL);
	mlx_loop_hook(mlx->ptr, loop, NULL);
	mlx_loop(mlx->ptr);
	return (0);
}
