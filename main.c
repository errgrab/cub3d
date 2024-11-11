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

void	debug(void);

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
	// DEBUG
	debug();

	mlx_hook(g()->mlx.win, ON_DESTROY, 0, event_quit, NULL);
	mlx_hook(g()->mlx.win, ON_KEYDOWN, 1, event_keydown, NULL);
	mlx_hook(g()->mlx.win, ON_KEYUP, 2, event_keyup, NULL);
	mlx_loop_hook(g()->mlx.ptr, loop, NULL);
	mlx_loop(g()->mlx.ptr);
	return (0);
}

void debug(void)
{
	printf("DEBUG\n");
	printf("Ceiling color: A=%d, R=%d, G=%d, B=%d\n",
		   (g()->map.ceil_color >> 24) & 0xFF,
		   (g()->map.ceil_color >> 16) & 0xFF,
		   (g()->map.ceil_color >> 8) & 0xFF,
		   (g()->map.ceil_color) & 0xFF);

	printf("Floor color: A=%d, R=%d, G=%d, B=%d\n",
		   (g()->map.floor_color >> 24) & 0xFF,
		   (g()->map.floor_color >> 16) & 0xFF,
		   (g()->map.floor_color >> 8) & 0xFF,
		   (g()->map.floor_color) & 0xFF);

	printf("Map size: %dx%d\n", g()->map.width, g()->map.height);
	printf("Player position: x=%f, y=%f\n", g()->usr.posx, g()->usr.posy);
	printf("Player direction: x=%f, y=%f\n", g()->usr.dirx, g()->usr.diry);
	printf("Player plane: %f\n", g()->usr.ply);

	printf("\nmap:\n");
	int i = -1;
	int j;
	while (++i < g()->map.height)
	{
		j = -1;
		while (++j < g()->map.width)
			printf("%d", g()->map.data[i * g()->map.width + j]);
		printf("\n");
	}

	void *win = mlx_new_window(g()->mlx.ptr, 1200, 1200, "Cub3D");
	sleep(1);
	mlx_put_image_to_window(g()->mlx.ptr, win, g()->map.north.ptr, 0, 0);
	sleep(1);
	mlx_put_image_to_window(g()->mlx.ptr, win, g()->map.south.ptr, 25, 25);
	sleep(1);
	mlx_put_image_to_window(g()->mlx.ptr, win, g()->map.east.ptr, 50, 50);
	sleep(1);
	mlx_put_image_to_window(g()->mlx.ptr, win, g()->map.west.ptr, 75, 75);
	sleep(1);

	mlx_destroy_window(g()->mlx.ptr, win);
	sleep(1);
}
