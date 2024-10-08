/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:10:01 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/09 19:31:09 by ecarvalh         ###   ########.fr       */
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

typedef struct s_v2	t_v2;
struct s_v2
{
	float	x;
	float	y;
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
	t_v2	pos;
	t_v2	dir;
	t_v2	plane;
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

// alloc.c
void	*ft_calloc(size_t n, size_t s);
void	ft_free(void *ptr);
void	ft_clean(void);

#endif // CUB3D_H
