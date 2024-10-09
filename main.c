/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:54:28 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/10 00:35:03 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//// Headers ///////////////////////////////////////////////////////////////////

#include <X11/X.h>      // macros
#include <X11/keysym.h> // macros _keyboard_
#include <stdlib.h>     // malloc, free, size_t, exit, etc...
#include <stdio.h>      // printf
#include <sys/time.h>   // gettimeofday
#include <errno.h>      // perror(3)
#include <stdbool.h>    // bool type
#define __USE_MISC
#include <math.h>       // (almost all of them)
#include "mlx.h"        // mlx_*

#include "cub3d.h"

t_global	*g(void)
{
	static t_global	g;

	return (&g);
}

int	quit(void)
{
	auto t_mlx * mlx = &g()->mlx;
	if (mlx->win)
		mlx_destroy_window(mlx->ptr, mlx->win);
	auto t_img * img = &mlx->frame;
	if (img->ptr)
		mlx_destroy_image(mlx->ptr, img->ptr);
	if (mlx->ptr)
	{
		mlx_destroy_display(mlx->ptr);
		free(mlx->ptr);
	}
	ft_clean();
	exit(0);
	return (0);
}

int	keyrelease(int key)
{
	if (key == XK_Escape)
		quit();
	return (0);
}

int	mouse_move(int x, int y)
{
	t_usr	*usr;

	usr = &g()->usr;
	usr->pos.x = x;
	usr->pos.y = y;
	return (0);
}

