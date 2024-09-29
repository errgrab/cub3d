/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:54:28 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/09/29 19:01:01 by ecarvalh         ###   ########.fr       */
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

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

typedef struct s_list t_list;
struct s_list
{
	void	*ptr;
	t_list	*next;
};

typedef struct s_mlx t_mlx;
struct s_mlx
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*buf;
	int		endian;
};

typedef struct s_map t_map;
struct s_map
{
	int		*data;
	size_t	size;
	size_t	width;
	bool	is_valid;
};

typedef struct s_v2f t_v2f;
struct s_v2f
{
	float	x;
	float	y;
};

typedef struct s_pl t_pl;
struct s_pl
{
	t_v2f	pos;
	t_v2f	dir;
	t_v2f	plane;
};

typedef struct s_glb t_glb;
struct s_glb
{
	t_mlx	mlx;
	t_map	map;
	t_pl	player;
	size_t	time_last;
	size_t	time_now;
	float	dt;
	size_t	fps;
};

//// alloc.c ///////////////////////////////////////////////////////////////////

/* Custom Allocator:
 *
 * void *ft_get_alloc(); // internal
 * void *ft_calloc(size_t num, size_t size); // simple calloc
 * void ft_free(void *ptr); // simple free
 * void ft_clean(); // clear everything
 */

static t_list **ft_get_alloc()
{
	static t_list	*alloc;

	return (&alloc);
}

void	*ft_calloc(size_t num, size_t size)
{
	auto void *ptr = malloc(num * size);
	if (ptr)
	{
		auto t_list **root = ft_get_alloc();
		auto size_t i = num * size;
		while (i && i--)
			((char*)ptr)[i] = 0;
		t_list *new_node = (t_list *)malloc(sizeof(t_list));
		if (new_node)
		{
			new_node->ptr = ptr;
			new_node->next = *root;
			*root = new_node;
		} else
			return (free(ptr), NULL);
	}
	return (ptr);
}

void	ft_free(void *ptr)
{
	auto t_list **node = ft_get_alloc();
	auto t_list *prev = NULL;
	while (*node)
	{
		if ((*node)->ptr == ptr)
		{
			auto t_list *to_free = *node;
			if (prev)
				prev->next = (*node)->next;
			else
				*node = (*node)->next;
			free(to_free->ptr);
			free(to_free);
			return ;
		}
		prev = *node;
		node = &(*node)->next;
	}
}

void	ft_clean() {
	auto t_list **node = ft_get_alloc();
	while (*node)
	{
		auto t_list *to_free = *node;
		*node = (*node)->next;
		free(to_free->ptr);
		free(to_free);
	}
}

//// main.c ////////////////////////////////////////////////////////////////////

t_glb	*get_glb(void)
{
	static t_glb	glb;

	return (&glb);
}

int	quit(void)
{
	auto t_mlx *mlx = &get_glb()->mlx;
	if (mlx->win)
		mlx_destroy_window(mlx->mlx, mlx->win);
	if (mlx->img)
		mlx_destroy_image(mlx->mlx, mlx->img);
	if (mlx->mlx)
	{
		mlx_destroy_display(mlx->mlx);
		free(mlx->mlx);
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

void	time_set(void)
{
	auto t_glb *glb = get_glb();
	glb->time_now = time_now();
	glb->dt = glb->time_now - glb->time_last;
	if (glb->dt > 0)
		glb->fps = 1000 / glb->dt;
	glb->time_last = glb->time_now;
}

size_t	_tern(int cond, size_t t_val, size_t f_val)
{
	if (cond)
		return (t_val);
	return (f_val);
}

int	loop(void)
{
	time_set();
	auto t_glb *glb = get_glb();
	for (int x = 0; x < WINDOW_WIDTH; x++)
	{
		float cx = 2 * x / (float)WINDOW_WIDTH - 1;
		t_v2f rdir = (t_v2f){
			glb->player.dir.x + glb->player.plane.x * cx,
			glb->player.dir.y + glb->player.plane.y * cx };
		t_v2f mapp = (t_v2f){
			(int)glb->player.pos.x,
			(int)glb->player.pos.y };
		t_v2f ddis = (t_v2f){
			_tern(rdir.x == 0, 1e30, fabsf(1 / rdir.x)),
			_tern(rdir.y == 0, 1e30, fabsf(1 / rdir.y)) };
		bool hit = false;
		bool isX = false;
		t_v2f step;
		t_v2f sidd;
		if (rdir.x < 0)
		{
			// SOCORRO EU NN SEI MAIS OQ TO FAZENDO
		}
	}
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
	auto t_glb *glb = get_glb();
	glb->time_last = time_now();
	auto int map_width = 8;
	auto int map[] = {
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 1, 0, 0, 1,
		1, 0, 0, 1, 1, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
	};
	auto int map_size = sizeof(map) / sizeof(int);
	glb->map.data = ft_calloc(map_size, sizeof(int));
	glb->map.size = map_size;
	glb->map.width = map_width;
	glb->map.is_valid = true;
	if (!glb->map.data)
		quit();
	ft_memmove(glb->map.data, map, map_size);
}

void	init_player(void)
{
	auto t_glb *glb = get_glb();
	glb->player.pos = (t_v2f){ 6, 6 };
	glb->player.dir = (t_v2f){ -1, 0 };
	glb->player.plane = (t_v2f){ 0, 0.66 };
}

void	init(void)
{
	auto t_mlx *mlx = &get_glb()->mlx;
	mlx->mlx = mlx_init();
	mlx->win = mlx_new_window(mlx->mlx, 800, 500, "");
	mlx->img = mlx_new_image(mlx->mlx, 800, 500);
	auto int d = 0;
	mlx->buf = mlx_get_data_addr(mlx->img, &d, &d, &mlx->endian);
	if (!mlx->win || !mlx->img)
		quit();
	mlx_hook(mlx->win, KeyRelease, KeyReleaseMask, keyrelease, NULL);
	mlx_hook(mlx->win, DestroyNotify, StructureNotifyMask, quit, NULL);
	mlx_loop_hook(mlx->mlx, loop, NULL);
	mlx_loop(mlx->mlx);
}

int	main(void)
{
	init_map_tmp();
	init();
	return (0);
}
