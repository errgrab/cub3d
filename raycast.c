/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:27:15 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/24 17:21:08 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_dda	raycast_start(int x);
void	raycast_hit(t_dda *dda);
void	raycast_draw(int x, t_dda *dda);
void	raycast(void);

int	raycast_calctx(t_dda *dda, t_img *img)
{
	double	wx;
	int		tx;

	if (dda->side == 0)
		wx = g()->usr.posy + dda->wdist * dda->rdy;
	else
		wx = g()->usr.posx + dda->wdist * dda->rdx;
	wx -= floor(wx);
	tx = (int)(wx * (double)img->width);
	if (dda->side == 0 && dda->rdx > 0)
		tx = img->width - tx - 1;
	if (dda->side == 1 && dda->rdx < 0)
		tx = img->width - tx - 1;
	return (tx);
}

t_dda	raycast_start(int x)
{
	const float	camx = 2 * x / (float)g()->frame.width - 1;
	t_dda		res;

	res.rdx = g()->usr.dirx + g()->usr.plx * camx;
	res.rdy = g()->usr.diry + g()->usr.ply * camx;
	res.mx = (int)g()->usr.posx;
	res.my = (int)g()->usr.posy;
	res.ddx = fabs(1 / res.rdx);
	res.ddy = fabs(1 / res.rdy);
	res.sx = 1;
	res.sdx = (res.mx + 1 - g()->usr.posx) * res.ddx;
	if (res.rdx < 0)
		res.sx = -1;
	if (res.rdx < 0)
		res.sdx = (g()->usr.posx - res.mx) * res.ddx;
	res.sy = 1;
	res.sdy = (res.my + 1 - g()->usr.posy) * res.ddy;
	if (res.rdy < 0)
		res.sy = -1;
	if (res.rdy < 0)
		res.sdy = (g()->usr.posy - res.my) * res.ddy;
	return (res);
}

void	raycast_hit(t_dda *dda)
{
	dda->hit = 0;
	while (dda->hit == 0)
	{
		if (dda->sdx < dda->sdy)
		{
			dda->sdx += dda->ddx;
			dda->mx += dda->sx;
			dda->side = 0;
		}
		else
		{
			dda->sdy += dda->ddy;
			dda->my += dda->sy;
			dda->side = 1;
		}
		if (get_map_value(dda->mx, dda->my) > 0)
			dda->hit = 1;
		else if (get_map_value(dda->mx, dda->my) < 0)
			break ;
	}
}

void	raycast_draw(int x, t_dda *dda)
{
	int	lheight;
	int	dstart;
	int	dend;

	if (dda->side == 0)
		dda->wdist = (dda->mx - g()->usr.posx + (1 - dda->sx) / 2) / dda->rdx;
	else
		dda->wdist = (dda->my - g()->usr.posy + (1 - dda->sy) / 2) / dda->rdy;
	lheight = (int)(g()->frame.height / dda->wdist);
	dstart = -lheight / 2 + g()->frame.height / 2;
	if (dstart < 0)
		dstart = 0;
	dend = lheight / 2 + g()->frame.height / 2;
	if (dend >= g()->frame.height)
		dend = g()->frame.height - 1;
	if (dda->side == 0)
	{
		draw_vertical_line(x, 0, dstart, g()->map.ceil_color);
		draw_vertical_line(x, dstart, dend, 0xFFFFFF);
		draw_vertical_line(x, dend, g()->frame.height, g()->map.floor_color);
		return ;
	}
	draw_vertical_line(x, 0, dstart, g()->map.ceil_color);
	draw_vertical_line(x, dstart, dend, 0xAAAAAA);
	draw_vertical_line(x, dend, g()->frame.height, g()->map.floor_color);
}

void	raycast(void)
{
	t_dda	res;
	int		x;

	x = 0;
	while (x < g()->frame.width)
	{
		res = raycast_start(x);
		raycast_hit(&res);
		raycast_draw(x, &res);
		x++;
	}
}