size_t	time_now(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
	{
		perror("time_now - gettimeofday");
		quit();
	}
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	time_update(void)
{
	auto t_time * time = &g()->time;
	time->now = time_now();
	time->dt = time->now - time->before;
	if (time->dt > 0)
		time->fps = 1000 / time->dt;
	time->before = time->now;
}

void	put_pixel(int x, int y, t_color color)
{
	auto t_img * img = &g()->mlx.frame;
	if (x >= 0 && x < img->width && y >= 0 && y < img->height)
	{
		auto char *dst = img->buf + (y * img->sl + x * (img->bpp / 8));
		*(t_color *)dst = color;
	}
}

void	clear(t_color color)
{
	auto t_img * img = &g()->mlx.frame;
	auto int x = -1;
	while (++x < img->width)
	{
		auto int y = 0;
		while (y < img->height)
			put_pixel(x, y++, color);
	}
}

void	ft_xorswapi(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

void	draw_verline(int x, int ystart, int yend, t_color color)
{
	t_img	*win;

	win = &g()->mlx.frame;
	if (ystart > yend)
		ft_xorswapi(&ystart, &yend);
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

void	draw_hozline(int y, int xstart, int xend, t_color color)
{
	t_img	*win;

	win = &g()->mlx.frame;
	if (xstart > xend)
		ft_xorswapi(&xstart, &xend);
	if (xstart < 0)
		xstart = 0;
	else if (xstart >= win->width)
		xstart = win->width;
	if (xend < 0)
		xend = 0;
	else if (xend >= win->width)
		xend = win->width;
	while (xstart < xend)
		put_pixel(xstart++, y, color);
}

void	draw_circle(int x, int y, int radius, t_color color)
{
	for (int rad = radius; rad >= 0; rad--)
	{
		for (double i = 0; i <= M_PI * 2; i+=0.01)
		{
			int px = x + rad * cos(i);
			int py = y + rad * sin(i);
			put_pixel(px, py, color);
		}
	}
}

void	draw_rect(int x, int y, int width, int height, t_color color)
{
	int	tx;
	int	ty;

	tx = x;
	ty = y;
	while (ty < height)
	{
		while (tx < width)
		{
			put_pixel(tx, ty, color);
			tx++;
		}
		tx = x;
		ty++;
	}
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

t_v2	v2_add(t_v2 a, t_v2 b)
{
	t_v2	res;

	res.x = a.x + b.x;
	res.y = a.y + b.y;
	return (res);
}

t_v2	v2_sub(t_v2 a, t_v2 b)
{
	t_v2	res;

	res.x = a.x - b.x;
	res.y = a.y - b.y;
	return (res);
}

t_v2	v2_mul(t_v2 a, t_v2 b)
{
	t_v2	res;

	res.x = a.x * b.x;
	res.y = a.y * b.y;
	return (res);
}

t_v2	v2_div(t_v2 a, t_v2 b)
{
	t_v2	res;

	res.x = a.x / b.x;
	res.y = a.y / b.y;
	return (res);
}

float	v2_len(t_v2 a)
{
	return (sqrt(a.x * a.x + a.y * a.y));
}

t_v2	v2_norm(t_v2 a)
{
	const float	len = v2_len(a);
	t_v2		res;

	res.x = 0;
	res.y = 0;
	if (len == 0)
		return (res);
	res.x = a.x / len;
	res.y = a.y / len;
	return (res);
}

t_v2	v2_scale(t_v2 a, float val)
{
	t_v2	res;
	
	res.x = a.x * val;
	res.y = a.y * val;
	return (res);
}

float	v2_dist(t_v2 a, t_v2 b)
{
	return (v2_len(v2_sub(a, b)));
}

// snap X to grid based on delta X
float	v2_snap(float x, float dx)
{
	if (dx > 0)
		return (ceil(x));
	if (dx < 0)
		return (floor(x));
	return (x);
}

t_v2	ray_step(t_v2 a, t_v2 b)
{
	(void)a;
	return (b);
}

t_v2	v2_window_size()
{
	t_img	*window;
	t_v2	res;

	window = &g()->mlx.frame;
	res.x = (float)window->width;
	res.y = (float)window->height;
	return (res);
}

void	draw_grid(float x, float y, float sz)
{
	t_map	*map;
	t_color	color;
	int		data;

	map = &g()->map;
	for (size_t i = 0; i <= map->width; i++)
	{
		for (size_t j = 0; j <= map->height; j++)
		{
			color = 0;
			draw_verline(x + i * sz, y + j, y + j * sz, CW);
			draw_hozline(y + j * sz, x + i, x + i * sz, CW);
			if (i < map->width && j < map->height)
			{
				data = map->data[j * map->width + i];
				if (data == 1)
					color = CW;
				else if (data == 2)
					color = CR;
				else if (data == 3)
					color = CM;
				else if (data == 4)
					color = CC;
				if (data)
					draw_rect(x + i * sz,  y + j * sz, x + i * sz + sz, y + j * sz + sz, color);
			}
		}
	}
}

int	loop(void)
{
	t_mlx	*mlx;
	t_img	*img;
	t_usr	*usr;

	time_update();
	mlx = &g()->mlx;
	img = &mlx->frame;
	usr = &g()->usr;
	draw_circle(usr->pos.x, usr->pos.y, 10, CW);
	draw_grid(10, 10, 80);
	//draw_circle(10 + usr->pos.x * 80, 10 + usr->pos.y * 80, 10, CW);
	mlx_put_image_to_window(mlx->ptr, mlx->win, img->ptr, 0, 0);
	return (0);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;

	if (!dest || !src)
		return (NULL);
	if (dest == src)
		return (dest);
	d = (char *)dest;
	s = (char *)src;
	if (d < s)
	{
		while (n--)
			*d++ = *s++;
	}
	else
	{
		while (n--)
			d[n] = s[n];
	}
	return (dest);
}

void	init_map_tmp(void)
{
	auto t_time * time = &g()->time;
	auto t_map * map = &g()->map;
	time->before = time_now();
	auto int map_width = 8;
	auto int map_data[] = {
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 2, 3, 0, 0, 1,
		1, 0, 0, 4, 2, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
	};
	auto int map_size = sizeof(map_data) / sizeof(int);
	map->data = ft_calloc(map_size, sizeof(int));
	map->width = map_width;
	map->height = map_width;
	if (!map->data)
		quit();
	ft_memmove(map->data, map_data, map_size * sizeof(int));
	for (size_t x = 0; x < map->width; x++)
	{
		for (size_t y = 0; y < map->height; y++)
		{
			printf("%d ", map->data[y * map_width + x]);
		}
		printf("\n");
	}
}

void	init_usr_tmp(void)
{
	auto t_usr * usr = &g()->usr;
	usr->pos = (t_v2){6.5, 6.5};
	usr->dir = (t_v2){-1, 0};
	// usr->plane = (t_v2){0, 0.66};
}

void	init(void)
{
	init_usr_tmp();
	init_map_tmp();
	auto t_mlx * mlx = &g()->mlx;
	auto t_img * img = &mlx->frame;
	img->width = 800;
	img->height = 800;
	mlx->ptr = mlx_init();
	mlx->win = mlx_new_window(mlx->ptr, img->width, img->height, "");
	img->ptr = mlx_new_image(mlx->ptr, img->width, img->height);
	if (!mlx->ptr || !mlx->win || !img->ptr)
		quit();
	img->buf = mlx_get_data_addr(img->ptr, &img->bpp, &img->sl, &img->endian);
	mlx_hook(mlx->win, KeyRelease, KeyReleaseMask, keyrelease, NULL);
	mlx_hook(mlx->win, DestroyNotify, StructureNotifyMask, quit, NULL);
	mlx_hook(mlx->win, MotionNotify, PointerMotionMask, mouse_move, NULL);
	mlx_loop_hook(mlx->ptr, loop, NULL);
	mlx_loop(mlx->ptr);
}

int	main(void)
{
	init();
	return (0);
}
