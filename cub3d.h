/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:10:01 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/19 18:32:38 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdlib.h>     // malloc, free, size_t, exit, etc...
# include <stdio.h>      // printf
# include <sys/time.h>   // gettimeofday
# include <unistd.h>     // read, write

# define __USE_MISC      // Math constants (M_PI)
# include <math.h>       // (almost all of them)

# include "mlx.h"        // mlx_*
# include "ft_alloc.h"   // ft_calloc, ft_free, ft_clean // good stuff

# define RIGHT_ARROW 65363
# define LEFT_ARROW 65361

# define SPEED 3
# define ROT_SPEED 3

/******************************************************************************/

enum e_events
{
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEDOWN = 4,
	ON_MOUSEUP = 5,
	ON_MOUSEMOVE = 6,
	ON_DESTROY = 17,
};

typedef struct s_img	t_img;
struct s_img
{
	void	*ptr;
	int		width;
	int		height;
	char	*data;
	int		bpp;
	int		sl;
	int		endian;
};

typedef struct s_mlx	t_mlx;
struct s_mlx
{
	void	*ptr;
	void	*win;
	int		width;
	int		height;
	char	*title;
};

typedef struct s_usr	t_usr;
struct s_usr
{
	float	posx;
	float	posy;
	float	dirx;
	float	diry;
	float	plx;
	float	ply;
};

typedef struct s_map	t_map;
struct s_map
{
	int		*data;
	int		width;
	int		height;
	int		ceil_color;
	int		floor_color;
	t_img	north;
	t_img	south;
	t_img	east;
	t_img	west;
};

typedef struct s_time	t_ime;
struct s_time
{
	float	fps;
	float	dt;
};

typedef struct s_g		t_g;
struct s_g
{
	t_mlx	mlx;
	t_img	frame;
	t_map	map;
	t_usr	usr;
	t_ime	time;
	int		argc;
	char	**argv;
	int		key_pressed[177];
	int		exit_status;
	int		debug;
};

typedef struct s_dda	t_dda;
struct s_dda
{
	float	rdx;
	float	rdy;
	float	ddx;
	float	ddy;
	float	sdx;
	float	sdy;
	float	wdist;
	int		mx;
	int		my;
	int		sx;
	int		sy;
	int		hit;
	int		htry;
	int		side;
};

/******************************************************************************/

// main.c
int		get_map_value(int x, int y);

// time.c
size_t	time_now(void);
void	time_update(void);

// global.c
t_g		*g(void);

// init.c
void	init_frame(void);
void	init_window(void);
int		init_map_tmp(void);

// event.c
int		event_quit(void);
int		event_keydown(int keycode);
int		event_keyup(int keycode);

// raycast.c
t_dda	raycast_start(int x);
void	raycast_hit(t_dda *dda);
void	raycast_draw(int x, t_dda *dda);
void	raycast(void);

// action.c
void	rotate(float speed);
void	move(float speed);

// draw.c
void	put_pixel(int x, int y, int color);
void	draw_vertical_line(int x, int ystart, int yend, int color);

// ft_memmove.c
void	*ft_memmove(void *dest, const void *src, size_t n);

#endif // CUB3D_H
