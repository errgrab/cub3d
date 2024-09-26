/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:54:28 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/09/26 18:36:11 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//// Headers ///////////////////////////////////////////////////////////////////

#include <X11/X.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#include "mlx.h"

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

typedef struct s_game t_game;
struct s_game
{
	int	*map;
	size_t	last_time;
};

typedef struct s_glb t_glb;
struct s_glb
{
	t_mlx	mlx;
	t_game	game;
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
		perror("Err: gettimeofday failed!");
		quit();
	}
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}


int	loop(void)
{
	auto t_game *game = &get_glb()->game;
	auto size_t now = time_now();
	auto size_t time_passed = now - game->last_time;
	if (time_passed > 0)
	{
		auto size_t fps = 1000 / time_passed;
		printf("fps: %zu\n", fps);
	}
	game->last_time = now;
	return (0);
}

void	init_map_tmp(void)
{
	auto	t_game *game = &get_glb()->game;
	game->last_time = time_now();
	game->map = (int []){
		1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,1,
		1,0,0,1,1,1,1,0,0,1,
		1,0,0,0,0,0,1,0,0,1,
		1,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,
	};
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
