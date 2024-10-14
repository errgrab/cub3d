/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:10:01 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/13 21:03:14 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <X11/X.h>      // macros
# include <X11/keysym.h> // macros _keyboard_
# include <stdlib.h>     // malloc, free, size_t, exit, etc...
# include <stdio.h>      // printf
# include <sys/time.h>   // gettimeofday
# include <errno.h>      // perror(3)
# include <stdbool.h>    // bool type
# include <unistd.h>     // read, write

# define __USE_MISC      // Math constants (M_PI)
# include <math.h>       // (almost all of them)

# include "mlx.h"        // mlx_*
# include "alloc.h"      // ft_calloc, ft_free, ft_clean // good stuff

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
	int	*data;
	int	width;
	int	height;
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
	int		mx;
	int		my;
	int		sx;
	int		sy;
	int		hit;
	int		side;
	int		walldist;
};

/******************************************************************************/

// time.c
size_t	time_now(void);
void	time_update(void);

// HERE
t_g		*g(void);
void	*_memmove(void *dest, const void *src, size_t n);

/******************************************************************************/

# ifdef CUB3D_IMPL

t_g	*g(void)
{
	static t_g	g = {0};

	return (&g);
}

void	*_memmove(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s = (char *)src;

	if (!dest || !src)
		return (NULL);
	if (dest == src)
		return (dest);
	d = (char *)dest;
	if (d < s)
		while (n--)
			*d++ = *s++;
	else
		while (n--)
			d[n] = s[n];
	return (dest);
}

# endif // CUB3D_IMPL
#endif // CUB3D_H
