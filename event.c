/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:28:48 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/19 17:10:42 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	event_quit(void);
int	event_keydown(int keycode);
int	event_keyup(int keycode);

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
	if (keycode <= 177)
		g()->key_pressed[keycode & 0xff] = 1;
	if (keycode == LEFT_ARROW)
		g()->key_pressed[14] = 1;
	if (keycode == RIGHT_ARROW)
		g()->key_pressed[15] = 1;
	return (0);
}

int	event_keyup(int keycode)
{
	if (keycode <= 177)
		g()->key_pressed[keycode & 0xff] = 0;
	if (keycode == 65307)
		event_quit();
	if (keycode == LEFT_ARROW)
		g()->key_pressed[14] = 0;
	if (keycode == RIGHT_ARROW)
		g()->key_pressed[15] = 0;
	return (0);
}
