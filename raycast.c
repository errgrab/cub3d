/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:27:15 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/25 14:54:43 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_dda	raycast_start(int x);
void	raycast_hit(t_dda *dda);
void	raycast_draw(t_dda *dda, t_img *img);
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
	if ((dda->side == 0 && dda->rdx < 0) || (dda->side == 1 && dda->rdy > 0))
		tx = img->width - tx - 1;
	return (tx);
}

t_dda	raycast_start(int x)
{
	const float	camx = 2 * x / (float)g()->frame.width - 1;
	t_dda		res;

	res.x = x;
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
	if (dda->side == 0)
		dda->wdist = (dda->mx - g()->usr.posx + (1 - dda->sx) / 2) / dda->rdx;
	else
		dda->wdist = (dda->my - g()->usr.posy + (1 - dda->sy) / 2) / dda->rdy;
}

void	raycast_draw(t_dda *dda, t_img *img)
{
	int	lheight;
	int	y;
	int	tx;
	int	ty;
	int	color;

	lheight = (int)(g()->frame.height / dda->wdist);
	dda->dstart = -lheight / 2 + g()->frame.height / 2;
	dda->dend = lheight / 2 + g()->frame.height / 2;
	if (dda->dstart < 0)
		dda->dstart = 0;
	if (dda->dend >= g()->frame.height)
		dda->dend = g()->frame.height - 1;
	tx = raycast_calctx(dda, img);
	y = dda->dstart;
	draw_vertical_line(dda->x, 0, dda->dstart, g()->map.ceil_color);
	while (y < dda->dend)
	{
		ty = (int)(((y - dda->dstart) * img->height) / (dda->dend
					- dda->dstart));
		color = *(int *)(img->data + (ty * img->sl + tx * (img->bpp / 8)));
		put_pixel(dda->x, y++, color);
	}
	draw_vertical_line(dda->x, dda->dend, g()->frame.height,
		g()->map.floor_color);
}

void	raycast(void)
{
	t_dda	res;

	res.x = 0;
	while (res.x < g()->frame.width)
	{
		res = raycast_start(res.x);
		raycast_hit(&res);
		raycast_draw(&res, &g()->map.north);
		res.x++;
	}
}
