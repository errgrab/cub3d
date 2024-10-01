/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:54:28 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/01 18:51:48 by ecarvalh         ###   ########.fr       */
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
#include <math.h>       // (almost all of them)
#include "mlx.h"        // mlx_*

#include "cub3d.h"

t_global	*global(void)
{
	static t_global	global;

	return (&global);
}

int	quit(void)
{
	auto t_mlx * mlx = &global()->mlx;
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
	auto t_time * time = &global()->time;
	time->now = time_now();
	time->dt = time->now - time->before;
	if (time->dt > 0)
		time->fps = 1000 / time->dt;
	time->before = time->now;
}

size_t	_tern(int cond, size_t t_val, size_t f_val)
{
	if (cond)
		return (t_val);
	return (f_val);
}

void	_xorswapi(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

void	put_pixel(int x, int y, t_color color)
{
	auto t_img * img = &global()->mlx.frame;
	if (x >= 0 && x < img->width && y >= 0 && y < img->height)
	{
		auto char *dst = img->buf + (y * img->sl + x * (img->bpp / 8));
		*(t_color *)dst = color;
	}
}

void	clear(t_color color)
{
	auto t_img * img = &global()->mlx.frame;
	auto int x = -1;
	while (++x < img->width)
	{
		auto int y = 0;
		while (y < img->height)
			put_pixel(x, y++, color);
	}
}

void	draw_verline(int x, int ystart, int yend, t_color color)
{
	if (ystart > yend)
		_xorswapi(&ystart, &yend);
	while (ystart < yend)
		put_pixel(x, ystart++, color);
}

int	sign(float v)
{
	if (v < 0)
		return (-1);
	else if (v > 0)
		return (1);
	else
		return (0);
}

int	loop(void)
{
	time_update();
	auto t_usr * usr = &global()->usr;
	auto t_map * map = &global()->map;
	auto t_mlx * mlx = &global()->mlx;
	auto t_img * img = &global()->mlx.frame;
	for (int x = 0; x < img->width; x++)
	{
		auto t_dda dda = {0};
		// normalized [-1, 1] camera vector
		dda.xc = (2 * (x / (float)(img->width))) - 1;
		dda.dir = {
			usr->dir.x + usr->plane.x * dda.xc,
			usr->dir.y + usr->plane.y * dda.xc};
		dda.pos = usr->pos;
		dda.ipos = {(int)dda.pos.x, (int)dda.pos.y};
		if (fabsf(dda.dir.x) < 1e-20)
			dda.dd.x = 1e30;
		else
			dda.dd.x = fabsf(1/dda.dir.x);
		if (fabsf(dda.dir.y) < 1e-20)
			dda.dd.y = 1e30;
		else
			dda.dd.y = fabsf(1/dda.dir.y);
		dda.sd = dda.dd;
		if (dda.dir.x < 0)
			dda.sd.x *= dda.pos.x - dda.ipos.x;
		else
			dda.sd.x *= dda.ipos.x + 1 - dda.pos.x;
		if (dda.dir.y < 0)
			dda.sd.y *= dda.pos.y - dda.ipos.y;
		else
			dda.sd.y *= dda.ipos.y + 1 - dda.pos.y;
		dda.step = {sign(dda.dir.x), sign(dda.dir.y)};
		auto struct {int val, isy; t_v2f pos;} hit = { false, false, {0, 0}};
		while (!hit.val)
		{
			if (dda.sd.x < dda.sd.y)
			{
				dda.sd.x += dda.dd.x;
				dda.ipos.x += dda.step.x;
				hit.isy = false;
			}
			else
			{
				dda.sd.y += dda.dd.y;
				dda.ipos.y += dda.step.y;
				hit.isy = true;
			}
			hit.val = map->data[ipos.y * map->width + ipos.x];
		}
		t_color color;
		switch (hit.val) {
		case 1: color = CW; break;
		case 2: color = CR; break;
		case 3: color = CB; break;
		case 4: color = CM; break;
		}
		if (hit.isy)
			color = 0xff000000 & ((color & 0xffffff) * 0xc0c0c0);
	}
	mlx_put_image_to_window(mlx->ptr, mlx->win, img->ptr, 0, 0);
	return (0);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	if (!dest || !src)
		return (NULL);
	if (dest == src)
		return (dest);
	auto char *d = dest;
	auto const char *s = src;
	if (d < s)
		while (n--)
			*d++ = *s++;
	else
		while (n--)
			d[n] = s[n];
	return (dest);
}

void	init_map_tmp(void)
{
	auto t_time * time = &global()->time;
	auto t_map * map = &global()->map;
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
	auto int map_size = sizeof(map) / sizeof(int);
	map->data = ft_calloc(map_size, sizeof(int));
	map->width = map_width;
	map->height = map_size / map_width;
	if (!map->data)
		quit();
	ft_memmove(map->data, map_data, map_size);
}

void	init_usr_tmp(void)
{
	auto t_usr * usr = &global()->usr;
	usr->pos = (t_v2f){6, 6};
	usr->dir = (t_v2f){-1, 0};
	usr->plane = (t_v2f){0, 0.66};
}

void	init(void)
{
	auto t_mlx * mlx = &global()->mlx;
	auto t_img * img = &mlx->frame;
	img->width = 640;
	img->height = 480;
	mlx->ptr = mlx_init();
	mlx->win = mlx_new_window(mlx->ptr, img->width, img->height, "");
	img->ptr = mlx_new_image(mlx->ptr, img->width, img->height);
	if (!mlx->ptr || !mlx->win || !img->ptr)
		quit();
	img->buf = mlx_get_data_addr(img->ptr, &img->bpp, &img->sl, &img->endian);
	mlx_hook(mlx->win, KeyRelease, KeyReleaseMask, keyrelease, NULL);
	mlx_hook(mlx->win, DestroyNotify, StructureNotifyMask, quit, NULL);
	mlx_loop_hook(mlx->ptr, loop, NULL);
	mlx_loop(mlx->ptr);
	init_usr_tmp();
	init_map_tmp();
}

int	main(void)
{
	init();
	return (0);
}
