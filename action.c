/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:29:51 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/19 18:33:28 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	rotate(float speed);
void	move(float speed);

void	rotate(float speed)
{
	const float	rot_dir = g()->key_pressed[15] - g()->key_pressed[14];
	const float	rotvel = rot_dir * speed * g()->time.dt;
	const float	odx = g()->usr.dirx;
	const float	opx = g()->usr.plx;

	g()->usr.dirx = odx * cos(rotvel) - g()->usr.diry * sin(rotvel);
	g()->usr.diry = odx * sin(rotvel) + g()->usr.diry * cos(rotvel);
	g()->usr.plx = opx * cos(rotvel) - g()->usr.ply * sin(rotvel);
	g()->usr.ply = opx * sin(rotvel) + g()->usr.ply * cos(rotvel);
}

void	move(float speed)
{
	const float	ver_dir = g()->key_pressed['w'] - g()->key_pressed['s'];
	const float	hoz_dir = g()->key_pressed['d'] - g()->key_pressed['a'];

	if (!get_map_value((int)(g()->usr.posx + ver_dir * speed * g()->usr.dirx
			* g()->time.dt), (int)g()->usr.posy))
		g()->usr.posx += ver_dir * speed * g()->usr.dirx * g()->time.dt;
	if (!get_map_value((int)g()->usr.posx, (int)(g()->usr.posy + ver_dir
			* speed * g()->usr.diry * g()->time.dt)))
		g()->usr.posy += ver_dir * speed * g()->usr.diry * g()->time.dt;
	if (!get_map_value((int)(g()->usr.posx + hoz_dir * speed
			* -g()->usr.diry * g()->time.dt), (int)g()->usr.posy))
		g()->usr.posx += hoz_dir * speed * -g()->usr.diry * g()->time.dt;
	if (!get_map_value((int)g()->usr.posx, (int)(g()->usr.posy + hoz_dir
			* speed * g()->usr.dirx * g()->time.dt)))
		g()->usr.posy += hoz_dir * speed * g()->usr.dirx * g()->time.dt;
}
