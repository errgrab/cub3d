/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:10:01 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/01 18:51:49 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define CW 0xffffffff
# define C0 0x00000000

# define CR 0xffff0000
# define CG 0xff00ff00
# define CB 0xff0000ff

# define CM 0xffff00ff
# define CY 0xffffff00
# define CC 0xff00ffff

typedef unsigned int	t_color;

typedef struct s_v2f	t_v2f;
struct s_v2f
{
	float	x;
	float	y;
};

typedef struct s_v2i	t_v2i;
struct s_v2i
{
	int	x;
	int	y;
};

typedef struct s_list	t_list;
struct s_list
{
	void	*ptr;
	t_list	*next;
};

typedef struct s_img	t_img;
struct s_img
{
	void	*ptr;
	char	*buf;
	int		width;
	int		height;
	int		bpp;
	int		sl;
	int		endian;
};

typedef struct s_mlx	t_mlx;
struct s_mlx
{
	void	*ptr;
	void	*win;
	t_img	frame;
};

typedef struct s_map	t_map;
struct s_map
{
	int		*data;
	size_t	width;
	size_t	height;
};

typedef struct s_usr	t_usr;
struct s_usr
{
	t_v2f	pos;
	t_v2f	dir;
	t_v2f	plane;
};

typedef struct s_time	t_time;
struct s_time
{
	size_t	before;
	size_t	now;
	size_t	fps;
	float	dt;
};

typedef struct s_global	t_global;
struct	s_global
{
	t_mlx	mlx;
	t_map	map;
	t_usr	usr;
	t_time	time;
};

typedef struct s_dda	t_dda;
struct s_dda
{
	float	xc;
	t_v2f	dir;
	t_v2f	pos;
	t_v2i	ipos;
	t_v2f	dd;
	t_v2f	sd;
	t_v2i	step;
};

// alloc.c
void	*ft_calloc(size_t n, size_t s);
void	ft_free(void *ptr);
void	ft_clean(void);

#endif // CUB3D_H
