/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:54:28 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/19 18:30:32 by ecarvalh         ###   ########.fr       */
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
	t_mlx	*mlx;

	g()->argc = argc;
	g()->argv = argv;
	g()->debug = 1;
	g()->map.ceil_color = 0xff3322dd;
	g()->map.floor_color = 0xff333333;
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
