/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:54:28 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/09/25 23:33:03 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//// Headers ///////////////////////////////////////////////////////////////////

#include <X11/X.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>
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
	auto void	*ptr = malloc(num * size);
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
	auto t_list	**node = ft_get_alloc();
	auto t_list *prev = NULL;
	while (*node)
	{
		if ((*node)->ptr == ptr)
		{
			auto t_list	*to_free = *node;
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
	auto t_list	**node = ft_get_alloc();
	while (*node)
	{
		auto t_list	*to_free = *node;
		*node = (*node)->next;
		free(to_free->ptr);
		free(to_free);
	}
}

//// main.c ////////////////////////////////////////////////////////////////////

t_mlx	*get_mlx()
{
	static t_mlx	mlx;

	return (&mlx);
}

int	quit()
{
	auto t_mlx *mlx = get_mlx();
	if (mlx->win)
		mlx_destroy_window(mlx->mlx, mlx->win);
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

int	loop()
{
	return (0);
}

void	init()
{
	auto t_mlx *mlx = get_mlx();
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
	init();
	return (0);
}
