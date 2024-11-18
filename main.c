/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:54:28 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/25 01:43:16 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_map_value(int x, int y)
{
	if (x < 0 || x >= g()->map.width || y < 0 || y >= g()->map.height)
		return (-1);
	return (g()->map.data[y * g()->map.width + x]);
}

int	loop(void)
{
	time_update();
	if (g()->debug)
		printf("FPS: %f\r", g()->time.fps);
	raycast();
	move(SPEED);
	rotate(ROT_SPEED);
	mlx_put_image_to_window(g()->mlx.ptr, g()->mlx.win, g()->frame.ptr, 0, 0);
	return (0);
}

// mlx_do_key_autorepeatoff(mlx->ptr); //> TODO: test it later
int	main(int argc, char **argv)
{
	g()->argc = argc;
	g()->argv = argv;
	g()->debug = 1;
	g()->map.ceil_color = 0xff3322dd;
	g()->map.floor_color = 0xff333333;
	if (parsing())
		return (1);

	printf("usr.dirx: %f\n", g()->usr.dirx);
	printf("usr.diry: %f\n", g()->usr.diry);
	printf("usr.ply: %f\n", g()->usr.ply);
	printf("usr.plx: %f\n", g()->usr.plx);

	mlx_hook(g()->mlx.win, ON_DESTROY, 0, event_quit, NULL);
	mlx_hook(g()->mlx.win, ON_KEYDOWN, 1, event_keydown, NULL);
	mlx_hook(g()->mlx.win, ON_KEYUP, 2, event_keyup, NULL);
	mlx_loop_hook(g()->mlx.ptr, loop, NULL);
	mlx_loop(g()->mlx.ptr);
	return (0);
}
