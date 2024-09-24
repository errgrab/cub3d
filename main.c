/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:54:28 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/09/24 18:33:45 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <X11/X.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>
#include "mlx.h"

/* Custom Allocation:
 *
 * void *ft_get_alloc(); // internal
 * void *ft_calloc();
 * void *ft_free();
 * void *ft_clean();
 *
 */

typedef struct s_list t_list;
struct s_list
{
	void	*ptr;
	t_list	*next;
};

t_list **ft_get_alloc()
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

typedef struct s_mlx t_mlx;
struct s_mlx
{
	void	*mlx;
	void	*win;
};

t_mlx	*get_mlx()
{
	static t_mlx	mlx;
	return (&mlx);
}

int	quit()
{
	auto t_mlx *mlx = get_mlx();
	mlx_destroy_window(mlx->mlx, mlx->win);
	mlx_destroy_display(mlx->mlx);
	free(mlx->mlx);
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

int main(void)
{
	auto t_mlx *mlx = get_mlx();
	mlx->mlx = mlx_init();
	mlx->win = mlx_new_window(mlx->mlx, 500, 500, "title");
	mlx_hook(mlx->win, KeyRelease, KeyReleaseMask, keyrelease, NULL);
	mlx_hook(mlx->win, DestroyNotify, StructureNotifyMask, quit, NULL);
	mlx_loop_hook(mlx->mlx, loop, NULL);
	mlx_loop(mlx->mlx);
	return (0);
}
